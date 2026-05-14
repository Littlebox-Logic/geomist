#include <geomist/shader.h>
#include <geomist/display.h>
#include <geomist/log.h>

#include <SDL3/SDL.h>
#include <stdlib.h>

static SDL_GPUGraphicsPipeline	*pipeline			= nullptr;
static SDL_GPUBuffer			*vertex_buffer		= nullptr;
static SDL_GPUShader			*ellipse_mist_vert	= nullptr;
static SDL_GPUShader			*mist_frag			= nullptr;
static _Uniforms				_uniforms			= (_Uniforms)
{
	.time	 = 0.0f,
	.x_c	 = 0.0f,
	.y_c	 = 0.0f,
	.r		 = 0.0f,
	.g		 = 1.0f,
	.b		 = 0.0f,
	.scale_x = 1.0f,
	.scale_y = 1.0f
};
Uniforms						uniforms			= &_uniforms;
SDL_GPUCommandBuffer			*cmd;

const uint8_t _ellipse_mist_vert[] =
{
	#embed "../obj/vert/ellipse_mist.spv"
};
const uint8_t _mist_frag[] =
{
	#embed "../obj/frag/mist.spv"
};

int shader_import(void)
{
	SDL_GPUShaderCreateInfo ellipse_mist_vert_i =
	{
		.code_size				= sizeof(_ellipse_mist_vert),
		.code					= _ellipse_mist_vert,
		.entrypoint				= "main",
		.format					= SDL_GPU_SHADERFORMAT_SPIRV,
		.stage					= SDL_GPU_SHADERSTAGE_VERTEX,
		.num_samplers			= 0,
		.num_storage_textures	= 0,
		.num_storage_buffers	= 0,
		.num_uniform_buffers	= 1
	};

	SDL_GPUShaderCreateInfo mist_frag_i =
	{
		.code_size				= sizeof(_mist_frag),
		.code					= _mist_frag,
		.entrypoint				= "main",
		.format					= SDL_GPU_SHADERFORMAT_SPIRV,
		.stage					= SDL_GPU_SHADERSTAGE_FRAGMENT,
		.num_samplers			= 0,
		.num_storage_textures	= 0,
		.num_storage_buffers	= 0,
		.num_uniform_buffers	= 0

	};

	ellipse_mist_vert	= SDL_CreateGPUShader(device, &ellipse_mist_vert_i);
	mist_frag			= SDL_CreateGPUShader(device, &mist_frag_i);

	return EXIT_SUCCESS;
}

int shader_init(void)
{
	Vertex points = nullptr;
	SDL_GPUBufferCreateInfo buffer_info =
	{
		.usage	= SDL_GPU_BUFFERUSAGE_VERTEX,
		.size	= sizeof(_Vertex) * disp_width * disp_height
	};

	if (!(points = SDL_malloc(buffer_info.size)))
	{
		printf("h");
		SDL_Log("Failed to allocate memory for points: %s.", SDL_GetError());
		return EXIT_FAILURE;
	}

	for (size_t x_idx = 0; x_idx < disp_width; x_idx ++)
		for (size_t y_idx = 0; y_idx < disp_height; y_idx ++)
		{
			points[y_idx * disp_width + x_idx].x	= x_idx; // % disp_width  / (float)disp_width  * 2.0f - 1.0f;
			points[y_idx * disp_width + x_idx].y	= y_idx; // % disp_height / (float)disp_height * 2.0f - 1.0f;
			points[y_idx * disp_width + x_idx].z	= 0.0f;
			points[y_idx * disp_width + x_idx]._pad	= 0.0f;
		}

	vertex_buffer = SDL_CreateGPUBuffer(device, &buffer_info);
	SDL_GPUTransferBufferCreateInfo trans_buf_info =
	{
		.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
		.size = buffer_info.size
	};
	SDL_GPUTransferBuffer	*transfer	= SDL_CreateGPUTransferBuffer(device, &trans_buf_info);
	memcpy(SDL_MapGPUTransferBuffer(device, transfer, false), points, buffer_info.size);
	SDL_UnmapGPUTransferBuffer(device, transfer);

	cmd = SDL_AcquireGPUCommandBuffer(device);
	SDL_GPUCopyPass			*copy		= SDL_BeginGPUCopyPass(cmd);

	SDL_GPUTransferBufferLocation trans_buf_loc =
	{
		.transfer_buffer = transfer,
		.offset = 0
	};
	SDL_GPUBufferRegion buf_region =
	{
		.buffer	= vertex_buffer,
		.offset	= 0,
		.size	= buffer_info.size
	};

	SDL_UploadToGPUBuffer(copy, &trans_buf_loc, &buf_region, false);
	SDL_EndGPUCopyPass(copy);
	SDL_SubmitGPUCommandBuffer(cmd);
	SDL_ReleaseGPUTransferBuffer(device, transfer);

	SDL_free(points);

	SDL_GPUVertexBufferDescription vb_desc =
	{
		.slot		= 0,
		.pitch		= sizeof(Vertex),
		.input_rate	= SDL_GPU_VERTEXINPUTRATE_VERTEX
	};

	SDL_GPUColorTargetDescription color_desc =
	{
		.format			= SDL_GetGPUSwapchainTextureFormat(device, window),
		.blend_state	= (SDL_GPUColorTargetBlendState)
		{
			.enable_blend			= true,
			.src_color_blendfactor	= SDL_GPU_BLENDFACTOR_SRC_ALPHA,
			.dst_color_blendfactor	= SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
			.color_blend_op			= SDL_GPU_BLENDOP_ADD,
			.src_alpha_blendfactor	= SDL_GPU_BLENDFACTOR_ONE,
			.dst_alpha_blendfactor	= SDL_GPU_BLENDFACTOR_ZERO,
			.alpha_blend_op			= SDL_GPU_BLENDOP_ADD
		}
	};

	SDL_GPUGraphicsPipelineCreateInfo pipeline_info =
	{
		.vertex_shader		= ellipse_mist_vert,
		.fragment_shader	= mist_frag,
		.vertex_input_state	= (SDL_GPUVertexInputState)
		{
			.num_vertex_buffers			= 1,
			.vertex_buffer_descriptions	= &vb_desc,
			.num_vertex_attributes		= 1,
			.vertex_attributes			= &(SDL_GPUVertexAttribute)
			{
				.location		= 0,
				.buffer_slot	= 0,
				.format			= SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
				.offset			= 0
			}
		},
		.primitive_type	= SDL_GPU_PRIMITIVETYPE_POINTLIST,
		.target_info	= (SDL_GPUGraphicsPipelineTargetInfo)
		{
			.num_color_targets			= 1,
			.color_target_descriptions	= &color_desc
		}
	};

	pipeline = SDL_CreateGPUGraphicsPipeline(device, &pipeline_info);

	return EXIT_SUCCESS;
}

void shader_commit(void)
{
	SDL_GPUTexture *swapchain;
	uint32_t width, height;

	cmd = SDL_AcquireGPUCommandBuffer(device);
	if (SDL_WaitAndAcquireGPUSwapchainTexture(cmd, window, &swapchain, &width, &height))
	{
		SDL_GPUColorTargetInfo target =
		{
			.texture		= swapchain,
			.clear_color	= (SDL_FColor){1.0f, 1.0f, 1.0f, 1.0f},
			.load_op		= SDL_GPU_LOADOP_CLEAR,
			.store_op		= SDL_GPU_STOREOP_STORE
		};
		SDL_GPUBufferBinding vertex_binding =
		{
			.buffer = vertex_buffer,
			.offset = 0
		};

		SDL_GPURenderPass *pass = SDL_BeginGPURenderPass(cmd, &target, 1, nullptr);
		SDL_BindGPUGraphicsPipeline(pass, pipeline);
		SDL_BindGPUVertexBuffers(pass, 0, &vertex_binding, 1);
		SDL_PushGPUVertexUniformData(cmd, 0, uniforms, sizeof(_Uniforms));
		SDL_DrawGPUPrimitives(pass, disp_width * disp_height, 1, 0, 0);
		SDL_EndGPURenderPass(pass);
	}
	else	SDL_Log("Failed to get swapchain.");

	SDL_SubmitGPUCommandBuffer(cmd);
}

void shader_close(void)
{
	SDL_ReleaseGPUGraphicsPipeline(device, pipeline);
	SDL_ReleaseGPUBuffer(device, vertex_buffer);

	SDL_ReleaseGPUShader(device, ellipse_mist_vert);
	SDL_ReleaseGPUShader(device, mist_frag);
}

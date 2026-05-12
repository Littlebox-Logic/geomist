#include <geomist/shader.h>
#include <geomist/display.h>

#include <SDL3/SDL.h>
#include <stdlib.h>

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

	SDL_GPUShader *ellipse_mist_vert	= SDL_CreateGPUShader(device, &ellipse_mist_vert_i);
	SDL_GPUShader *mist_frag			= SDL_CreateGPUShader(device, &mist_frag_i);

	return EXIT_SUCCESS;
}

int shader_init(void)
{
	Vertex points;
	if (!(points = SDL_malloc(sizeof(_Vertex) * disp_width * disp_height)))
	{
		SDL_Log("Failed to allocate memory for points: %s.", SDL_GetError());
		return EXIT_FAILURE;
	}

	for (size_t index = 0; index < disp_width * disp_height; index ++)
	{
		points[index].x		= index % disp_width / (float)disp_width * 2.0f - 1.0f;
		points[index].y		= 0.0f;
		points[index].z		= 0.0f;
		points[index]._pad	= 0.0f;
	}

	// TODO ...
	return EXIT_SUCCESS;
}

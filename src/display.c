#include <geomist/display.h>
#include <geomist/log.h>
#include <stdlib.h>

static SDL_Window		*window		= nullptr;
static SDL_DisplayMode	*disp_mode	= nullptr;
SDL_Renderer			*renderer	= nullptr;
SDL_GPUDevice			*device		= nullptr;
size_t					disp_width  = 0;
size_t					disp_height = 0;

int display_init(void)
{
	int count = 0;
	/*if (!SDL_SetEnvironmentVariable(SDL_GetEnvironment(), "SDL_RENDER_DRIVER", "software", true))
	{
		Log(ERROR, "Failed to set SDL environment variable: %s", SDL_GetError());
		return EXIT_FAILURE;
	}*/

	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		Log(ERROR, "Failed to initialize SDL3 core: %s", SDL_GetError());
		return EXIT_FAILURE;
	}

	if (!(disp_mode = (SDL_DisplayMode *)SDL_GetCurrentDisplayMode(SDL_GetDisplays(&count)[0])))
	{
		Log(ERROR, "Failed to get display mode: %s", SDL_GetError());
		goto QUIT;
	}

	disp_width	= disp_mode -> w / 2;
	disp_height	= disp_mode -> h / 2;

	if (!(window = SDL_CreateWindow("Geomist", disp_width, disp_height, SDL_WINDOW_HIGH_PIXEL_DENSITY)))
	{
		Log(ERROR, "Failed to create window: %s", SDL_GetError());
		goto QUIT;
	}

	SDL_PropertiesID props = SDL_CreateProperties();
	SDL_SetPointerProperty(props, SDL_PROP_RENDERER_CREATE_WINDOW_POINTER, window);
	SDL_SetNumberProperty(props, SDL_PROP_RENDERER_CREATE_PRESENT_VSYNC_NUMBER, 1);

	if (!(renderer = SDL_CreateRendererWithProperties(props)))
	{
		Log(ERROR, "Failed to create renderer: %s", SDL_GetError());
		goto CLEANUP;
	}

	if (!(SDL_SetRenderVSync(renderer, 1)))
	{
		Log(ERROR, "Failed to enable VSync: %s", SDL_GetError());
		goto DESTROY;
	}

	SDL_DestroyProperties(props);
	SDL_FRect bg_rect = {0, 0, disp_width, disp_height};
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	if (!(device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, "gpu_render", NULL)))
	{
		SDL_Log("Failed to get GPU device: %s.", SDL_GetError());
		goto DESTROY;
	}

	SDL_ClaimWindowForGPUDevice(device, window);
	SDL_SetGPUSwapchainParameters(device, window, SDL_GPU_SWAPCHAINCOMPOSITION_SDR, SDL_GPU_PRESENTMODE_IMMEDIATE);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderFillRect(renderer, &bg_rect);
	SDL_RenderPresent(renderer);
	return EXIT_SUCCESS;

DESTROY:
	SDL_DestroyRenderer(renderer);
CLEANUP:
	SDL_DestroyProperties(props);
	SDL_DestroyWindow(window);
QUIT:
	SDL_Quit();
	return EXIT_FAILURE;
}

void display_close(void)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

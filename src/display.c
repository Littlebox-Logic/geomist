#include <geomist/display.h>
#include <geomist/log.h>
#include <stdlib.h>

static SDL_Window		*window		= NULL;
static SDL_DisplayMode	*disp_mode	= NULL;
SDL_Renderer			*renderer	= NULL;
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

	SDL_DestroyProperties(props);
	SDL_FRect bg_rect = {0, 0, disp_width, disp_height};
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderFillRect(renderer, &bg_rect);
	SDL_RenderPresent(renderer);
	return EXIT_SUCCESS;

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

#pragma once

#include <SDL3/SDL.h>
#include <stdint.h>

int		display_init(void);
void	display_close(void);

extern size_t disp_width;
extern size_t disp_height;

extern SDL_Window		*window;
extern SDL_GPUDevice	*device;
extern SDL_Renderer		*renderer;

inline void pixel(float x, float y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	if (!SDL_RenderPoint(renderer, x, y))
		SDL_Log("Failed to write pixel: %s", SDL_GetError());
}

inline void disp_flush(void) {SDL_RenderPresent(renderer);}

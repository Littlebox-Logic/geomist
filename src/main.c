#include <geomist/display.h>
#include <geomist/graphics.h>
#include <geomist/shader.h>

#include <stdlib.h>

#define MAX(a, b) (x > y ? x : y)
#define MIN(a, b) (x > y ? y : x)

int main(int argc, char *argv[])
{
	bool running = true;
	SDL_Event event;

	display_init();
	shader_import();
	shader_init();

	// ellipse_mist(disp_width / 2, disp_height / 8 * 3, 0.3f, 0.2f, 255, 255, 128, 128);
	// ellipse_mist(disp_width / 2, disp_height / 2, 1.0f, 0.5f, 128, 64, 255, 255);
	// ellipse_mist(disp_width / 3 * 2, disp_height / 3 * 2, 0.75f, 0.5f, 128, 255, 255, 128);
	rect_mist(disp_width / 2, disp_height / 2, disp_width / 4, disp_height / 5, 128, 64, 255, 128);
	disp_flush();

	while (running)
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_EVENT_QUIT)	running = false;
			SDL_Delay(16);
		}

	display_close();
	return EXIT_SUCCESS;
}

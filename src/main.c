#include <geomist/display.h>
#include <geomist/log.h>

#include <stdlib.h>
#include <math.h>

#define MAX(a, b) (x > y ? x : y)
#define MIN(a, b) (x > y ? y : x)

void demo(void)
{
	for (int x = 0; x < disp_width; x ++)
		for (int y = 0; y < disp_height; y ++)
			pixel((float)x, (float)y, 128, 64, 255,
				(uint8_t)(255 * pow(M_E, - ((x - (int)(disp_width / 2)) * (x - (int)(disp_width / 2)) - (y - (int)(disp_height / 2)) * (y - (int)(disp_height / 2))) / 100000.0f)));

				// sin((float)(x - 1024) * (x - 1024) / y + (float)(x - 1024) * (x - 1024) / y)));
}

int main(int argc, char *argv[])
{
	bool running = true;
	SDL_Event event;

	display_init();
	demo();
	disp_flush();

	while (running)
		while (SDL_PollEvent(&event))
			if (event.type == SDL_EVENT_QUIT)	running = false;

	display_close();
	return EXIT_SUCCESS;
}

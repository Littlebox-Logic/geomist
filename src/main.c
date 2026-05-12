#include <geomist/display.h>
#include <geomist/demo.h>

#include <stdlib.h>

#define MAX(a, b) (x > y ? x : y)
#define MIN(a, b) (x > y ? y : x)

int main(int argc, char *argv[])
{
	bool running = true;
	SDL_Event event;

	display_init();
	demo_3();
	disp_flush();

	while (running)
		while (SDL_PollEvent(&event))
			if (event.type == SDL_EVENT_QUIT)	running = false;

	display_close();
	return EXIT_SUCCESS;
}

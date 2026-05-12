#include <geomist/demo.h>
#include <geomist/display.h>

#include <stdint.h>
#include <math.h>

void demo_1(void)
{
	for (int x = 0; x < disp_width; x ++)
		for (int y = 0; y < disp_height; y ++)
			pixel((float)x, (float)y, 128, 64, 255,
				(uint8_t)(255 * pow(M_E, - ((x - (int)(disp_width / 2)) * (x - (int)(disp_width / 2)) - (y - (int)(disp_height / 2)) * (y - (int)(disp_height / 2))) / 100000.0f))); 
}

void demo_2(void)
{
	for (int x = 0; x < disp_width; x ++)
		for (int y = 0; y < disp_height; y ++)
			pixel((float)x, (float)y, 128, 64, 255,
				(uint8_t)(255 * pow(M_E, - ((x - (int)(disp_width / 2)) * (x - (int)(disp_width / 2)) + (y - (int)(disp_height / 2)) * (y - (int)(disp_height / 2))) / 100000.0f))); 
}

void demo_3(void)
{
	for (int x = 0; x < disp_width; x ++)
		for (int y = 0; y < disp_height; y ++)
			pixel((float)x, (float)y, 255, 64, 64,
				(uint8_t)(255 * pow(M_E, - ((x - (int)(disp_width / 2)) * (x - (int)(disp_width / 2)) + (y - (int)(disp_height / 2)) * (y - (int)(disp_height / 2))) / 100000.0f) * 
				(1 - sin((x - (int)(disp_width / 2)) * (x - (int)(disp_width / 2)) + (y - (int)(disp_height / 2)) * (y - (int)(disp_height / 2)))
				 - sin(3 * ((x - (int)(disp_width / 2)) * (x - (int)(disp_width / 2)) + (y - (int)(disp_height / 2)) * (y - (int)(disp_height / 2)))) / 3
				 - sin(5 * ((x - (int)(disp_width / 2)) * (x - (int)(disp_width / 2)) + (y - (int)(disp_height / 2)) * (y - (int)(disp_height / 2)))) / 5)));
}

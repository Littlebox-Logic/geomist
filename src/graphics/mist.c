#include <geomist/display.h>
#include <geomist/graphics.h>

#include <math.h>

// Draws a misty ellipse centered at (x_c, y_c) with the specified scale and color.
void ellipse_mist(size_t x_c, size_t y_c, float scale_x, float scale_y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	for (int x = 0; x < disp_width; x ++)
		for (int y = 0; y < disp_height; y ++)
			pixel((float)x, (float)y, r, g, b,
				(uint8_t)(255 * pow(M_E, - (SQUARE((x - (int)x_c) / scale_x) + SQUARE((y - (int)(y_c)) / scale_y)) / 100000.0f))); 
}

void rect_mist(size_t x_c, size_t y_c, size_t width, size_t height, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	for (int x = 0; x < disp_width; x ++)
		for (int y = 0; y < disp_height; y ++)
			pixel((float)x, (float)y, r, g, b,
				(uint8_t)(0.25 * a * (tanh(5 * ((float)width - (x - (int)x_c)) / (float)(width)) + tanh(5 * ((float)width + (x - (int)x_c)) / (float)(width))) * (tanh(5 * ((float)height - (y - (int)y_c)) / (float)(height)) + tanh(5 * ((float)height + (y - (int)y_c)) / (float)(height)))));
}
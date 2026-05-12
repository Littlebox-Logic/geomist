#pragma once

#include <stdint.h>

extern const uint8_t ellipse_mist_vert[];
extern const uint8_t mist_frag[];

typedef struct
{
	float x, y, z;
	float _pad;
}	*Vertex, _Vertex;

typedef struct
{
	float time;
	float x_c, y_c;
	float scale_x, scale_y;
	float r, g, b;
}	*Uniforms, _Uniforms;

int shader_import(void);
int shader_init(void);

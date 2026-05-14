#pragma once

#include <stdint.h>

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

extern Uniforms uniforms;

int		shader_import(void);
int		shader_init(void);
void	shader_commit(void);
void	shader_close(void);

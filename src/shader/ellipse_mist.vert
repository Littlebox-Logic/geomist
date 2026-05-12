#version 450

#define M_PI	3.14159265358979323846264338327
#define M_E		2.71828182845904523536028747135

layout(push_constant)	uniform	Uniforms
{
	float time;
	float x_c, y_c;
	float scale_x, scale_y;
	float r, g, b;
}	ubo;
layout(location = 0)	in		vec3 in_posi;
layout(location = 0)	out		vec4 out_color;

void main(void)
{
	float x = in_posi.x;
	float y = in_posi.y;
	gl_Position = vec4(x, y, 0.0, 1.0);
	out_color = vec4(ubo.r, ubo.g, ubo.b,
		mod(pow(M_E, - (pow((x - ubo.x_c) / ubo.scale_x, 2.0) + pow((y - ubo.y_c) / ubo.scale_y, 2.0)) / 100000.0), 1.0));
}
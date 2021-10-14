#version 410
in vec3 color;
out vec4 colorFragmento;

void main()
{
	colorFragmento = vec4(color, 1.0);
}
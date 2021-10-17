#version 410
layout (location = 0) in vec3 posicion;
layout (location = 1) in vec3 colorVertice;

uniform mat4 matProj;
uniform mat4 matView;

out vec3 color;

void main ()
{
	//gl_Position = vec4 ( posicion, 1 );
	gl_Position = matProj * matView * vec4(posicion,1);
	//gl_Position = MVP * vec4 ( posicion, 1 );
	color = colorVertice;
}
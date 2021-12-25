#version 410

layout (location = 0) in vec3 vVertex;

uniform mat4 matModVisProj;

void main()
{
	gl_Position = matModVisProj * vec4(vVertex, 1);
}
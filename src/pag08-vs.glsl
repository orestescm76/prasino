#version 410
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;

uniform mat4 matProjViewModel;
uniform mat4 matModel;

out vec3 pos;
out vec3 normal;

void main ()
{
	//gl_Position = vec4 ( posicion, 1 );
	gl_Position = matProjViewModel * vec4(vPos, 1);
	pos = vec3(matModel * vec4(vPos, 1));
	normal = vec3(matModel * vec4(vNormal,0));
}
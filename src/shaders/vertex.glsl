#version 410
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoords;

uniform mat4 matProjViewModel;
uniform mat4 matModelView;
uniform mat4 matModelViewTransInv;
uniform mat4 matShadow;

out outputVS
{
	vec3 pos;
	vec3 normal;
	vec2 texCoords;
	vec4 shadowCoords;
} outVS;

void main ()
{
	outVS.normal = vec3(matModelViewTransInv * vec4(vNormal, 0.0));
	outVS.pos = vec3(matModelView * vec4(vPos, 1.0));
	outVS.texCoords = vTexCoords;
	//gl_Position = vec4 ( posicion, 1 );
	outVS.shadowCoords = matShadow*vec4(vPos,1);
	//this DOES REALLY fixes shadow acne
	outVS.shadowCoords.z -= 0.005;
	gl_Position = matProjViewModel * vec4(vPos, 1.0);
}
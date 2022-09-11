#version 410
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoords;
layout (location = 3) in vec3 vTangents;
layout (location = 4) in vec3 vBiTangents;

uniform mat4 matProjViewModel;
uniform mat4 matModelView;
uniform mat4 matShadow;

out outputVS
{
	vec3 pos;
	vec2 texCoords;
	vec3 posTg;
	vec4 
	;
} outVS;

out mat3 TBNI;

void main ()
{
	vec3 N = normalize(vec3(matModelView*vec4(vNormal,0)));
	vec3 T = normalize(vec3(matModelView*vec4(vTangents,0)));
	vec3 B = cross(N,T);
	TBNI = transpose(mat3(T,B,N));
	vec3 P = vec3(matModelView * vec4(vPos, 1.0));
	outVS.pos = P;
	outVS.texCoords = vTexCoords;
	outVS.posTg = TBNI * P;
	outVS.shadowCoords = matShadow*vec4(vPos,1);
	gl_Position = matProjViewModel * vec4(vPos, 1.0);
}
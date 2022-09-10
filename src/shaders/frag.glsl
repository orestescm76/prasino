#version 410 core

in outputVS
{
	vec3 pos;
	vec3 normal;
	vec2 texCoords;
	vec4 shadowCoords;
} inputVS;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;

uniform vec3 Ia;
uniform vec3 Id;
uniform vec3 Is;

uniform vec3 lPos;
uniform vec3 lDir;
uniform float shininess;
uniform float sAngle;

uniform sampler2D texSampler;
uniform sampler2DShadow samplerShadow;

layout (location = 0) out vec4 fragColor;
//Set the subroutine
subroutine vec3 lightMode(vec3 difColor);
//Set the uniform to the subroutine and give it a name we can use
subroutine uniform lightMode light;

subroutine vec3 getColor();
subroutine uniform getColor color;

subroutine (lightMode)
vec3 wire(vec3 difColor)
{
	//returns a simple blue color
	return difColor;
}

subroutine (lightMode)
vec3 ambientColor(vec3 difColor)
{
	return Ia * difColor;
}

subroutine (lightMode)
vec3 point(vec3 difColor)
{
	vec3 n = normalize(inputVS.normal);
	vec3 l = normalize(lPos - inputVS.pos);
	vec3 v = normalize(-inputVS.pos);
	
	vec3 r = normalize(reflect(-l,n));
	vec3 diff = (Id * difColor * max( dot (l,n), 0.0));
	vec3 spec = (Is * Ks * pow( max( dot(r,v),0.0),shininess));
	return diff+spec;
}

subroutine (lightMode)
vec3 directional(vec3 difColor)
{
	float shadow = textureProj(samplerShadow, inputVS.shadowCoords);
	float shadowFactor = clamp(shadow, 0.005, 1.0);
	vec3 n = normalize(inputVS.normal);
	vec3 l = -lDir;
	vec3 v = normalize(-inputVS.pos);
	
	vec3 r = reflect(-l,n);


	vec3 diff = (Id * difColor * max( dot (l,n), 0.0));
	vec3 spec = (Is * Ks * pow( max( dot(r,v), 0.0),shininess));
	return shadowFactor*(diff+spec);
}

subroutine (lightMode)
vec3 spot(vec3 difColor)
{
	float shadow = textureProj(samplerShadow, inputVS.shadowCoords);
	//l -> dir luz
	vec3 l = normalize(lPos-inputVS.pos);
	vec3 d = normalize(lDir);
	float cosG = cos(sAngle);
	float sFactor = 1.0;
	float cosD = dot(l, -d);
	if(cosD < cosG)
	{
		cosD = 0.0;
	}
	vec3 n = normalize(inputVS.normal);
	vec3 v = normalize(-inputVS.pos);
	vec3 r = reflect(-l,n);
	vec3 diff = (Id*difColor * max(dot(l,n),0.0));
	vec3 spec = (Is*Ks * pow( max( dot(r,v), 0.0 ) , shininess));
	sFactor = pow(cosD, 64);
	return shadow*sFactor*(diff+spec);
}

subroutine (getColor)
vec3 textured()
{
	return texture(texSampler, inputVS.texCoords).rgb;
}

subroutine (getColor)
vec3 simpleColorAmbient()
{
	return Ka;
}

subroutine (getColor)
vec3 simpleColorDiffuse()
{
	return Kd;
}


void main()
{
	fragColor = vec4(light(color()),1.0);
}
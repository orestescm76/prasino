#version 410

in outputVS
{
	vec3 pos;
	vec3 normal;
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

layout (location = 0) out vec4 fragColor;
//Set the subroutine
subroutine vec3 lightMode();
//Set the uniform to the subroutine and give it a name we can use
subroutine uniform lightMode light;

subroutine (lightMode)
vec3 wire()
{
	//returns a simple blue color
	return vec3(0,0,1);
}

subroutine (lightMode)
vec3 ambientColor()
{
	vec3 amb = (Ka*Ia);
	return amb;
}

subroutine (lightMode)
vec3 point()
{
	vec3 n = normalize(inputVS.normal);
	vec3 l = normalize(lPos - inputVS.pos);
	vec3 v = normalize(-inputVS.pos);
	vec3 r = reflect(-l, n);
	vec3 diff = (Id*Kd * max(dot(l,n),0));
	vec3 spec = (Is*Ks * pow( max( dot(r,v), 0 ) , shininess));
	return diff + spec;
}

subroutine (lightMode)
vec3 directional()
{
	vec3 n = normalize(inputVS.normal);
	vec3 l = -lDir;
	vec3 v = normalize(-inputVS.pos);
	vec3 r = reflect(-l, n);

	vec3 diff = (Id*Kd * max(dot(l,n),0.0));
	vec3 spec;
	spec = (Is*Ks * pow( max( dot(r,v), 0.0 ) , shininess));
	return diff + spec;
}

subroutine (lightMode)
vec3 spot()
{
	vec3 l = normalize(lPos-inputVS.pos);
	vec3 d = lDir;
	float cosG = cos(sAngle);
	float sFactor = 1.0;
	if(dot(-l, d) < cosG)
	{
		sFactor=0.0;
	}
	vec3 n = normalize(inputVS.normal);
	vec3 v = normalize(-inputVS.pos);
	vec3 r = reflect(-l,n);
	vec3 diff = (Id*Kd * max(dot(l,n),0.0));
	vec3 spec;
	spec = (Is*Ks * pow( max( dot(r,v), 0.0 ) , shininess));
	return sFactor*(diff+spec);
}
void main()
{
	fragColor = vec4(light(),1.0);
}
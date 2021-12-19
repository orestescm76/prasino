#version 410

in outputVS
{
	vec3 pos;
	vec2 texCoords;
	vec3 posTg;
} inputVS;

in mat3 TBNI;

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
uniform sampler2D texSamplerNM;

layout (location = 0) out vec4 fragColor;
//Set the subroutine
subroutine vec3 lightMode();
//Set the uniform to the subroutine and give it a name we can use
subroutine uniform lightMode light;

subroutine (lightMode)
vec3 wire()
{
	//returns a simple blue color
	return Kd;
}

subroutine (lightMode)
vec3 ambientColor()
{
	vec3 c = texture(texSampler, inputVS.texCoords).rgb;
	vec3 amb = (c*Ia);
	return amb;
}

subroutine (lightMode)
vec3 point()
{
	vec3 lightPos = TBNI * lPos;
	vec3 n = texture(texSamplerNM, inputVS.texCoords).rgb;
	n = normalize(n*2.0-1.0);
	vec3 c = texture(texSampler, inputVS.texCoords).rgb;

	vec3 l = normalize(lightPos - inputVS.posTg);
	vec3 v = normalize(-inputVS.posTg);
	
	vec3 r = normalize(reflect(-l,n));
	vec3 diff = (Id * c * max( dot (l,n), 0.0));
	vec3 spec = (Is * Ks * pow( max( dot(r,v),0.0),shininess));
	return diff+spec;
}

subroutine (lightMode)
vec3 directional()
{
	vec3 lightDir = TBNI * lDir;
	vec3 n = texture(texSamplerNM, inputVS.texCoords).rgb;
	n = normalize(n*2.0-1.0);
	vec3 c = texture(texSampler, inputVS.texCoords).rgb;
	vec3 l = -lightDir;

	vec3 v = normalize(-inputVS.posTg);
	vec3 r = reflect(-l,n);


	vec3 diff = (Id * c * max( dot (l,n), 0.0));
	vec3 spec = (Is * Ks * pow( max( dot(r,v), 0.0),shininess));
	return diff+spec;
}

subroutine (lightMode)
vec3 spot()
{
	vec3 lightPos = TBNI * lPos;
	vec3 lightDir = TBNI * lDir;
	vec3 n = texture(texSamplerNM, inputVS.texCoords).rgb;
	n = normalize(n*2.0-1.0);

	vec3 c = texture(texSampler, inputVS.texCoords).rgb;
	vec3 l = normalize(lightPos-inputVS.posTg);
	vec3 d = lightDir;
	float cosG = cos(sAngle);
	float sFactor = 1.0;
	float cosD = dot(-l,d);
	if(cosD < cosG)
	{
		cosD = 0.0;
	}
	vec3 v = normalize(-inputVS.posTg);
	vec3 r = reflect(-l,n);
	vec3 diff = (Id*c * max(dot(l,n),0.0));
	vec3 spec= (Is*Ks * pow( max( dot(r,v), 0.0 ) , shininess));
	sFactor = pow(cosD, 16);
	return sFactor*(diff+spec);
}
void main()
{
	fragColor = vec4(light(),1.0);
}
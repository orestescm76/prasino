#version 410

in vec3 pos;
in vec3 normal;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;

uniform vec3 Ia;
uniform vec3 Id;
uniform vec3 Is;

uniform vec3 lPos;
uniform float shininess;

layout (location = 0) out vec4 fragColor;
//Set the subroutine
subroutine vec3 chooseColor();
//Set the uniform to the subroutine and give it a name we can use
subroutine uniform chooseColor renderColor;

subroutine (chooseColor)
vec3 blue()
{
	return vec3(0,0,1);
}

subroutine (chooseColor)
vec3 ambientColor()
{
	vec3 amb = (Ka*Ia);
	return amb;
}

subroutine (chooseColor)
vec3 diffuseAndSpecular()
{
	vec3 n = normalize(normal);
	vec3 l = normalize(lPos-pos);
	vec3 v = normalize(-pos);
	vec3 r = reflect(-l, n);
	vec3 diff = (Id*Kd * max(dot(l,n),0.0));
	vec3 spec = (Is*Ks * pow(max(dot(r,v),0.0),shininess));
	return diff;
}
void main()
{
	fragColor = vec4(renderColor(),1.0);
}
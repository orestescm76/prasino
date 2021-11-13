#version 410

out vec4 fragColor;

uniform vec3 diffuse;
//Set the subroutine
subroutine vec4 chooseColor();
//Set the uniform to the subroutine and give it a name we can use
subroutine uniform chooseColor renderColor;

subroutine (chooseColor)
vec4 blue()
{
	return vec4(0,0,1,1);
}

subroutine (chooseColor)
vec4 diffuseColor()
{
	return vec4(diffuse,1);
}

void main()
{
	fragColor = renderColor();
}
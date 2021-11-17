#include "pch.h"
#include "Light.h"

PAG::Light::Light(glm::vec3 amb) : ambient(amb)
{
	type = LightType::AMBIENT;
}

PAG::Light::Light(glm::vec3 dif, glm::vec3 spec, glm::vec3 pos): diffuse(dif), specular(spec), position(pos)
{
	type = LightType::POINT;
}

PAG::Light::Light(glm::vec3 dif, glm::vec3 spec, glm::vec3 pos, glm::vec3 dir): diffuse(dif), specular(spec), position(pos), direction(dir)
{
	type = LightType::DIRECTIONAL;
}

PAG::Light::~Light()
{

}
#include "pch.h"
#include "Light.h"

PAG::Light::Light(glm::vec3 amb) : ambient(amb)
{
	type = LightType::AMBIENT;
}

PAG::Light::Light(glm::vec3 dif, glm::vec3 spec, glm::vec3 vec, LightType type): diffuse(dif), specular(spec)
{
	this->type = type;
	if (type == LightType::POINT)
		position = vec;
	else if(type == LightType::DIRECTIONAL)
	{
		direction = vec;
		position = direction * glm::vec3(-2);
	}
}

PAG::Light::Light(glm::vec3 dif, glm::vec3 spec, glm::vec3 pos, glm::vec3 dir, float gamma) : diffuse(dif), specular(spec), position(pos), direction(glm::normalize(dir)), angle(gamma)
{
	type = LightType::SPOTLIGHT;
}

PAG::Light::Light(const Light& orig): position(orig.position), ambient(orig.ambient), diffuse(orig.diffuse), specular(orig.specular), direction(orig.direction), angle(orig.angle), type(orig.type)
{
}

PAG::Light::~Light()
{

}
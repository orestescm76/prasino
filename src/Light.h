#pragma once
namespace PAG
{
	enum class LightType
	{
		AMBIENT,
		POINT,
		DIRECTIONAL,
		SPOTLIGHT
	};

	class Light
	{
	public:
		glm::vec3 position = { 0,0,0 };
		glm::vec3 ambient = { 0,0,0 };
		glm::vec3 diffuse = { 0,0,0 };
		glm::vec3 specular = { 0,0,0 };
		glm::vec3 direction = { 0,0,0 };
		float angle = 0.0f;
		LightType type = LightType::AMBIENT;
		//Ambient constructor
		Light(glm::vec3 amb);
		//Point constructor
		Light(glm::vec3 dif, glm::vec3 spec, glm::vec3 pos);
		//Directional constructor
		Light(glm::vec3 dif, glm::vec3 spec, glm::vec3 pos, glm::vec3 dir);
		~Light();
	};
}
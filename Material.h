#pragma once
#include "pch.h"

namespace PAG
{
	//Using a struct because it's a simple object and I don't think we need too many getters and setters.
	struct Material
	{
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float shininess = 0.0f;

		Material();
		Material(glm::vec3 a, glm::vec3 d, glm::vec3 s, float ss);
		Material(const Material& orig);
		~Material();
	};
}


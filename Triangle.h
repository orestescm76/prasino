#pragma once
#include "Model.h"


namespace PAG
{
	class Triangle : public Model
	{
		GLfloat verticesArr[9] = { -.5, -.5, 0,
					.5, -.5, 0,
					.0,  .5, 0 };
		//indices para pintarlo
		GLuint indicesArr[3] = { 0, 1, 2 };
		//colores de los vertices
		GLfloat colorsArr[9] = { 0.0f, 0.733f, 0.176f,
							0.835f, 0.188f, 0.196f,
							//0.678f, 0.847f, 0.902f };
							0.114f, 0.118f, 0.2f };
	public:
		Triangle();
		Triangle(std::shared_ptr<ShaderProgram>& shaderProgram);
		void initModel();
		~Triangle();
	};
}
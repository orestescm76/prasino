#pragma once
#include "ShaderProgram.h"
namespace PAG
{
	class Model
	{
		GLfloat* vertices;
		GLfloat* colors;
		GLuint* indices;
		//vertices del triangulo
		GLfloat verticesArr[9] = { -.5, -.5, 0,
							.5, -.5, 0,
							.0,  .5, 0 };
		//indices para pintarlo
		GLuint indicesArr[3] = { 0, 1, 2 };
		//colores de los vertices
		GLfloat colorsArr[9] = { 0.0f, 0.733f, 0.176f,
							0.835f, 0.188f, 0.196f,
							0.678f, 0.847f, 0.902f };
		                  //0.114f, 0.118f, 0.2f };
		//IDENTIFICADORES
		GLuint idVAO = 0;
		GLuint idVBO = 0;
		GLuint idColor = 0;
		GLuint idIBO = 0;
		std::shared_ptr<ShaderProgram> sp;
	public:
		Model();
		Model(GLfloat* v, GLfloat* c, GLuint* i, std::shared_ptr<ShaderProgram> shaderProgram);
		Model(std::shared_ptr<ShaderProgram> shaderProgram);
		Model(const Model& model);
		void initModel();
		void draw();
		void erase();
		~Model();
	};
}
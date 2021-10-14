#pragma once
#include "ShaderProgram.h"
namespace PAG
{
	class Model
	{
		GLfloat* vertices;
		GLfloat* colors;
		GLuint* indices;
		//IDENTIFICADORES
		GLuint idVAO = 0;
		GLuint idVBO = 0;
		GLuint idColor = 0;
		GLuint idIBO = 0;
		std::shared_ptr<ShaderProgram> sp;
	public:
		Model();
		Model(GLfloat* v, GLfloat* c, GLuint* i, std::shared_ptr<ShaderProgram> shaderProgram);
		Model(const Model& model);
		void initModel();
		void draw();
		void erase();
		~Model();
	};
}
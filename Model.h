#pragma once
#include "ShaderProgram.h"
namespace PAG
{
	class Model
	{
	protected:
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
		Model(GLfloat* v, GLfloat* c, GLuint* i, std::shared_ptr<ShaderProgram>& shaderProgram);
		Model(std::shared_ptr<ShaderProgram> shaderProgram);
		Model(const Model& model);
		virtual void initModel();
		virtual void draw();
		virtual ~Model();
	};
}
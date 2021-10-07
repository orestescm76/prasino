#pragma once
#include "ShaderProgram.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

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
		ShaderProgram sp;
	public:
		Model();
		Model(GLfloat* v, GLfloat* c, GLuint* i, std::string filevs, std::string filefs);
		void initModel();
		void draw();
		void unDraw();
		~Model();
	};
}
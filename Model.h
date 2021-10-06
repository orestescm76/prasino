#pragma once
#include "ShaderProgram.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace PAG
{
	class Model
	{
		//IDENTIFICADORES
		GLuint idVAO = 0;
		GLuint idVBO = 0;
		GLuint idColor = 0;
		GLuint idIBO = 0;
		ShaderProgram sp;
	public:
		Model();
		~Model();
	};
}
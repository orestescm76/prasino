#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
namespace PAG
{
	class ShaderProgram
	{
		GLint idSP;
		//Shaders of the program
		Shader fragmentShader;
		Shader vertexShader;
	public:
		ShaderProgram();
		~ShaderProgram();
		//Creates the shader from shader data
		void createShader(Shader& shader);
		//Checks and returns the error log messages if there's anything wrong.
		void checkErrors(GLint status, GLint id, std::string msg, bool isShader);
		GLint getIdSP() { return idSP; }
		void useProgram();
	};
}

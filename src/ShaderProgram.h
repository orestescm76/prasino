#pragma once
#include "Shader.h"
namespace PAG
{
	class ShaderProgram
	{
		GLint idSP = 0;
		//Shaders of the program
		Shader fragmentShader;
		Shader vertexShader;
		//Checks and returns the error log messages if there's anything wrong.
		void checkErrors(GLint status, GLint id, std::string msg);
		//Attaches and links the shaders
		void linkShaders();
	public:
		ShaderProgram();
		ShaderProgram(std::string filevs, std::string filefs);
		ShaderProgram(const ShaderProgram& sp);
		~ShaderProgram();
		GLint getIdSP();
		void useProgram();
		void deactivate();
		//Setting up the uniforms is a Shader thing.
		Shader& getVertexShader();
		Shader& getFragmentShader();
	};
}

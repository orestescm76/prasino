#pragma once
namespace PAG
{
	class Shader
	{
		std::string filename = "";
		std::string shaderName = "";
		GLint id = 0;
		GLint idSP = 0;
		GLenum shaderType;
		std::string src = "";
		//Returns the source code from a shader file
		std::string loadShader();
		//Checks and returns the error log messages if there's anything wrong.
		void checkErrors(GLint status, GLint id, std::string msg);
	public:
		Shader();
		Shader(std::string path, std::string name, GLenum type, GLint sp);
		Shader(const Shader& shader);
		~Shader();
		void createShader();
		GLint getId();
		std::string& getSrc();
	};
}


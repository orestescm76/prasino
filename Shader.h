#pragma once
namespace PAG
{
	class Shader
	{
		std::string filename;
		std::string shaderName;
		GLint id;
		GLenum shaderType;
		std::string src;
		//Returns the source code from a shader file
		std::string loadShader();
	public:
		Shader(): id(0), shaderType(0) {}
		Shader(std::string path, std::string name, GLenum type);
		~Shader();
		void createShader()
		{
			id = glCreateShader(shaderType);
		}
		GLint getId() { return id; }
		std::string& getSrc() { return src; }
		std::string getName() { return shaderName; }
	};
}


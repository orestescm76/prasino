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
		GLint getUniformLocation(std::string uniform) const;
		void createShader();
		//Returns the Shader's source code
		std::string& getSrc();
		
	public:
		Shader();
		Shader(std::string path, std::string name, GLenum type, GLint sp);
		Shader(const Shader& shader);
		~Shader();
		Shader& operator=(const Shader& orig) = default;
		//Returns the Shader's ID
		GLint getId();
		//Uniform set
		void setUniform(std::string uniform, glm::vec3 vec) const;
		void setUniform(std::string uniform, glm::mat4 mat) const;
		void setUniform(std::string uniform, float num) const;
		void setUniform(std::string uniform, unsigned int num) const;
		void setUniformSubroutine(std::string uniform, std::string func) const;
		void setUniformSubroutine(GLuint* param, GLsizei numParams) const;
	};
}


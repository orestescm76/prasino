#include "pch.h"
#include "Log.h"
#include "ShaderProgram.h"

PAG::ShaderProgram::ShaderProgram()
{

}

PAG::ShaderProgram::ShaderProgram(std::string filevs, std::string filefs)
{
	//Intentamos cargar los shaders
	try
	{
		idSP = glCreateProgram();
		vertexShader = Shader(filevs, "Vertex shader", GL_VERTEX_SHADER, idSP);
 		fragmentShader = Shader(filefs, "Fragment shader", GL_FRAGMENT_SHADER, idSP);
		//Todo bien, los creamos
		linkShaders();
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error("ShaderProgram::ShaderProgram -> " + (std::string)e.what());
	}
}
PAG::ShaderProgram::ShaderProgram(const ShaderProgram& sp)
{
	try
	{
		idSP = glCreateProgram();
		vertexShader = Shader(sp.vertexShader);
		fragmentShader = Shader(sp.fragmentShader);
		//Todo bien, los creamos
		linkShaders();
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error("ShaderProgram::ShaderProgram(const ShaderProgram& sp) -> " + (std::string)e.what());
	}
}

PAG::ShaderProgram::~ShaderProgram()
{
	if(idSP != 0)
		glDeleteProgram(idSP);
}

void PAG::ShaderProgram::linkShaders()
{
	GLint status = 0;
	try
	{
		//Enlazar shader (link)
		Log::getInstance()->printMessage(msgType::INFO, "Linking shaders");
		glLinkProgram(idSP);
		glGetProgramiv(idSP, GL_LINK_STATUS, &status);
		checkErrors(status, idSP, "Couldn't link the shaders");
	}
	catch (const std::runtime_error& e)
	{
		throw std::runtime_error("ShaderProgram::linkShaders -> "+ (std::string)e.what());
	}

}

void PAG::ShaderProgram::checkErrors(GLint status, GLint id, std::string msg)
{
	if (status == GL_FALSE)
	{
		Log::getInstance()->printMessage(msgType::ERROR, msg);
		char log[1024];
		GLsizei buff;
		glGetProgramInfoLog(id, sizeof(log), &buff, log);
		std::string logs(log);
		throw std::runtime_error("ShaderProgram::checkErrors -> " + logs);
	}
}

GLint PAG::ShaderProgram::getIdSP()
{
	return idSP;
}

void PAG::ShaderProgram::useProgram()
{
	glUseProgram(idSP);
}

void PAG::ShaderProgram::deactivate()
{
	glUseProgram(0);
}

const PAG::Shader& PAG::ShaderProgram::getVertexShader()
{
	return vertexShader;
}

const PAG::Shader& PAG::ShaderProgram::getFragmentShader()
{
	return fragmentShader;
}



#include "pch.h"
#include "Log.h"
#include "ShaderProgram.h"

PAG::ShaderProgram::ShaderProgram()
{

}

PAG::ShaderProgram::ShaderProgram(std::string filevs, std::string filefs) try :
	idSP(glCreateProgram()),
	//Intentamos cargar los shaders
	vertexShader(filevs, "Vertex shader", GL_VERTEX_SHADER, idSP),
	fragmentShader(filefs, "Fragment shader", GL_FRAGMENT_SHADER, idSP)
{
	//Todo bien, los creamos
	linkShaders();
	vertexShader.setSPID(idSP);
	fragmentShader.setSPID(idSP);
	fragmentShader.createSubroutineCache();
	linkShaders();

}
catch (const std::exception& e)
{
	throw std::runtime_error("ShaderProgram::ShaderProgram -> " + (std::string)e.what());
}

PAG::ShaderProgram::ShaderProgram(const ShaderProgram& sp) try :
	//Intentamos cargar los shaders
	vertexShader(sp.vertexShader),
	fragmentShader(sp.fragmentShader)
{
	idSP = glCreateProgram();
	//Todo bien, los creamos
	vertexShader.setSPID(idSP);
	fragmentShader.setSPID(idSP);
	fragmentShader.createSubroutineCache();
	linkShaders();
}
catch (const std::exception& e)
{
	throw std::runtime_error("ShaderProgram::ShaderProgram -> " + (std::string)e.what());
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

PAG::Shader& PAG::ShaderProgram::getVertexShader()
{
	return vertexShader;
}

PAG::Shader& PAG::ShaderProgram::getFragmentShader()
{
	return fragmentShader;
}



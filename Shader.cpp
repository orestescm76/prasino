#include "Shader.h"
#include "Log.h"
#include "Renderer.h"
#include <fstream>
#include <sstream>

PAG::Shader::Shader(std::string path, std::string name, GLenum type) : filename(path), shaderName(name), id(0), shaderType(type)
{
	src = "";
	try
	{
		src = loadShader();
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error("Shader::Shader -> " + (std::string)e.what()); //object shouldn't finish creating....
	}
}

PAG::Shader::~Shader()
{
	glDeleteShader(id);
}

std::string PAG::Shader::loadShader()
{
	Log::getInstance()->printMessage(msgType::INFO, "Opening " + filename);
	std::ifstream input;
	input.open(filename, std::ifstream::in);
	if (!input.is_open())
	{
		throw std::invalid_argument("Shader::loadShader() -> Cannot open " + filename);
	}
	std::stringstream ssVs;
	ssVs << input.rdbuf();
	input.close();
	Log::getInstance()->printMessage(msgType::OK, "Read ok");
	return ssVs.str();
}
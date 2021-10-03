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
		Log::getInstance()->printMessage(msgType::WARNING, e.what());
		throw; //object shouldn't finish creating....
	}
}

std::string PAG::Shader::loadShader()
{
	Log::getInstance()->printMessage(msgType::INFO, "Opening " + filename);
	std::ifstream input;
	input.open(filename, std::ifstream::in);
	if (!input.is_open())
	{
		throw std::invalid_argument("Cannot open " + filename);
	}
	std::stringstream ssVs;
	ssVs << input.rdbuf();
	input.close();
	Log::getInstance()->printMessage(msgType::OK, "Read ok");
	return ssVs.str();
}
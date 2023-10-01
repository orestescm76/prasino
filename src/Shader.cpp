#include "pch.h"
#include "Shader.h"
#include "Log.h"
#include "Renderer.h"

PAG::Shader::Shader() : shaderType(0) 
{

}

PAG::Shader::Shader(std::string path, std::string name, GLenum type, GLint sp) : filename(path), shaderName(name), id(0), shaderType(type), idSP(sp), uniformLocation(), numSubroutines(0), subroutineIndex(nullptr)
{
	src = "";
	try
	{
		src = loadShader();
		createShader();

	}
	catch (const std::exception& e)
	{
		throw std::runtime_error("Shader::Shader -> " + (std::string)e.what()); //object shouldn't finish creating....
	}
}

PAG::Shader::Shader(const Shader& shader): src(shader.src), idSP(shader.idSP), shaderType(shader.shaderType), shaderName(shader.shaderName)
{
	try
	{
		createShader();
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error("Shader::Shader(const Shader& shader) -> " + (std::string)e.what()); //object shouldn't finish creating....
	}
}

PAG::Shader::~Shader()
{
	if(id != 0)
		glDeleteShader(id);
	delete[] subroutineIndex;
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

void PAG::Shader::createShader()
{
	GLint status = 0;
	//Crear shader
	Log::getInstance()->printMessage(msgType::INFO, "Creating shader");
	try
	{
		id = glCreateShader(shaderType);
		//Lectura de shaders
		const GLchar* shaderSource = src.c_str();
		glShaderSource(id, 1, &shaderSource, nullptr);
		//Compilar shaders
		Log::getInstance()->printMessage(msgType::INFO, "Compiling shader");
		glCompileShader(id);
		glGetShaderiv(id, GL_COMPILE_STATUS, &status);
		checkErrors(status, id, shaderName + " wasn't compiled");
		//Attach shaders
		Log::getInstance()->printMessage(msgType::INFO, "Attaching shaders");
		glAttachShader(idSP, id);

	}
	catch (const std::exception& e)
	{
		throw std::runtime_error("Shader::createShader -> " + (std::string)e.what());
	}
}

GLint PAG::Shader::getId()
{
	return id;
}

void PAG::Shader::createSubroutineCache()
{
	Log::getInstance()->printMessage(msgType::INFO, "Creating cache");
	glGetProgramStageiv(idSP, shaderType, GL_ACTIVE_SUBROUTINE_UNIFORMS, &numSubroutines);
	if (numSubroutines != 0)
		subroutineIndex = new GLuint[numSubroutines];
	else
		subroutineIndex = nullptr;
}

std::string& PAG::Shader::getSrc() 
{
	return src;
}

void PAG::Shader::setUniform(std::string uniform, glm::vec3 vec) 
{
	glUniform3fv(getUniformLocation(uniform), 1, &vec[0]);
}

void PAG::Shader::setUniform(std::string uniform, glm::mat4 mat) 
{
	glUniformMatrix4fv(getUniformLocation(uniform), 1, GL_FALSE, &mat[0][0]);
}

void PAG::Shader::setUniform(std::string uniform, float num) 
{
	glUniform1f(getUniformLocation(uniform), num);
}
void PAG::Shader::setUniform(std::string uniform, unsigned int num) 
{
	glUniform1i(getUniformLocation(uniform), num);
}

void PAG::Shader::setUniformSubroutine(std::string uniform, std::string func) 
{
	//Check the uniform name
	if (uniformLocation.find(uniform) == uniformLocation.end())
	{
		GLuint idFunc = glGetSubroutineIndex(idSP, shaderType, uniform.c_str());
		assert(idFunc != GL_INVALID_INDEX);
		uniformLocation[uniform] = idFunc;
	}
	//Then check the selected subroutine
	if (uniformLocation.find(func) == uniformLocation.end())
	{
		GLuint idSub = glGetSubroutineUniformLocation(idSP, shaderType, func.c_str());
		assert(idSub != GL_INVALID_INDEX);
		uniformLocation[func] = idSub;
	}
	subroutineIndex[uniformLocation[func]] = uniformLocation[uniform];
}

void PAG::Shader::setSubroutines() 
{
	glUniformSubroutinesuiv(shaderType, numSubroutines, subroutineIndex);
}

void PAG::Shader::setSPID(GLint id)
{
	shaderProgramID = id;
}

void PAG::Shader::checkErrors(GLint status, GLint shaderId, std::string msg)
{
	if (status == GL_FALSE)
	{
		Log::getInstance()->printMessage(msgType::LOG_ERROR, msg);
		char log[1024];
		GLsizei buff;
		glGetShaderInfoLog(shaderId, sizeof(log), &buff, log);
		std::string logs(log);
		throw std::runtime_error("ShaderProgram::checkErrors -> " + logs);
	}
}

GLint PAG::Shader::getUniformLocation(std::string uniform)
{	
	if (uniformLocation.find(uniform) == uniformLocation.end())
	{
		GLint loc = glGetUniformLocation(idSP, uniform.c_str());
		assert(loc != GL_INVALID_INDEX);
		uniformLocation[uniform] = loc;
	}
	return uniformLocation[uniform];
	//if (loc != GL_INVALID_INDEX)
	//	return loc;
	//Log::getInstance()->printMessage(msgType::WARNING, "Shader::getUniformLocation()->Couldn't find the uniform location "+uniform);
}

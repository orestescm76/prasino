#include "Log.h"
#include "ShaderProgram.h"

PAG::ShaderProgram::ShaderProgram() : idSP(0)
{
	//Intentamos cargar los shaders
	try
	{
		vertexShader = Shader("pag03-vs.glsl", "Vertex shader", GL_VERTEX_SHADER);
		fragmentShader = Shader("pag03-fs.glsl", "Fragment shader", GL_FRAGMENT_SHADER);
		//Todo bien, los creamos
		idSP = glCreateProgram();
		createShader(vertexShader);
		createShader(fragmentShader);
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error("ShaderProgram::ShaderProgram -> " + (std::string)e.what());
	}
}
PAG::ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(idSP);
}

void PAG::ShaderProgram::createShader(Shader& shader)
{
	GLint statusfs = 0;
	GLint statusvs = 0;
	GLint status = 0;

	//Crear shader
	Log::getInstance()->printMessage(msgType::INFO, "Creating shader");
	shader.createShader();
	GLint idShader = shader.getId();
	try
	{
		//Lectura de shaders
		const GLchar* src = shader.getSrc().c_str();
		glShaderSource(idShader, 1, &src, nullptr);
		//Compilar shaders
		Log::getInstance()->printMessage(msgType::INFO, "Compiling shader");
		glCompileShader(idShader);
		glGetShaderiv(idShader, GL_COMPILE_STATUS, &statusvs);
		checkErrors(statusvs, idShader, shader.getName() + " wasn't compiled", true);
		//Attach shaders
		Log::getInstance()->printMessage(msgType::INFO, "Attaching shader");
		glAttachShader(idSP, idShader);
		//Enlazar shader (link)
		Log::getInstance()->printMessage(msgType::INFO, "Linking shader");
		glLinkProgram(idSP);
		glGetProgramiv(idSP, GL_LINK_STATUS, &status);
		checkErrors(status, idSP, "Couldn't link the shaders", false);
	}
	catch (const std::runtime_error& e)
	{
		throw std::runtime_error("ShaderProgram::createShader -> "+ (std::string)e.what());
	}

}

void PAG::ShaderProgram::checkErrors(GLint status, GLint id, std::string msg, bool isShader)
{
	if (status == GL_FALSE)
	{
		Log::getInstance()->printMessage(msgType::ERROR, msg);
		char log[1024];
		GLsizei buff;
		if (isShader)
			glGetShaderInfoLog(id, sizeof(log), &buff, log);
		else
			glGetProgramInfoLog(id, sizeof(log), &buff, log);
		std::string logs(log);
		throw std::runtime_error("ShaderProgram::checkErrors -> " + logs);
	}
}

void PAG::ShaderProgram::useProgram()
{
	glUseProgram(idSP);
}


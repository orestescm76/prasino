/*
* @author orestescm76
* @brief renderer cpp
*/

#include <iostream>
#include "Renderer.h"
#include "Log.h"

PAG::Renderer* PAG::Renderer::instancia = nullptr;
const std::string PAG::Renderer::version = "0.4.1";

PAG::Renderer::Renderer(): sp()
{
	createModel();
	r = .6f;
	g = .6f;
	b = .6f;
	a = 1.0f;
}

PAG::Renderer::~Renderer()
{
	glDeleteVertexArrays(1, &idVBO);
	glDeleteVertexArrays(1, &idVBOColor);
	glDeleteVertexArrays(1, &idIBO);
	glDeleteVertexArrays(1, &idVAO);
}

PAG::Renderer* PAG::Renderer::getInstance()
{
	if (!instancia)
	{
		try
		{
			instancia = new Renderer();
		}
		catch (const std::exception& e)
		{
			throw std::runtime_error("Renderer::getInstance() -> " + (std::string)e.what());
		}
	}
	return instancia;
}

void PAG::Renderer::refreshWindow()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	sp.useProgram();
	glBindVertexArray(idVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
}

void PAG::Renderer::changeColor(double yoffset)
{
	if (yoffset > 0)
		addColor();
	else minusColor();
	configBackColor(r, g, b, a);
}

void PAG::Renderer::start()
{
	configBackColor(r,g,b,a);
	activeZBuffer();
	//Polilínea, sólidos de revolución
	//glPrimitiveRestartIndex(0xFFFF);
	//glEnable(GL_PRIMITIVE_RESTART);
	
	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_MULTISAMPLE);
}

void PAG::Renderer::activeZBuffer()
{
	// - Le decimos a OpenGL que tenga en cuenta la profundidad a la hora de dibujar.
	//   No tiene por qué ejecutarse en cada paso por el ciclo de eventos.
	glEnable(GL_DEPTH_TEST);
}

void PAG::Renderer::addColor()
{
	r += .05f;
	g += .05f;
	b += .05f;
	if (r >= 1.0f)
		r = 1.0f;
	if (g >= 1.0f)
		g = 1.0f;
	if (b >= 1.0f)
		b = 1.0f;
}

void PAG::Renderer::minusColor()
{
	r -= .05f;
	g -= .05f;
	b -= .05f;
	if (r <= 0.0f)
		r = 0.0f;
	if (g <= 0.0f)
		g = 0.0f;
	if (b <= 0.0f)
		b = 0.0f;
}

void PAG::Renderer::configBackColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	glClearColor(r, g, b, a);
}

void PAG::Renderer::printInfo()
{
	std::string renderer((const char*)glGetString(GL_RENDERER));
	std::string vendor = (const char*)glGetString(GL_VENDOR);
	std::string version = (const char*)glGetString(GL_VERSION);
	std::string shadingVersion = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
	Log::getInstance()->printMessage(PAG::msgType::INFO, "RENDERER: " + renderer);
	Log::getInstance()->printMessage(PAG::msgType::INFO, "VENDOR: " + vendor);
	Log::getInstance()->printMessage(PAG::msgType::INFO, "VERSION: " + version);
	Log::getInstance()->printMessage(PAG::msgType::INFO, "SHADING LANGUAGE VERSION: " + shadingVersion);
}

void PAG::Renderer::createModel()
{
	//vertices del triangulo
	GLfloat vertices[] = { -.5, -.5, 0,
						.5, -.5, 0,
						.0,  .5, 0 };
	//indices para pintarlo
	GLuint indices[] = { 0, 1, 2 };
	//colores de los vertices
	GLfloat colores[] = { 0.0, 0.733, 0.176, 
						0.835, 0.188, 0.196,
						0.114, 0.118, 0.2 };
	/*
	GLfloat vboentrelazado[] = {-.5, -.5, 0, 0.0, 0.733, 0.176, 
								 .5, -.5, 0, 0.835, 0.188, 0.196, 
								 .0,  .5, 0, 0.114, 0.118, 0.2 };
	*/
	//generamos el vao y la vinculamos
	Log::getInstance()->printMessage(msgType::INFO, "Creating VAO");
	glGenVertexArrays(1, &idVAO);
	glBindVertexArray(idVAO);
	//SIN ENTRELAZAR
	Log::getInstance()->printMessage(msgType::INFO, "Creating VBO");
	glGenBuffers(1, &idVBO);
	glBindBuffer(GL_ARRAY_BUFFER, idVBO);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), vertices,
		GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
		nullptr);
	glEnableVertexAttribArray(0);
	//generamos el VBO de los colores, pongo 1 para luego especificarlo en el layout del shader	
	glGenBuffers(1, &idVBOColor);
	glBindBuffer(GL_ARRAY_BUFFER, idVBOColor);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colores, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), nullptr);
	glEnableVertexAttribArray(1);
	//ENTRELAZADO
	//glGenBuffers(1, &idVBO);
	//glBindBuffer(GL_ARRAY_BUFFER, idVBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vboentrelazado), vboentrelazado, GL_STATIC_DRAW);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), nullptr);
	//glEnableVertexAttribArray(0);

	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLfloat*)NULL+3);
	//glEnableVertexAttribArray(1);
	//indices
	Log::getInstance()->printMessage(msgType::INFO, "Creating IBO");
	glGenBuffers(1, &idIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(GLuint), indices,
		GL_STATIC_DRAW);
}

void PAG::Renderer::configViewport(int width, int height)
{
	glViewport(0, 0, width, height);
}
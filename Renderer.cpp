/*
* @author orestescm76
* @brief renderer cpp
*/

#include "pch.h"
#include "Renderer.h"

PAG::Renderer* PAG::Renderer::instance = nullptr;
const std::string PAG::Renderer::version = "0.5.1";

PAG::Renderer::Renderer()
{
	//vertices del triangulo
	GLfloat vertices[] = {-.5, -.5, 0,
						.5, -.5, 0,
						.0,  .5, 0 };
	//indices para pintarlo
	GLuint indices[] = { 0, 1, 2 };
	//colores de los vertices
	GLfloat colores[] = {0.0f, 0.733f, 0.176f,
						0.835f, 0.188f, 0.196f,
						0.114f, 0.118f, 0.2f };
	try
	{
		triangle = new Model(vertices, colores, indices, "pag03-vs.glsl", "pag03-fs.glsl");
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error("PAG::Renderer::Renderer -> " + (std::string)e.what());
	}
	r = .6f;
	g = .6f;
	b = .6f;
	a = 1.0f;
}


PAG::Renderer::~Renderer()
{
	delete triangle;
}

PAG::Renderer* PAG::Renderer::getInstance()
{
	if (!instance)
	{
		try
		{
			instance = new Renderer();
		}
		catch (const std::exception& e)
		{
			throw std::runtime_error("Renderer::getInstance() -> " + (std::string)e.what());
		}
	}
	return instance;
}

void PAG::Renderer::refreshWindow()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (drawing)
		draw();
	else
		erase();
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

void PAG::Renderer::configBackColor(GLfloat _r, GLfloat _g, GLfloat _b, GLfloat _a)
{
	glClearColor(_r, _g, _b, _a);
}

void PAG::Renderer::printInfo()
{
	std::string renderer((const char*)glGetString(GL_RENDERER));
	std::string vendor = (const char*)glGetString(GL_VENDOR);
	std::string openglversion = (const char*)glGetString(GL_VERSION);
	std::string shadingVersion = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
	Log::getInstance()->printMessage(PAG::msgType::INFO, "RENDERER: " + renderer);
	Log::getInstance()->printMessage(PAG::msgType::INFO, "VENDOR: " + vendor);
	Log::getInstance()->printMessage(PAG::msgType::INFO, "VERSION: " + openglversion);
	Log::getInstance()->printMessage(PAG::msgType::INFO, "SHADING LANGUAGE VERSION: " + shadingVersion);
}

void PAG::Renderer::draw()
{
	drawing = true;
	triangle->draw();
}

void PAG::Renderer::erase()
{
	drawing = false;
	triangle->unDraw();
}


void PAG::Renderer::configViewport(int width, int height)
{
	glViewport(0, 0, width, height);
}
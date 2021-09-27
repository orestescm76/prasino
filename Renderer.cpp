/*
* @author orestescm76
* @brief renderer cpp
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include "Renderer.h"

PAG::Renderer* PAG::Renderer::instancia = nullptr;

const char* PAG::Renderer::version = "0.3.3";
PAG::Renderer::Renderer()
{
	crearShader("pag03-vs.glsl", "pag03-fs.glsl");
	crearModelo();
	r = .6f;
	g = .6f;
	b = .6f;
	a = 1.0f;
}

PAG::Renderer::~Renderer()
{

}

PAG::Renderer* PAG::Renderer::getInstancia()
{
	if (!instancia)
		instancia = new Renderer();
	return instancia;
}

void PAG::Renderer::refrescar_ventana()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glUseProgram(idSP);
	glBindVertexArray(idVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
}

void PAG::Renderer::cambiar_color(double yoffset)
{
	if (yoffset > 0)
		sumar_color();
	else restar_color();
	configurar_color(r, g, b, a);
}

void PAG::Renderer::inicializar()
{
	configurar_color(r,g,b,a);
	activarZBuffer();
	glEnable(GL_MULTISAMPLE);
}

void PAG::Renderer::activarZBuffer()
{
	// - Le decimos a OpenGL que tenga en cuenta la profundidad a la hora de dibujar.
	//   No tiene por qué ejecutarse en cada paso por el ciclo de eventos.
	glEnable(GL_DEPTH_TEST);
}

void PAG::Renderer::sumar_color()
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

void PAG::Renderer::restar_color()
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

void PAG::Renderer::configurar_color(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	glClearColor(r, g, b, a);
}

void PAG::Renderer::imprimirInformacion()
{
	// - Información de OpenGL
	std::cout << "RENDERER: " << glGetString(GL_RENDERER) << std::endl
		<< "VENDOR: " << glGetString(GL_VENDOR) << std::endl
		<< "VERSION: " << glGetString(GL_VERSION) << std::endl
		<< "SHADING LANGUAGE VERSION: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

void PAG::Renderer::crearModelo()
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

	GLfloat vboentrelazado[] = {-.5, -.5, 0, 0.0, 0.733, 0.176, 
								 .5, -.5, 0, 0.835, 0.188, 0.196, 
								 .0,  .5, 0, 0.114, 0.118, 0.2 };
	//generamos el vao y la vinculamos
	glGenVertexArrays(1, &idVAO);
	glBindVertexArray(idVAO);
	//SIN ENTRELAZAR
	glGenBuffers(1, &idVBO);
	glBindBuffer(GL_ARRAY_BUFFER, idVBO);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), vertices,
		GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
		nullptr);
	glEnableVertexAttribArray(0);
	//generamos el VBO de los colores, con ID 1 para luego especificarlo en el shader
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

	//glGenBuffers(1, &idVBOColor);
	//glBindBuffer(GL_ARRAY_BUFFER, idVBOColor);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vboentrelazado), vboentrelazado, GL_STATIC_DRAW);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLfloat*)NULL+3);
	//glEnableVertexAttribArray(1);
	//indices
	glGenBuffers(1, &idIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(GLuint), indices,
		GL_STATIC_DRAW);
}

void PAG::Renderer::configurar_viewport(int width, int height)
{
	glViewport(0, 0, width, height);
}

void PAG::Renderer::crearShader(std::string vsfile, std::string fsfile)
{
	std::string miVertexShader = "";
	std::string miFragmentShader = "";
	try
	{
		miVertexShader = loadShader(vsfile);
		miFragmentShader = loadShader(fsfile);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return;
	}
	idVS = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* fuenteVS = miVertexShader.c_str();
	glShaderSource(idVS, 1, &fuenteVS, nullptr);
	glCompileShader(idVS);
	idFS = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fuenteFS = miFragmentShader.c_str();
	glShaderSource(idFS, 1, &fuenteFS, nullptr);
	glCompileShader(idFS);
	idSP = glCreateProgram();
	glAttachShader(idSP, idVS);
	glAttachShader(idSP, idFS);
	glLinkProgram(idSP);
}
std::string PAG::Renderer::loadShader(std::string file)
{
	std::ifstream input;
	input.open(file, std::ifstream::in);
	if (!input.is_open())
	{
		throw std::invalid_argument("Cannot open " + file);
	}
	std::stringstream ssVs;
	ssVs << input.rdbuf();
	return ssVs.str();
}
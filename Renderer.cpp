/*
* @author orestescm76
* @brief renderer cpp
*/

#include <iostream>
#include "Renderer.h"

PAG::Renderer* PAG::Renderer::instancia = nullptr;

const char* PAG::Renderer::version = "0.3.1";
PAG::Renderer::Renderer()
{
	crearShader("", "");
	crearModelo();
	r = .6;
	g = .6;
	b = .6;
	a = 1.0;
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
	inicializar();
}

void PAG::Renderer::inicializar()
{
	// - Le decimos a OpenGL que tenga en cuenta la profundidad a la hora de dibujar.
	//   No tiene por qué ejecutarse en cada paso por el ciclo de eventos.
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
}

void PAG::Renderer::sumar_color()
{
	r += .05;
	g += .05;
	b += .05;
	if (r >= 1.0)
		r = 1.0;
	if (g >= 1.0)
		g = 1.0;
	if (b >= 1.0)
		b = 1.0;
}

void PAG::Renderer::restar_color()
{
	r -= .05;
	g -= .05;
	b -= .05;
	if (r <= 0.0)
		r = 0.0;
	if (g <= 0.0)
		g = 0.0;
	if (b <= 0.0)
		b = 0.0;
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
	GLfloat vertices[] = { -.5, -.5, 0,
						.5, -.5, 0,
						.0,  .5, 0 };
	GLuint indices[] = { 0, 1, 2 };
	glGenVertexArrays(1, &idVAO);
	glBindVertexArray(idVAO);
	glGenBuffers(1, &idVBO);
	glBindBuffer(GL_ARRAY_BUFFER, idVBO);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), vertices,
		GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
		nullptr);
	glEnableVertexAttribArray(0);
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
	//TEMP
	std::string miVertexShader =
		"#version 410\n"
		"layout (location = 0) in vec3 posicion;\n"
		"void main ()\n"
		"{\n"
		"   gl_Position = vec4 ( posicion, 1 );\n"
		"}\n";
	std::string miFragmentShader =
		"#version 410\n"
		"out vec4 colorFragmento;\n"
		"void main ()\n"
		"{\n"
		"   colorFragmento = vec4 ( 1.0, .4, .2, 1.0 );\n"
		"}\n";
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

void PAG::Renderer::configurar_color()
{
	glClearColor(r, g, b, a);
}
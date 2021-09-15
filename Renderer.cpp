/*
* @author orestescm76
* @brief renderer cpp
*/

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Renderer.h"

PAG::Renderer* PAG::Renderer::instancia = nullptr;
double PAG::Renderer::r = .6;
double PAG::Renderer::g = .6;
double PAG::Renderer::b = .6;
double PAG::Renderer::a = 1.0;

PAG::Renderer::Renderer()
{

}

PAG::Renderer::~Renderer()
{
	delete instancia;
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
}

void PAG::Renderer::cambiar_color(double yoffset)
{
	if (yoffset > 0)
		sumar_color();
	else restar_color();
	configurar_color();
}

void PAG::Renderer::configurar_color()
{
	glClearColor(r, g, b, a);
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

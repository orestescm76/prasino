#pragma once
#include "ShaderProgram.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

namespace PAG
{
	class Renderer
	{
	private:
		static Renderer* instancia;
		GLfloat r, g, b, a;
		//IDENTIFICADORES
		GLuint idVAO = 0;
		GLuint idVBO = 0;
		GLuint idVBOColor = 0;
		GLuint idIBO = 0;
		Renderer();
		virtual ~Renderer();
		void configBackColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
		//void createShaders();
		ShaderProgram sp;
	public:
		static const std::string version;
		// Lo único que necesito estático de la clase Renderer es la instancia y el método de la instancia
		// ya que así o creo una instancia y hago render->getInstancia() o PAG::Renderer::getInstancia(), lo mismo da.
		static Renderer* getInstance(); 
		void refreshWindow();
		void changeColor(double yoffset);
		void start();
		void activeZBuffer();
		void addColor();
		void minusColor();
		void configViewport(int width, int height);
		void createModel();
		void printInfo();
	};
}

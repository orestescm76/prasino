#pragma once
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
		GLuint idVS = 0;
		GLuint idFS = 0;
		GLuint idSP = 0;
		GLuint idVAO = 0;
		GLuint idVBO = 0;
		GLuint idVBOColor = 0;
		GLuint idIBO = 0;
		Renderer();
		void configurar_color(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	public:
		static const char* version;
		virtual ~Renderer();
		// Lo único que necesito estático de la clase Renderer es la instancia y el método de la instancia
		// ya que así o creo una instancia y hago render->getInstancia() o PAG::Renderer::getInstancia(), lo mismo da.
		static Renderer* getInstancia(); 
		void refrescar_ventana();
		void cambiar_color(double yoffset);
		void inicializar();
		void activarZBuffer();
		void sumar_color();
		void restar_color();
		void configurar_viewport(int width, int height);
		void crearModelo();
		void crearShader(std::string vsfile, std::string fsfile);
		std::string loadShader(std::string file);
		void comprobarErrores(GLint status, GLint shader, std::string msg, bool isShader);
		void imprimirInformacion();
	};
}

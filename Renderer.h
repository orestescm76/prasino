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
		double r, g, b, a;
		//IDENTIFICADORES
		GLuint idVS = 0;
		GLuint idFS = 0;
		GLuint idSP = 0;
		GLuint idVAO = 0;
		GLuint idVBO = 0;
		GLuint idIBO = 0;
		Renderer();
	public:
		static const char* version;
		virtual ~Renderer();
		// Lo �nico que necesito est�tico de la clase Renderer es la instancia y el m�todo de la instancia
		// ya que as� o creo una instancia y hago render->getInstancia() o PAG::Renderer::getInstancia(), lo mismo da.
		static Renderer* getInstancia(); 
		void refrescar_ventana();
		void cambiar_color(double yoffset);
		void configurar_color();
		void inicializar();
		void sumar_color();
		void restar_color();
		void configurar_viewport(int width, int height);
		void crearModelo();
		void crearShader(std::string vsfile, std::string fsfile);
		void imprimirInformacion();
	};
}

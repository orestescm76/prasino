#pragma once
#include "Model.h"
#include "Log.h"
namespace PAG
{
	class Renderer
	{
	private:
		static Renderer* instance;
		GLfloat r, g, b, a;
		Model* triangle;
		Renderer();
		void configBackColor(GLfloat _r, GLfloat _g, GLfloat _b, GLfloat _a);
		bool drawing = false;
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
		void printInfo();
		void draw();
		void erase();
		bool isDrawing() { return drawing; }
		virtual ~Renderer();
	};
}

#pragma once

namespace PAG
{
	class Renderer
	{
	private:
		static Renderer* instancia;
		double r, g, b, a;
		Renderer();
	public:
		virtual ~Renderer();
		// Lo único que necesito estático de la clase Renderer es la instancia y el método de la instancia
		// ya que así o creo una instancia y hago render->getInstancia() o PAG::Renderer::getInstancia(), lo mismo da.
		static Renderer* getInstancia(); 
		void refrescar_ventana();
		void cambiar_color(double yoffset);
		void configurar_color();
		void sumar_color();
		void restar_color();
		void imprimirInformacion();
	};
}

#pragma once

namespace PAG
{
	class Renderer
	{
	private:
		static Renderer* instancia;
		static double r, g, b, a;
		Renderer();
	public:
		virtual ~Renderer();
		static Renderer* getInstancia();
		static void refrescar_ventana();
		static void cambiar_color(double yoffset);
		static void configurar_color();
		static void sumar_color();
		static void restar_color();
		void imprimirInformacion();
	};
}

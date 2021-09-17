/*
* @author orestescm76
* @brief main
* VERSION 0.2.1
*/

#include <iostream>
#include "Renderer.h"
//en este orden que si no la jodemos
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//callback de redibujar
void window_refresh_callback(GLFWwindow* window)
{
	PAG::Renderer::getInstancia()->refrescar_ventana();
	glfwSwapBuffers(window);
	std::cout << "Ventana redibujada" << std::endl;
}
//callback al redimensionar ventana
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	std::cout << "Ventana redimensionada" << std::endl;
}
//callback al pulsar una tecla
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	std::cout << "Key callback called" << std::endl;
}
//callback de botones del ratón
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		std::cout << "Se ha pulsado " << button << std::endl;
	}
	else if (action == GLFW_RELEASE)
	{
		std::cout << "Se ha soltado " << button << std::endl;
	}
}
//callback rueda ratón
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	std::cout << "Movida la rueda del raton " << xoffset
		<< " Unidades en horizontal y " << yoffset << " unidades en vertical"
		<< std::endl;
	PAG::Renderer::getInstancia()->cambiar_color(yoffset);
	window_refresh_callback(window);
}

int main()
{
	PAG::Renderer* render = PAG::Renderer::getInstancia();
	std::cout << "Iniciando PAG2122 " << render->version << std::endl;
	// - Inicializa GLFW. Es un proceso que sólo debe realizarse una vez en la aplicación
	if (glfwInit() != GLFW_TRUE)
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
	}
	// - Definimos las características que queremos que tenga el contexto gráfico
	//   OpenGL de la ventana que vamos a crear. Por ejemplo, el número de muestras o el
	//   modo Core Profile.
	glfwWindowHint(GLFW_SAMPLES, 4); // - Activa antialising con 4 muestras
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // - Esta y las 2
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // siguientes activan un contexto
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1); // OpenGL Core Profile 4.1
	// - Definimos el puntero para guardar la dirección de la ventana de la aplicación y
    //   la creamos
	GLFWwindow* window;
	// - Tamaño, título de la ventana, en ventana y no en pantalla completa,
    //   sin compartir recursos con otras ventanas.
	window = glfwCreateWindow(1024, 576, "PAG2122-[ColominaMonsalve-Orestes]", nullptr, nullptr);
	//¿salió bien?
	if (!window)
	{
		std::cout << "Failed to open GLFW window" << std::endl;
		glfwTerminate();
		return -2;
	}
	// - Hace que el contexto OpenGL asociado a la ventana que acabamos de crear pase a
    //   ser el contexto actual de OpenGL para las siguientes llamadas a la biblioteca
	glfwMakeContextCurrent(window);
	// - Ahora inicializamos GLEW.
    //   IMPORTANTE: GLEW debe inicializarse siempre DESPUÉS de que se haya
    //               inicializado GLFW y creado la ventana
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return -3;
	}
	render->imprimirInformacion();
	//registrar callbacks
	glfwSetWindowRefreshCallback(window, window_refresh_callback);	
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);
	
	//establecer un gris
	render->configurar_color();
	// - Le decimos a OpenGL que tenga en cuenta la profundidad a la hora de dibujar.
    //   No tiene por qué ejecutarse en cada paso por el ciclo de eventos.
	glEnable(GL_DEPTH_TEST);
	//Ciclo de eventos
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	window = nullptr;
	glfwTerminate();
	std::cout << "PAR2122 is finished!" << std::endl;
	std::cin.get();
}

/*
* @author orestescm76
* @brief main
* VERSION 0.4.0
*/

#include <iostream>
#include "Renderer.h"
#include "Log.h"
//en este orden que si no la jodemos
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//callback de redibujar
void window_refresh_callback(GLFWwindow* window)
{
	PAG::Renderer::getInstance()->refreshWindow();
	glfwSwapBuffers(window);
	//std::cout << "Ventana redibujada" << std::endl;
}
//callback al redimensionar ventana
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	PAG::Renderer::getInstance()->configViewport(width, height);
	//std::cout << "Ventana redimensionada: " << width << "x" << height << std::endl;
}
//callback al pulsar una tecla
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	//std::cout << "Key callback called" << std::endl;
}
//callback de botones del ratón
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		//std::cout << "Se ha pulsado " << button << std::endl;
	}
	else if (action == GLFW_RELEASE)
	{
		//std::cout << "Se ha soltado " << button << std::endl;
	}
}
//callback rueda ratón
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	//std::cout << "Movida la rueda del raton " << xoffset
	//	<< " Unidades en horizontal y " << yoffset << " unidades en vertical"
	//	<< std::endl;
	PAG::Renderer::getInstance()->changeColor(yoffset);
	window_refresh_callback(window);
}

int main()
{
	PAG::Log* log = PAG::Log::getInstance();
	// - Inicializa GLFW. Es un proceso que sólo debe realizarse una vez en la aplicación
	if (glfwInit() != GLFW_TRUE)
	{
		log->printMessage(PAG::msgType::ERROR, "Failed to initialize GLFW");
		return -1;
	}

	// - Definimos las características que queremos que tenga el contexto gráfico
	//   OpenGL de la ventana que vamos a crear. Por ejemplo, el número de muestras o el
	//   modo Core Profile.
	glfwWindowHint(GLFW_SAMPLES, 4); // - Activa antialising con 4 muestras
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // - Esta y las 2
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // siguientes activan un contexto
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1); // OpenGL Core Profile 4.1
	// - Definimos el puntero para guardar la dirección de la ventana de la aplicación y la creamos
	GLFWwindow* window;
	// - Tamaño, título de la ventana, en ventana y no en pantalla completa, sin compartir recursos con otras ventanas.
	window = glfwCreateWindow(1024, 576, "PAG2122-[ColominaMonsalve-Orestes]", nullptr, nullptr);
	//¿salió bien?
	if (!window)
	{
		log->printMessage(PAG::msgType::ERROR, "Failed to open GLFW window");
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
		log->printMessage(PAG::msgType::ERROR, "Failed to initialize GLEW");
		glfwDestroyWindow(window);
		glfwTerminate();
		return -3;
	}
	log->printMessage(PAG::msgType::INFO, "Starting PAG 2122 " + PAG::Renderer::getInstance()->version);
	PAG::Renderer::getInstance()->printInfo();
	//registrar callbacks
	glfwSetWindowRefreshCallback(window, window_refresh_callback);	
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);
	
	//configurar OpenGL
	PAG::Renderer::getInstance()->start();

	//Ciclo de eventos
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	window = nullptr;
	glfwTerminate();
	log->printMessage(PAG::msgType::INFO, "PAR2122 is finished!");
	std::cout << "Press enter to continue..." << std::endl;
	std::cin.get();
}

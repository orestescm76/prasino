/*
* @author orestescm76
* @brief main
* VERSION 0.7.0a1
* 
*/
#include "pch.h"
#include "Renderer.h"
#include "Log.h"
#include "Model.h"

double xold, yold = 0.0;
bool hideMouse = false;

void hideShowMouse(GLFWwindow* window)
{
	if (hideMouse)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		hideMouse = false;
	}
	else
	{
		glfwSetInputMode(window, GLFW_CURSOR,  GLFW_CURSOR_NORMAL);
		hideMouse = true;
	}
}
//callback de redibujar
void window_refresh_callback(GLFWwindow* window)
{
	PAG::Renderer::getInstance()->refreshWindow();
	glfwSwapBuffers(window);
	//std::cout << "Ventana redibujada" << std::endl;
}
//callback al redimensionar ventana - C400, par�metro formal sin referencia
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	PAG::Renderer::getInstance()->configViewport(width, height);
	window;
	//std::cout << "Ventana redimensionada: " << width << "x" << height << std::endl;
}
//callback al pulsar una tecla
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	mods;
	scancode;		
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			break;
		case GLFW_KEY_P:
			PAG::Log::getInstance()->printMessage(PAG::msgType::INFO, "Pan set");
			PAG::Renderer::getInstance()->getCamera().setMov(PAG::MovType::PAN);
				break;
		case GLFW_KEY_T:
			PAG::Log::getInstance()->printMessage(PAG::msgType::INFO, "Tilt set");
			PAG::Renderer::getInstance()->getCamera().setMov(PAG::MovType::TILT);
			break;
		case GLFW_KEY_O:
			PAG::Log::getInstance()->printMessage(PAG::msgType::INFO, "Orbit set");
			PAG::Renderer::getInstance()->getCamera().setMov(PAG::MovType::ORBIT);
			break;
		case GLFW_KEY_C:
			PAG::Log::getInstance()->printMessage(PAG::msgType::INFO, "Crane set");
			PAG::Renderer::getInstance()->getCamera().setMov(PAG::MovType::CRANE);
			break;
		case GLFW_KEY_L:
			PAG::Log::getInstance()->printMessage(PAG::msgType::INFO, "Dolly set");
			PAG::Renderer::getInstance()->getCamera().setMov(PAG::MovType::DOLLY);
			break;
		case GLFW_KEY_UP:
		case GLFW_KEY_DOWN:
		case GLFW_KEY_LEFT:
		case GLFW_KEY_RIGHT:
		case GLFW_KEY_W:
		case GLFW_KEY_S:
			PAG::Renderer::getInstance()->getCamera().move(key);
			break;
		case GLFW_KEY_A:
			std::cout << "Drawing" << std::endl;
			PAG::Renderer::getInstance()->setDrawingTriangle(true);
			PAG::Renderer::getInstance()->draw();
			break;
		case GLFW_KEY_D:
			std::cout << "Deleting" << std::endl;
			PAG::Renderer::getInstance()->setDrawingTriangle(false);
			PAG::Renderer::getInstance()->erase();
			break;
		case GLFW_KEY_Z:
			hideShowMouse(window);
			break;
		case GLFW_KEY_H:
			std::cout << "'p' for panning" << std::endl
				<< "'t' for tilting" << std::endl
				<< "'o' for orbit" << std::endl
				<< "'c' for crane" << std::endl
				<< "'l' for dolly" << std::endl
				<< "'a' for creating the triangle" << std::endl
				<< "'d' for destroying the triangle" << std::endl
				<< "'z' to hide/show cursor" << std::endl;
			break;
		case GLFW_KEY_R:
			PAG::Renderer::getInstance()->getCamera().reset();
			break;
		default:
			std::cout << "Press 'h' for the keybinds" << std::endl;
			break;
		}
		window_refresh_callback(window);
	}
	
}
//callback de botones del rat�n
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	mods;
	button;
	window;

	if (action == GLFW_PRESS)
	{

		//std::cout << "Se ha pulsado " << button << std::endl;
	}
	else if (action == GLFW_RELEASE)
	{
		//std::cout << "Se ha soltado " << button << std::endl;
	}
}
//callback rueda rat�n
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	xoffset;
	//std::cout << "Movida la rueda del raton " << xoffset
	//	<< " Unidades en horizontal y " << yoffset << " unidades en vertical"
	//	<< std::endl;
	PAG::Renderer::getInstance()->getCamera().zoom(yoffset);
	PAG::Renderer::getInstance()->changeColor(yoffset);
	window_refresh_callback(window);
}

void mouse_moved_callback(GLFWwindow* window, double xpos, double ypos)
{
	double xoffset = xpos - xold;
	double yoffset = ypos - yold;
	PAG::Renderer::getInstance()->getCamera().move(xoffset, yoffset);
	xold = xpos;
	yold = ypos;
	window_refresh_callback(window);
}

int main()
{
	// - Inicializa GLFW. Es un proceso que s�lo debe realizarse una vez en la aplicaci�n
	if (glfwInit() != GLFW_TRUE)
	{
		PAG::Log::getInstance()->printMessage(PAG::msgType::ERROR, "Failed to initialize GLFW");
		return -1;
	}

	// - Definimos las caracter�sticas que queremos que tenga el contexto gr�fico
	//   OpenGL de la ventana que vamos a crear. Por ejemplo, el n�mero de muestras o el
	//   modo Core Profile.
	glfwWindowHint(GLFW_SAMPLES, 4); // - Activa antialising con 4 muestras
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // - Esta y las 2
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // siguientes activan un contexto
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1); // OpenGL Core Profile 4.1
	// - Definimos el puntero para guardar la direcci�n de la ventana de la aplicaci�n y la creamos
	GLFWwindow* window;
	// - Tama�o, t�tulo de la ventana, en ventana y no en pantalla completa, sin compartir recursos con otras ventanas.
	window = glfwCreateWindow(1280, 720, "PAG2122-[ColominaMonsalve-Orestes]", nullptr, nullptr);
	//�sali� bien?
	if (!window)
	{
		PAG::Log::getInstance()->printMessage(PAG::msgType::ERROR, "Failed to open GLFW window");
		glfwTerminate();
		return -2;
	}
	// - Hace que el contexto OpenGL asociado a la ventana que acabamos de crear pase a
    //   ser el contexto actual de OpenGL para las siguientes llamadas a la biblioteca
 	glfwMakeContextCurrent(window);
	// - Ahora inicializamos GLEW.
    //   IMPORTANTE: GLEW debe inicializarse siempre DESPU�S de que se haya
    //               inicializado GLFW y creado la ventana
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		PAG::Log::getInstance()->printMessage(PAG::msgType::ERROR, "Failed to initialize GLEW");
		glfwDestroyWindow(window);
		glfwTerminate();
		return -3;
	}
	//Start the renderer.
	try
	{
		PAG::Log::getInstance()->printMessage(PAG::msgType::INFO, "Starting PAG 2122 " + PAG::Renderer::getInstance()->version);
	}
	catch (const std::exception& e)
	{
		PAG::Log::getInstance()->printMessage(PAG::msgType::ERROR, e.what());
		return -1;
	}
	PAG::Renderer::getInstance()->configViewport(1280, 720);
	PAG::Renderer::getInstance()->printInfo();
	//registrar callbacks
	glfwSetWindowRefreshCallback(window, window_refresh_callback);	
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, mouse_moved_callback);
	//configurar OpenGL
	PAG::Renderer::getInstance()->start();
	glfwGetCursorPos(window, &xold, &yold);
	//Ciclo de eventos

	while (!glfwWindowShouldClose(window))
	{

		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	window = nullptr;
	glfwTerminate();
	delete PAG::Renderer::getInstance();
	PAG::Log::getInstance()->printMessage(PAG::msgType::INFO, "PAR2122 is finished!");
	std::cout << "Press enter to continue..." << std::endl;
	std::cin.get();
	return 0;
}

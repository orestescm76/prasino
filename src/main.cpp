/*
* @author orestescm76
* @brief main
* VERSION 0.9.0a1
* 
*/
#include "pch.h"
#include "Renderer.h"
#include "Log.h"
#include "Model.h"

double xold, yold = 0.0;
bool hideMouse = false;

void printHelp()
{
	std::cout
		<< "'1' shows the triangle" << std::endl
		<< "'2' shows the tetrahedron" << std::endl
		<< "'p' for panning" << std::endl
		<< "'t' for tilting" << std::endl
		<< "'o' for orbit" << std::endl
		<< "'c' for crane" << std::endl
		<< "'f' for dolly" << std::endl
		<< "'z' to hide/show cursor" << std::endl
		<< "'l' to lock the camera" << std::endl
		<< "'v' to set the render to Fill" << std::endl
		<< "'b' to set the render to Wire" << std::endl
		<< "'r' to reset the camera" << std::endl
		<< "Mouse wheel to change the color and zoom!" << std::endl;
}

void hideShowMouse(GLFWwindow* window)
{
	if (hideMouse)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		hideMouse = false;
	}
	else
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		hideMouse = true;
	}
}
//callback de redibujar
void window_refresh_callback(GLFWwindow* window)
{
	try
	{
		PAG::Renderer::getInstance()->refreshWindow();
	}
	catch (const std::exception& e)
	{
		PAG::Log::getInstance()->printMessage(PAG::msgType::ERROR, (std::string)e.what());

		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	glfwSwapBuffers(window);
	//std::cout << "Ventana redibujada" << std::endl;
}
//callback al redimensionar ventana - C400, parámetro formal sin referencia
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
			std::cout << "Move the mouse!" << std::endl;
			PAG::Renderer::getInstance()->setCamera(PAG::MovType::PAN);
				break;
		case GLFW_KEY_T:
			PAG::Log::getInstance()->printMessage(PAG::msgType::INFO, "Tilt set");
			std::cout << "Move the mouse!" << std::endl;
			PAG::Renderer::getInstance()->setCamera(PAG::MovType::TILT);
			break;
		case GLFW_KEY_O:
			PAG::Log::getInstance()->printMessage(PAG::msgType::INFO, "Orbit set");
			std::cout << "Move the mouse!" << std::endl;
			PAG::Renderer::getInstance()->setCamera(PAG::MovType::ORBIT);
			break;
		case GLFW_KEY_C:
			PAG::Log::getInstance()->printMessage(PAG::msgType::INFO, "Crane set");
			PAG::Renderer::getInstance()->setCamera(PAG::MovType::CRANE);
			std::cout << "Press up to move up\nPress down to move down" << std::endl;
			break;
		case GLFW_KEY_F:
			PAG::Log::getInstance()->printMessage(PAG::msgType::INFO, "Dolly set");
			PAG::Renderer::getInstance()->setCamera(PAG::MovType::DOLLY);
			std::cout << "Press left to move in the -X axis\nPress right to move in the +X axis" << std::endl;
			std::cout << "Press S to move in the -Z axis\nPress W to move in the +Z axis" << std::endl;
			break;
		case GLFW_KEY_L:
			PAG::Log::getInstance()->printMessage(PAG::msgType::INFO, "Camera locked");
			PAG::Renderer::getInstance()->setCamera(PAG::MovType::LOCK);
			break;
		case GLFW_KEY_V:
			PAG::Log::getInstance()->printMessage(PAG::msgType::INFO, "Set fill");
			PAG::Renderer::getInstance()->setRenderType(PAG::RenderType::SOLID);
			break;
		case GLFW_KEY_B:
			PAG::Log::getInstance()->printMessage(PAG::msgType::INFO, "Set wire");
			PAG::Renderer::getInstance()->setRenderType(PAG::RenderType::WIRE);
			break;
		case GLFW_KEY_UP:
		case GLFW_KEY_DOWN:
		case GLFW_KEY_LEFT:
		case GLFW_KEY_RIGHT:
		case GLFW_KEY_W:
		case GLFW_KEY_S:
			PAG::Renderer::getInstance()->moveCamera(key);
			break;
		case GLFW_KEY_1:
			PAG::Renderer::getInstance()->setDrawingTriangle(true);
			PAG::Renderer::getInstance()->erase();
			break;
		case GLFW_KEY_2:
			PAG::Renderer::getInstance()->setDrawingTriangle(false);
			PAG::Renderer::getInstance()->erase();
			break;
		case GLFW_KEY_Z:
			hideShowMouse(window);
			break;
		case GLFW_KEY_H:
			printHelp();
			break;
		case GLFW_KEY_R:
			PAG::Renderer::getInstance()->resetCamera();
			break;
		default:
			std::cout << "Press 'h' for the keybinds" << std::endl;
			break;
		}
		window_refresh_callback(window);
	}
	
}
//callback de botones del ratón
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
//callback rueda ratón
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	xoffset;
	//std::cout << "Movida la rueda del raton " << xoffset
	//	<< " Unidades en horizontal y " << yoffset << " unidades en vertical"
	//	<< std::endl;
	PAG::Renderer::getInstance()->zoomCamera(yoffset);
	PAG::Renderer::getInstance()->changeColor(yoffset);
	window_refresh_callback(window);
}

void mouse_moved_callback(GLFWwindow* window, double xpos, double ypos)
{
	double xoffset = xpos - xold;
	double yoffset = ypos - yold;
	int mouse_state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if(mouse_state == GLFW_PRESS)
		PAG::Renderer::getInstance()->moveCamera(xoffset, yoffset);
	xold = xpos;
	yold = ypos;
	window_refresh_callback(window);
}

int main()
{
	// - Inicializa GLFW. Es un proceso que sólo debe realizarse una vez en la aplicación
	if (glfwInit() != GLFW_TRUE)
	{
		PAG::Log::getInstance()->printMessage(PAG::msgType::ERROR, "Failed to initialize GLFW");
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
	window = glfwCreateWindow(1280, 720, "PAG2122-[ColominaMonsalve-Orestes]", nullptr, nullptr);
	//¿salió bien?
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
    //   IMPORTANTE: GLEW debe inicializarse siempre DESPUÉS de que se haya
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
	//First I draw before any event
	try
	{
		PAG::Renderer::getInstance()->refreshWindow();

	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
		return -5;
	}
	std::cout << "Press 'h' to print keybinds" << std::endl;
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

/*
* @author orestescm76
* @brief main
* VERSION 1.0.0
* i'd love to add imgui but that's for another day
*/
#include "pch.h"
#include "Renderer.h"
#include "Log.h"
#include "Model.h"

double xold, yold = 0.0;
bool hideMouse = false;

void printHelp()
{
#ifdef _WIN32
	OPENFILENAME ofl;
	wchar_t filename[MAX_PATH] = L"";
	ofl.lStructSize = sizeof(OPENFILENAME);
	ofl.hwndOwner = NULL;
	ofl.lpstrFile = filename;
	ofl.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
	ofl.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	ofl.nMaxFile = sizeof(filename);
	ofl.nFilterIndex = 1;
	ofl.lpstrFileTitle = NULL;
	ofl.nMaxFileTitle = 0;
	ofl.lpstrInitialDir = NULL;
	if (GetOpenFileName(&ofl) == TRUE)
	{
		MessageBox(NULL, ofl.lpstrFile, L"", MB_OK);
	}
	MessageBox(NULL, LR"("'1' adds the triangle
'2' adds the tetrahedron
'3' adds the cow
'4' adds the Rook
'5' adds the Tottenham Spurs cube (NM)
'6' adds the Fallen Knight (NM)
'p' for panning
't' for tilting
'o' for orbit
'c' for crane
'f' for dolly
'z' to hide/show cursor
'l' to lock the camera
'v' to set the render to Fill
'b' to set the render to Wire
'r' to reset the camera
'd' deletes the active model
'u' adds the texture to the active model
'i' adds normal mapping to the active model, if possible
'z' locks the cursor inside the window
'TAB' changes the active model
Mouse wheel to change the color and zoom!")", L"Help", 0);
#endif // _WIN32

	std::cout
		<< "'1' adds the triangle" << std::endl
		<< "'2' adds the tetrahedron" << std::endl
		<< "'3' adds the cow" << std::endl
		<< "'4' adds the Rook" << std::endl
		<< "'5' adds the Tottenham Spurs cube (NM)" << std::endl
		<< "'6' adds the Fallen Knight (NM)" << std::endl
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
		<< "'d' deletes the active model" << std::endl
		<< "'u' adds the texture to the active model" << std::endl
		<< "'i' adds normal mapping to the active model, if possible" << std::endl
		<< "'z' locks the cursor inside the window" << std::endl
		<< "'TAB' changes the active model" << std::endl
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
		PAG::Log::getInstance()->printMessage(PAG::msgType::LOG_ERROR, (std::string)e.what());

		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

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
			PAG::Renderer::getInstance()->setRenderTypeToActiveModel(PAG::RenderType::SOLID);
			break;
		case GLFW_KEY_B:
			PAG::Log::getInstance()->printMessage(PAG::msgType::INFO, "Set wire");
			PAG::Renderer::getInstance()->setRenderTypeToActiveModel(PAG::RenderType::WIRE);
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
			PAG::Renderer::getInstance()->addModel(PAG::ModelType::TRIANGLE);
			break;
		case GLFW_KEY_2:
			PAG::Renderer::getInstance()->addModel(PAG::ModelType::TETRAHEDRON);
			break;
		case GLFW_KEY_3:
			PAG::Renderer::getInstance()->addModel("./models/vaca.obj", "cow");
			break;
		case GLFW_KEY_4:
			PAG::Renderer::getInstance()->addModel("./models/Rook.obj", "rook");
			break;
		case GLFW_KEY_5:
			PAG::Renderer::getInstance()->addModel("./models/cube.obj", "spurs");
			break;
		case GLFW_KEY_6:
			PAG::Renderer::getInstance()->addModel("./models/Knight.obj", "knight");
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
		case GLFW_KEY_D:
			PAG::Renderer::getInstance()->deleteActiveModel();
			break;
		case GLFW_KEY_U:
			PAG::Renderer::getInstance()->setTextureToActiveModel();
			break;
		case GLFW_KEY_I:
			//PAG::Renderer::getInstance()->setNormalMappingToActiveModel();
			break;
		case GLFW_KEY_TAB:
			PAG::Renderer::getInstance()->step();
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
	// - Inicializa GLFW. Es un proceso que s�lo debe realizarse una vez en la aplicaci�n
	if (glfwInit() != GLFW_TRUE)
	{
		PAG::Log::getInstance()->printMessage(PAG::msgType::LOG_ERROR, "Failed to initialize GLFW");
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
	window = glfwCreateWindow(1280, 720, "Prasino 1.0", nullptr, nullptr);
	//�sali� bien?
	if (!window)
	{
		PAG::Log::getInstance()->printMessage(PAG::msgType::LOG_ERROR, "Failed to open GLFW window");
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
		PAG::Log::getInstance()->printMessage(PAG::msgType::LOG_ERROR, "Failed to initialize GLEW");
		glfwDestroyWindow(window);
		glfwTerminate();
		return -3;
	}
	//Start the renderer.
	try
	{
		PAG::Log::getInstance()->printMessage(PAG::msgType::INFO, "Starting Prasino " + PAG::Renderer::getInstance()->version);
	}
	catch (const std::exception& e)
	{
		PAG::Log::getInstance()->printMessage(PAG::msgType::LOG_ERROR, e.what());
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
	glfwSwapInterval(1);
	//Ciclo de eventos
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	window = nullptr;
	glfwTerminate();
	delete PAG::Renderer::getInstance();
	PAG::Log::getInstance()->printMessage(PAG::msgType::INFO, "Prasino is finished!");
	//std::cout << "Press enter to continue..." << std::endl;
	//std::cin.get();
	return 0;
}
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdLine, int cmdshow)
{
	return main();
}
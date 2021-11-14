/*
* @author orestescm76
* @brief renderer cpp
*/

#include "pch.h"
#include "Renderer.h"

PAG::Renderer* PAG::Renderer::instance = nullptr;
const std::string PAG::Renderer::version = "0.8.0a1";

PAG::Renderer::Renderer() : 
	copper(glm::vec3(0.19125, 0.0735, 0.0225), glm::vec3(.7038,.27048,.0828), { .256777, .137622, 0.086014 }, 64.0f), 
	camera(),
	ambL(glm::vec3(.25f,.25f,.25f)),
	point(glm::vec3(.8f, .8f, .8f), glm::vec3(1,1,1), glm::vec3(1,0,1))
{
	try
	{
		sp = std::make_shared<ShaderProgram>("pag08-vs.glsl", "pag08-fs.glsl");
		spLightCube = std::make_shared<ShaderProgram>("pag08-vs.glsl", "pag08-light-fs.glsl");
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error("PAG::Renderer::Renderer -> " + (std::string)e.what());
	}
	triangle = std::make_unique <Model>(sp, PAG::ModelType::TRIANGLE, copper);
	tetrahedron = std::make_unique<Model>(sp, PAG::ModelType::TETRAHEDRON, copper);
	lightCube = std::make_unique<Model>(spLightCube, ModelType::LIGHT_CUBE);
	backColor = { 0,0,0,1 };
}

void PAG::Renderer::configBackColor(glm::vec4 color)
{
	backColor = color;
	glClearColor(backColor.r, backColor.g, backColor.b, backColor.a);
}


PAG::Renderer::~Renderer()
{
	//std::unique_ptr and std::shared_ptr destroys on its own and we do not need to delete it manually
}

void PAG::Renderer::loadUniforms()
{
	if (drawingTriangle)
	{
		triangle->useProgram();
		sp->getFragmentShader().setUniformVec3("Ka", triangle->getMaterial().ambient);
		sp->getFragmentShader().setUniformVec3("Kd", triangle->getMaterial().diffuse);
		sp->getFragmentShader().setUniformVec3("Ks", triangle->getMaterial().specular);
		sp->getFragmentShader().setUniformFloat("shininess", triangle->getMaterial().shininess);
	}
	else
	{
		tetrahedron->useProgram();
		sp->getFragmentShader().setUniformVec3("Ka", tetrahedron->getMaterial().ambient);
		sp->getFragmentShader().setUniformVec3("Kd", tetrahedron->getMaterial().diffuse);
		sp->getFragmentShader().setUniformVec3("Ks", tetrahedron->getMaterial().specular);
		sp->getFragmentShader().setUniformFloat("shininess", tetrahedron->getMaterial().shininess);
	}
}

void PAG::Renderer::activateLight(Light& l)
{
	switch (l.type)
	{
	case LightType::AMBIENT:
		
		sp->getFragmentShader().setUniformSubroutine("", "ambientColor");
		sp->getFragmentShader().setUniformVec3("Ia", ambL.ambient);
		break;
	default:
		sp->getFragmentShader().setUniformSubroutine("", "diffuseAndSpecular");
		sp->getFragmentShader().setUniformVec3("Id", point.diffuse);
		sp->getFragmentShader().setUniformVec3("Is", point.specular);
		sp->getFragmentShader().setUniformVec3("lPos", point.position);

		break;
	}
	
}

PAG::Renderer* PAG::Renderer::getInstance()
{
	if (!instance)
	{
		try
		{
			instance = new Renderer();
		}
		catch (const std::exception& e)
		{
			throw std::runtime_error("Renderer::getInstance() -> " + (std::string)e.what());
		}
	}
	return instance;
}

void PAG::Renderer::refreshWindow()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (!triangle.get() && drawingTriangle) //If it's destroyed and we need to draw
		triangle = std::make_unique<Model>(sp, PAG::ModelType::TRIANGLE, copper);

	if (!tetrahedron.get() && !drawingTriangle) //If it's destroyed and we need to draw
		tetrahedron = std::make_unique<Model>(sp, PAG::ModelType::TETRAHEDRON, copper);

	
	activateLight(ambL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	loadUniforms();
	draw();
	activateLight(point);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	draw();

}

void PAG::Renderer::changeColor(double yoffset)
{
	if (yoffset > 0)
		addColor();
	else minusColor();
	configBackColor(backColor);
}

void PAG::Renderer::start()
{
	configBackColor({0,0,0,1});
	activeZBuffer();
	//Polilínea, sólidos de revolución
	//glPrimitiveRestartIndex(0xFFFF);
	//glEnable(GL_PRIMITIVE_RESTART);
	
	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
}

void PAG::Renderer::activeZBuffer()
{
	// - Le decimos a OpenGL que tenga en cuenta la profundidad a la hora de dibujar.
	//   No tiene por qué ejecutarse en cada paso por el ciclo de eventos.
	glEnable(GL_DEPTH_TEST);
}

void PAG::Renderer::addColor()
{
	backColor.r += .05f;
	backColor.g += .05f;
	backColor.b += .05f;
	if (backColor.r >= 1.0f)
		backColor = glm::vec4(1);
}

void PAG::Renderer::minusColor()
{
	backColor.r -= .05f;
	backColor.g -= .05f;
	backColor.b -= .05f;
	if (backColor.r <= 0.0f)
		backColor = glm::vec4(0);
}

void PAG::Renderer::printInfo()
{
	std::string renderer((const char*)glGetString(GL_RENDERER));
	std::string vendor = (const char*)glGetString(GL_VENDOR);
	std::string openglversion = (const char*)glGetString(GL_VERSION);
	std::string shadingVersion = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
	Log::getInstance()->printMessage(PAG::msgType::INFO, "RENDERER: " + renderer);
	Log::getInstance()->printMessage(PAG::msgType::INFO, "VENDOR: " + vendor);
	Log::getInstance()->printMessage(PAG::msgType::INFO, "VERSION: " + openglversion);
	Log::getInstance()->printMessage(PAG::msgType::INFO, "SHADING LANGUAGE VERSION: " + shadingVersion);
}

void PAG::Renderer::draw()
{

		

	glm::mat4 view = camera.getViewMatrix();
	glm::mat4 proj = camera.getProjMatrix();
	//Multiply the matrices
	glm::mat4 projview = proj * view;
	glm::mat4 projviewmod = proj * view * glm::translate(glm::mat4(1), point.position);
	sp->useProgram();
	try
	{
		sp->getVertexShader().setUniformMat4("matProjViewModel", projview);
		sp->getVertexShader().setUniformMat4("matModel", glm::mat4(1.0));

		if (drawingTriangle)
		{
			triangle->setDrawingMode(renderType);
			triangle->draw();
		}
		else
		{
			tetrahedron->setDrawingMode(renderType);
			tetrahedron->draw();
		}
		spLightCube->useProgram();
		spLightCube->getVertexShader().setUniformMat4("matProjViewModel", projviewmod);
		lightCube->draw();
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error("PAG::Renderer::draw() -> " + (std::string)e.what());
	}


}

void PAG::Renderer::erase()
{
	if (triangle.get()) //if there is a triangle
		triangle.reset(); //Destroy the triangle, but do it once!
	
	if (tetrahedron.get())
		tetrahedron.reset();
}

void PAG::Renderer::configViewport(int width, int height)
{
	wViewport = width;
	hViewport = height;
	camera.setViewport(width, height);
	glViewport(0, 0, width, height);
}

bool PAG::Renderer::isDrawingTriangle()
{
	return drawingTriangle;
}

void PAG::Renderer::setDrawingTriangle(bool draw)
{
	drawingTriangle = draw;
}

void PAG::Renderer::setRenderType(RenderType rt)
{
	renderType = rt;
}

void PAG::Renderer::moveCamera(int key)
{
	camera.move(key);
}

void PAG::Renderer::moveCamera(float xoffset, float yoffset)
{
	camera.move(xoffset, yoffset);
}

void PAG::Renderer::setCamera(MovType mov)
{
	camera.setMov(mov);
}

void PAG::Renderer::resetCamera()
{
	camera.reset();
}

void PAG::Renderer::zoomCamera(float yoffset)
{
	camera.zoom(yoffset);
}
/*
* @author orestescm76
* @brief renderer cpp
*/

#include "pch.h"
#include "Renderer.h"

PAG::Renderer* PAG::Renderer::instance = nullptr;
const std::string PAG::Renderer::version = "0.8.0a3";

PAG::Renderer::Renderer() : 
	mat(glm::vec3(0,0,0), glm::vec3(.1,.35,.1), glm::vec3(.45,.55,.45), 0.9f*128.0f),
	camera(),
	lights()
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
	triangle = std::make_unique <Model>(sp, PAG::ModelType::TRIANGLE, mat);
	tetrahedron = std::make_unique<Model>(sp, PAG::ModelType::TETRAHEDRON, mat);
	lightCube = std::make_unique<Model>(spLightCube, ModelType::LIGHT_CUBE);
	Light ambL(glm::vec3(.12,.12,.12));
	Light point(glm::vec3(1.0f,1.0f,1.0f), glm::vec3(.7f,.7f,.7f), glm::vec3(.5f,0.0f,.5f), LightType::POINT);
	Light dir(glm::vec3(.3,.3,.3), glm::vec3(.5,.5,.5),glm::vec3(0,0,1), LightType::DIRECTIONAL);
	Light spot(glm::vec3(.9f, .9f, .9f), glm::vec3(.7,.7,.7), glm::vec3(.6,.5,.6), glm::vec3(-1,-1,-1), 30);
	//lights.push_back(ambL);
	lights.push_back(point);
	//lights.push_back(dir);
	//lights.push_back(spot);
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

void PAG::Renderer::activateLight(Light& l, ShaderProgram& shaderProgram, Model* model)
{
	glm::vec3 lPos, lDir;
	switch (l.type)
	{
	case LightType::AMBIENT:
		shaderProgram.getFragmentShader().setUniformSubroutine("", "ambientColor");
		shaderProgram.getFragmentShader().setUniformVec3("Ia", l.ambient);
		shaderProgram.getFragmentShader().setUniformVec3("Ka", model->getMaterial().ambient);
		break;
	case LightType::POINT:
		shaderProgram.getFragmentShader().setUniformSubroutine("", "point");
		shaderProgram.getFragmentShader().setUniformVec3("Id", l.diffuse);
		shaderProgram.getFragmentShader().setUniformVec3("Is", l.specular);
		//Apply transform
		lPos = glm::vec3(camera.getViewMatrix() * glm::vec4(l.position, 1.0f));
		shaderProgram.getFragmentShader().setUniformVec3("lPos", lPos);

		shaderProgram.getFragmentShader().setUniformVec3("Kd", model->getMaterial().diffuse);
		shaderProgram.getFragmentShader().setUniformVec3("Ks", model->getMaterial().specular);
		shaderProgram.getFragmentShader().setUniformFloat("shininess", model->getMaterial().shininess);

		break;
	case LightType::DIRECTIONAL:
		shaderProgram.getFragmentShader().setUniformSubroutine("", "directional");
		shaderProgram.getFragmentShader().setUniformVec3("Id", l.diffuse);
		shaderProgram.getFragmentShader().setUniformVec3("Is", l.specular);
		//Apply transform
		glm::vec3 lDir = glm::vec3(camera.getViewMatrix() * glm::vec4(l.direction, 0));
		lDir = glm::normalize(lDir);
		shaderProgram.getFragmentShader().setUniformVec3("lDir", lDir);
		shaderProgram.getFragmentShader().setUniformVec3("Kd", model->getMaterial().diffuse);
		shaderProgram.getFragmentShader().setUniformVec3("Ks", model->getMaterial().specular);
		shaderProgram.getFragmentShader().setUniformFloat("shininess", model->getMaterial().shininess);
		break;
	case LightType::SPOTLIGHT:
		shaderProgram.getFragmentShader().setUniformSubroutine("", "spot");
		shaderProgram.getFragmentShader().setUniformVec3("Id", l.diffuse);
		shaderProgram.getFragmentShader().setUniformVec3("Is", l.specular);
		//Apply transform
		//lPos = glm::vec3(camera.getViewMatrix() * glm::vec4(l.position, 1));
		//shaderProgram.getFragmentShader().setUniformVec3("lPos", lPos);

		//Apply transform
		lDir = glm::vec3(camera.getViewMatrix() * glm::vec4(l.direction, 0));
		lDir = glm::normalize(lDir);
		shaderProgram.getFragmentShader().setUniformVec3("lDir", lDir);

		shaderProgram.getFragmentShader().setUniformFloat("sAngle", glm::radians(l.angle));

		shaderProgram.getFragmentShader().setUniformVec3("Kd", model->getMaterial().diffuse);
		shaderProgram.getFragmentShader().setUniformVec3("Ks", model->getMaterial().specular);
		shaderProgram.getFragmentShader().setUniformFloat("shininess", model->getMaterial().shininess);
		break;
	}

}

void PAG::Renderer::loadCameraUniforms(ShaderProgram& shaderProgram)
{
	glm::mat4 view = camera.getViewMatrix();
	glm::mat4 proj = camera.getProjMatrix();
	//Multiply the matrices
	glm::mat4 projview = proj * view;
	

	shaderProgram.getVertexShader().setUniformMat4("matProjViewModel", projview);
	shaderProgram.getVertexShader().setUniformMat4("matModelView", view*glm::mat4(1));
	
}

void PAG::Renderer::drawLightCube(Light& l)
{
	spLightCube->useProgram();
	glm::mat4 projviewmod = camera.getProjMatrix() * camera.getViewMatrix() * glm::translate(glm::mat4(1), l.position);
	spLightCube->getVertexShader().setUniformMat4("matProjViewModel", projviewmod);
	spLightCube->getFragmentShader().setUniformVec3("lightColor", l.diffuse);
	lightCube->draw();
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
		triangle = std::make_unique<Model>(sp, PAG::ModelType::TRIANGLE, mat);

	if (!tetrahedron.get() && !drawingTriangle) //If it's destroyed and we need to draw
		tetrahedron = std::make_unique<Model>(sp, PAG::ModelType::TETRAHEDRON, mat);

	//sp->useProgram();
	//activateLight(ambL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	draw(lights[0]);
	drawLightCube(lights[0]);
	//activateLight(point);
	//loadUniforms();
	
	if (lights.size() > 1)
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		for (size_t i = 1; i < lights.size(); i++)
		{
			draw(lights[i]);
			drawLightCube(lights[i]);
		}
	}
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
	glDepthFunc(GL_LEQUAL);
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

void PAG::Renderer::draw(Light& l)
{
	try
	{
		if (drawingTriangle)
		{	//WE ACTIVATE THE PROGRAM HERE
			triangle->useProgram();
			
			//Set the light
			activateLight(l, *triangle->getShaderProgram(), triangle.get());

			loadCameraUniforms(*triangle->getShaderProgram());
			//Set the drawing mode.
			triangle->setDrawingMode(renderType);
			//Ready
			triangle->draw();
		}
		else
		{
			tetrahedron->useProgram();
			loadCameraUniforms(*tetrahedron->getShaderProgram());
			//Set the drawing mode.
			tetrahedron->setDrawingMode(renderType);
			//Set the light
			activateLight(l, *tetrahedron->getShaderProgram(), tetrahedron.get());
			//Ready
			tetrahedron->draw();
		}
		//if(l.type != LightType::AMBIENT)
		//	drawLightCube(l);
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
/*
* @author orestescm76
* @brief renderer cpp
*/

#include "pch.h"
#include "Renderer.h"

PAG::Renderer* PAG::Renderer::instance = nullptr;
const std::string PAG::Renderer::version = "0.9.0a1";

PAG::Renderer::Renderer() :
	//mat(glm::vec3(.1745f, .01175f,.01175f), glm::vec3(.61424f, .04136f, .04136f), glm::vec3(.727811f,.626959,.626959f), 0.6f*128.0f),
	mat(glm::vec3(0.0215,0.1745,0.0215),glm::vec3(0.07568,	0.61424 ,	0.07568),glm::vec3(0.633, 	0.727811 ,	0.633), .6f*128.0f),
	camera(),
	lights(),
	models()
{
	try
	{
		shaderProgram = std::make_shared<ShaderProgram>("pag08-vs.glsl", "pag08-fs.glsl");
		spLightCube = std::make_shared<ShaderProgram>("pag08-vs.glsl", "pag08-light-fs.glsl");
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error("PAG::Renderer::Renderer -> " + (std::string)e.what());
	}
	//models.push_back(createModel(ModelType::TRIANGLE, shaderProgram, mat));
	//models.push_back(createModel(ModelType::TETRAHEDRON, shaderProgram, mat));
	lightCube = std::make_unique<Model>(spLightCube, ModelType::LIGHT_CUBE);
	models.push_back(std::make_unique<Model>(shaderProgram,"ChessKnight.obj",mat));
	Light ambL(glm::vec3(.12,.12,.12));
	Light point(glm::vec3(1), glm::vec3(1), glm::vec3(-.2,.3,.2), LightType::POINT);
	Light dir(glm::vec3(.1f,.1f,.9f), glm::vec3(.9),glm::vec3(0,0,1), LightType::DIRECTIONAL);
	Light spot(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1,1,1), glm::vec3(-1,-1,-1), 64.0f);

	lights.push_back(ambL);
	lights.push_back(point);
	lights.push_back(dir);
	lights.push_back(spot);

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

void PAG::Renderer::activateLight(Light& l, ShaderProgram* sp, Model* model)
{
	glm::vec3 lPos, lDir;
	switch (l.type)
	{
	case LightType::AMBIENT:
		sp->getFragmentShader().setUniformSubroutine("", "ambientColor");
		sp->getFragmentShader().setUniformVec3("Ia", l.ambient);
		sp->getFragmentShader().setUniformVec3("Ka", model->getMaterial().ambient);
		break;
	//WORKS
	case LightType::POINT:
		sp->getFragmentShader().setUniformSubroutine("", "point");
		sp->getFragmentShader().setUniformVec3("Id", l.diffuse);
		sp->getFragmentShader().setUniformVec3("Is", l.specular);
		//Apply transform
		lPos = glm::vec3(camera.getViewMatrix() * glm::vec4(l.position, 1.0f));
		sp->getFragmentShader().setUniformVec3("lPos", lPos);
		sp->getFragmentShader().setUniformVec3("Kd", model->getMaterial().diffuse);
		sp->getFragmentShader().setUniformVec3("Ks", model->getMaterial().specular);
		sp->getFragmentShader().setUniformFloat("shininess", model->getMaterial().shininess);

		break;
	case LightType::DIRECTIONAL:
		sp->getFragmentShader().setUniformSubroutine("", "directional");
		sp->getFragmentShader().setUniformVec3("Id", l.diffuse);
		sp->getFragmentShader().setUniformVec3("Is", l.specular);
		//Apply transform
		glm::mat4 matrix = glm::inverse(glm::transpose(camera.getViewMatrix()));
		glm::vec3 lDir = glm::vec3(matrix * glm::vec4(l.direction, 0));
		lDir = glm::normalize(lDir);
		sp->getFragmentShader().setUniformVec3("lDir", lDir);
		sp->getFragmentShader().setUniformVec3("Kd", model->getMaterial().diffuse);
		sp->getFragmentShader().setUniformVec3("Ks", model->getMaterial().specular);
		sp->getFragmentShader().setUniformFloat("shininess", model->getMaterial().shininess);
		break;
	case LightType::SPOTLIGHT:
		sp->getFragmentShader().setUniformSubroutine("", "spot");
		sp->getFragmentShader().setUniformFloat("sAngle", glm::radians(l.angle));
		sp->getFragmentShader().setUniformVec3("Id", l.diffuse);
		sp->getFragmentShader().setUniformVec3("Is", l.specular);
		//Apply transform
		lPos = glm::vec3(camera.getViewMatrix() * glm::vec4(l.position, 1));
		sp->getFragmentShader().setUniformVec3("lPos", lPos);

		//Apply transform
		lDir = glm::vec3(camera.getViewMatrix() * glm::vec4(l.direction, 0));
		lDir = glm::normalize(lDir);
		sp->getFragmentShader().setUniformVec3("lDir", lDir);

		

		sp->getFragmentShader().setUniformVec3("Kd", model->getMaterial().diffuse);
		sp->getFragmentShader().setUniformVec3("Ks", model->getMaterial().specular);
		sp->getFragmentShader().setUniformFloat("shininess", model->getMaterial().shininess);
		break;
	}

}

void PAG::Renderer::loadUniforms(ShaderProgram* sp, Model* model)
{
	glm::mat4 mod = model->getModelMatrix();
	glm::mat4 view = camera.getViewMatrix();
	glm::mat4 proj = camera.getProjMatrix();
	//Multiply the matrices

	glm::mat4 viewMod = view * mod;
	glm::mat4 projviewmodel = proj * viewMod;
	glm::mat4 transInvViewMod = glm::transpose(glm::inverse(view*mod));

	sp->getVertexShader().setUniformMat4("matProjViewModel", projviewmodel);
	sp->getVertexShader().setUniformMat4("matModelView", viewMod);
	sp->getVertexShader().setUniformMat4("matModelViewTransInv", transInvViewMod);
}

void PAG::Renderer::drawLightCube(Light& l)
{
	if (l.type == LightType::AMBIENT)
		return;
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
	//For each model
	for (size_t i = 0; i < models.size(); i++)
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//Draw with ambient
		draw(lights[0], models[i].get());
		drawLightCube(lights[0]);
		//For each light
		if (lights.size() > 1)
		{
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			//Draw with light
			for (size_t j = 1; j < lights.size(); j++)
			{
				draw(lights[j], models[i].get());
				drawLightCube(lights[j]);
			}
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

void PAG::Renderer::draw(Light& l, Model* model)
{
	try
	{
		//First use shader program
		model->useProgram();
		//Then activate the light
		activateLight(l, model->getShaderProgram(), model);
		//Load camera uniforms
		loadUniforms(model->getShaderProgram(), model);
		//Render
		model->setDrawingMode(renderType);
		model->draw();
		if(l.type != LightType::AMBIENT)
			drawLightCube(l);
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error("PAG::Renderer::draw() -> " + (std::string)e.what());
	}
}

std::unique_ptr<PAG::Model> PAG::Renderer::createModel(ModelType type, std::shared_ptr<ShaderProgram>& sp, Material& mat)
{
	return std::make_unique<Model>(sp, type, mat);
}

void PAG::Renderer::erase()
{
	//if (triangle.get()) //if there is a triangle
	//	triangle.reset(); //Destroy the triangle, but do it once!
	//
	//if (tetrahedron.get())
	//	tetrahedron.reset();
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
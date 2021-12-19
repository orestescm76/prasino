/*
* @author orestescm76
* @brief renderer cpp
*/

#include "pch.h"
#include "Renderer.h"

PAG::Renderer* PAG::Renderer::instance = nullptr;
const std::string PAG::Renderer::version = "1.0.0a1";

PAG::Renderer::Renderer() :
	activeModel(-1),
	mat(glm::vec3(0.135, 0.2225, 0.1575), glm::vec3(0.54, 0.89, 0.63), glm::vec3(.316228), .1 * 128),
	//mat(glm::vec3(.1745f, .01175f,.01175f), glm::vec3(.61424f, .04136f, .04136f), glm::vec3(.727811f,.626959,.626959f), 0.6f*128.0f),
	//mat(glm::vec3(0.0215,0.1745,0.0215),glm::vec3(0.07568,	0.61424 ,	0.07568),glm::vec3(0.633, 	0.727811 ,	0.633), .6f*128.0f),
	camera({3,2,3}, 90, .1,50,wViewport, hViewport),
	lights(),
	models(),
	textures()
{
	try
	{
		shaderProgram = std::make_shared<ShaderProgram>("pag09-vs.glsl", "pag09-fs.glsl");
		spLightCube = std::make_shared<ShaderProgram>("pag09-vs.glsl", "pag08-light-fs.glsl");
		shaderProgramTexture = std::make_shared<ShaderProgram>("pag09-vs.glsl", "pag09-fs-tex.glsl");
		shaderProgramTextureNM = std::make_shared<ShaderProgram>("pag10-vs-nm.glsl", "pag10-fs-nm-tex.glsl");
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error("PAG::Renderer::Renderer -> " + (std::string)e.what());
	}

	
	lightCube = std::make_unique<Model>(spLightCube, ModelType::LIGHT_CUBE);
	Light ambL(glm::vec3(.18));
	Light point(glm::vec3(.3), glm::vec3(1), glm::vec3(-.5,.5,.2), LightType::POINT);
	Light dir(glm::vec3(.8,1,.2), glm::vec3(.9),glm::vec3(0,0,1), LightType::DIRECTIONAL);
	Light spot(glm::vec3(1), glm::vec3(1.0f), glm::vec3(1,1,1), glm::vec3(-1,-1,-1), 64.0f);

	lights.push_back(ambL);
	lights.push_back(point);
	lights.push_back(dir);
	lights.push_back(spot);

	backColor = { 0,0,0,1 };

	textures.insert({ "cow", std::make_shared<Texture>("./textures/vaca.png",TextureType::IMAGE) });
	textures.insert({ "win95", std::make_shared<Texture>("./textures/win95.png",TextureType::IMAGE) });
	textures.insert({ "marble", std::make_shared<Texture>("./textures/marble.png",TextureType::IMAGE) });
	textures.insert({ "spurs", std::make_shared<Texture>("./textures/tottenham.png",TextureType::IMAGE)});
	textures.insert({ "spursNM", std::make_shared<Texture>("./textures/tottenham_nm.png",TextureType::NORMAL_MAP) });
}

void PAG::Renderer::configBackColor(glm::vec4 color)
{
	backColor = color;
	glClearColor(backColor.r, backColor.g, backColor.b, backColor.a);
}


void PAG::Renderer::printActiveModel()
{
	if(activeModel != -1)
		Log::getInstance()->printMessage(msgType::INFO, "Active model: " + models[activeModel]->name());
	else
		Log::getInstance()->printMessage(msgType::INFO, "No model is active");
}

void PAG::Renderer::deleteActiveModel()
{
	if (activeModel != -1)
	{
		models[activeModel].reset();
		std::vector<std::unique_ptr<Model>>::iterator it;
		it = models.begin();
		it += activeModel;
		models.erase(it);
		activeModel--;
		printActiveModel();
	}
	if (models.size() > 0)
		activeModel = 0;
}

PAG::Renderer::~Renderer()
{
	//std::unique_ptr and std::shared_ptr destroys on its own and we do not need to delete it manually
}

void PAG::Renderer::setTextureToActiveModel()
{
	if (activeModel != -1)
	{
		Model* model = models[activeModel].get();
		//If it is not drawing texture
		if (!model->isDrawingTexture())
		{
			model->setShaderProgram(shaderProgramTexture);
			//Check models
			if (model->name() == "cow")
				model->addTexture(textures["cow"]);
			else if (model->name() == "rook")
				model->addTexture(textures["marble"]);
			else if (model->name() == "spurs")
				model->addTexture(textures["spurs"]);
			else
				model->addTexture(textures["win95"]);
			model->setDrawTexture(true);
		}
		else
		{
			model->setDrawTexture(false);
			model->setShaderProgram(shaderProgram);
			model->unBindTexture();
		}
	}
}

void PAG::Renderer::activateLight(Light& l, ShaderProgram* sp, Model* model)
{
	glm::vec3 lPos, lDir;
	switch (l.type)
	{
	case LightType::AMBIENT:
		sp->getFragmentShader().setUniformSubroutine("", "ambientColor");
		sp->getFragmentShader().setUniform("Ia", l.ambient);
		if(!model->isDrawingTexture())
			sp->getFragmentShader().setUniform("Ka", model->getMaterial().ambient);
		break;
	//WORKS
	case LightType::POINT:
		sp->getFragmentShader().setUniformSubroutine("", "point");
		sp->getFragmentShader().setUniform("Id", l.diffuse);
		sp->getFragmentShader().setUniform("Is", l.specular);
		//Apply transform
		lPos = glm::vec3(camera.getViewMatrix() * glm::vec4(l.position, 1.0f));
		sp->getFragmentShader().setUniform("lPos", lPos);
		if (!model->isDrawingTexture())
			sp->getFragmentShader().setUniform("Kd", model->getMaterial().diffuse);
		sp->getFragmentShader().setUniform("Ks", model->getMaterial().specular);
		sp->getFragmentShader().setUniform("shininess", model->getMaterial().shininess);

		break;
	case LightType::DIRECTIONAL:
		sp->getFragmentShader().setUniformSubroutine("", "directional");
		sp->getFragmentShader().setUniform("Id", l.diffuse);
		sp->getFragmentShader().setUniform("Is", l.specular);
		//Apply transform
		glm::mat4 matrix = glm::inverse(glm::transpose(camera.getViewMatrix()));
		glm::vec3 lDir = glm::vec3(matrix * glm::vec4(l.direction, 0));
		lDir = glm::normalize(lDir);
		sp->getFragmentShader().setUniform("lDir", lDir);
		if (!model->isDrawingTexture())
			sp->getFragmentShader().setUniform("Kd", model->getMaterial().diffuse);
		sp->getFragmentShader().setUniform("Ks", model->getMaterial().specular);
		sp->getFragmentShader().setUniform("shininess", model->getMaterial().shininess);
		break;
	case LightType::SPOTLIGHT:
		sp->getFragmentShader().setUniformSubroutine("", "spot");
		sp->getFragmentShader().setUniform("sAngle", glm::radians(l.angle));
		sp->getFragmentShader().setUniform("Id", l.diffuse);
		sp->getFragmentShader().setUniform("Is", l.specular);
		//Apply transform
		lPos = glm::vec3(camera.getViewMatrix() * glm::vec4(l.position, 1));
		sp->getFragmentShader().setUniform("lPos", lPos);

		//Apply transform
		lDir = glm::vec3(camera.getViewMatrix() * glm::vec4(l.direction, 0));
		lDir = glm::normalize(lDir);
		sp->getFragmentShader().setUniform("lDir", lDir);

		
		if (!model->isDrawingTexture())
			sp->getFragmentShader().setUniform("Kd", model->getMaterial().diffuse);
		sp->getFragmentShader().setUniform("Ks", model->getMaterial().specular);
		sp->getFragmentShader().setUniform("shininess", model->getMaterial().shininess);
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

	sp->getVertexShader().setUniform("matProjViewModel", projviewmodel);
	sp->getVertexShader().setUniform("matModelView", viewMod);
	if(!model->isDrawingNormalMapping())
		sp->getVertexShader().setUniform("matModelViewTransInv", transInvViewMod);
}

void PAG::Renderer::drawLightCube(Light& l)
{
	if (l.type == LightType::AMBIENT)
		return;
	spLightCube->useProgram();
	glm::mat4 projviewmod = camera.getProjMatrix() * camera.getViewMatrix() * glm::translate(glm::mat4(1), l.position);
	spLightCube->getVertexShader().setUniform("matProjViewModel", projviewmod);
	spLightCube->getFragmentShader().setUniform("lightColor", l.diffuse);
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
std::unique_ptr<PAG::Model> PAG::Renderer::createModel(std::shared_ptr<ShaderProgram>& shaderProgram, std::string filename, Material mat, std::string name, bool NM = false)
{
	if(!NM)
		return std::make_unique<Model>(shaderProgram, filename, mat, name);
	else
	{
		std::unique_ptr<Model> model = std::make_unique<Model>(shaderProgram, filename, mat, name);
		model->setNormalMapping(true);
		model->setDrawTexture(true);
		model->addTexture(textures["spurs"]);
		model->addTexture(textures["spursNM"]);
		return model;
	}
}
bool PAG::Renderer::checkExistingModel(ModelType type)
{
	for (size_t i = 0; i < models.size(); i++)
	{
		if (models[i]->getType() == type)
			return true;
	}
	return false;
}

bool PAG::Renderer::checkExistingModel(std::string name)
{
	for (size_t i = 0; i < models.size(); i++)
	{
		if (models[i]->name() == name)
			return true;
	}
	return false;
}

void PAG::Renderer::configViewport(int width, int height)
{
	wViewport = width;
	hViewport = height;
	camera.setViewport(width, height);
	glViewport(0, 0, width, height);
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

void PAG::Renderer::step()
{
	if (activeModel == -1)
		return;
	if (++activeModel >= models.size())
		activeModel = 0;
	printActiveModel();
}

void PAG::Renderer::addModel(ModelType type)
{
	if (!checkExistingModel(type))
	{
		switch (type)
		{
		case PAG::ModelType::TRIANGLE:
			models.push_back(createModel(ModelType::TRIANGLE, shaderProgram, mat));
			break;
		case PAG::ModelType::TETRAHEDRON:
			models.push_back(createModel(ModelType::TETRAHEDRON, shaderProgram, mat));
			break;
		default:
			break;
		}
		if (activeModel == -1)
			activeModel = 0;
		else if (activeModel >= 0)
			activeModel++;
		printActiveModel();
	}
	else
		Log::getInstance()->printMessage(msgType::INFO, "You added that model");
}

void PAG::Renderer::addModel(std::string filename, std::string name)
{
	if (!checkExistingModel(name))
	{
		if (name == "spurs")
			models.push_back(createModel(shaderProgramTextureNM, filename, mat, name, true));
		else
			models.push_back(createModel(shaderProgram, filename, mat, name));
		if (activeModel == -1)
			activeModel = 0;
		else if (activeModel >= 0)
			activeModel++;

		if (name == "cow")
		{
			models[activeModel]->rotate(-90, { 1,0,0 });
			models[activeModel]->move({ 2,0,0 });
		}
		if (name == "rook")
		{
			models[activeModel]->move({ 2,0,2 });
			models[activeModel]->scale(glm::vec3(.05));

		}

		printActiveModel();
	}
	else
		Log::getInstance()->printMessage(msgType::INFO, "You added that model");
}

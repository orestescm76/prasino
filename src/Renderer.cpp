/*
* @author orestescm76
* @brief renderer cpp
*/

#include "pch.h"
#include "Renderer.h"

PAG::Renderer* PAG::Renderer::instance = nullptr;
const std::string PAG::Renderer::version = "1.0.0";

PAG::Renderer::Renderer() :
	activeModel(-1),
	mat(glm::vec3(0.135, 0.2225, 0.1575), glm::vec3(0.54, 0.89, 0.63), glm::vec3(1), .1 * 128),
	qMat(glm::vec3(.1), glm::vec3(.60), glm::vec3(.7), 0.25f*128.0f),
	//mat(glm::vec3(0.0215,0.1745,0.0215),glm::vec3(0.07568,	0.61424 ,	0.07568),glm::vec3(0.633, 	0.727811 ,	0.633), .6f*128.0f),
	camera({3,3,3}, 90, .1,50,wViewport, hViewport),
	lights(),
	models(),
	textures()
{
	try
	{
		shaderProgram = std::make_unique<ShaderProgram>("vertex.glsl", "frag.glsl");
		//shaderProgramTexture = std::make_shared<ShaderProgram>("pag10-vs.glsl", "pag10-fs-tex.glsl");
		shaderProgramTextureNM = std::make_shared<ShaderProgram>("vertex.glsl", "frag.glsl");
		shaderProgramShadow = std::make_shared<ShaderProgram>("vertex_shadow.glsl", "frag_shadow.glsl");
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error("PAG::Renderer::Renderer -> " + (std::string)e.what());
	}

	
	lightCube = std::make_unique<Model>(shaderProgram, ModelType::LIGHT_CUBE);
	Light ambL(glm::vec3(.18));
	Light point(glm::vec3(.3), glm::vec3(1), glm::vec3(-.5,.5,.2), LightType::POINT);
	Light dir(glm::vec3(1,1,.6), glm::vec3(.6),glm::vec3(-.7,-.7,-.7), LightType::DIRECTIONAL);
	Light spot(glm::vec3(0,1,1), glm::vec3(1.0f), glm::vec3(3,4,3), glm::vec3(-3,-4,-3), 128.0f);

	lights.push_back(ambL);
	//lights.push_back(point);
	lights.push_back(dir);
	//lights.push_back(spot);

	backColor = { 0,0,0,1 };

	textures.insert({ "cow", std::make_shared<Texture>("./textures/vaca.png",TextureType::IMAGE) });
	textures.insert({ "grass", std::make_shared<Texture>("./textures/grass.png",TextureType::IMAGE) });
	textures.insert({ "marble", std::make_shared<Texture>("./textures/marble.png",TextureType::IMAGE) });
	textures.insert({ "spurs", std::make_shared<Texture>("./textures/tottenham.png",TextureType::IMAGE)});
	textures.insert({ "spursNM", std::make_shared<Texture>("./textures/tottenham_nm.png",TextureType::NORMAL_MAP) });
	textures.insert({ "stone", std::make_shared<Texture>("./textures/stone.png", TextureType::IMAGE) });
	textures.insert({ "stoneNM", std::make_shared<Texture>("./textures/stone_nm.png", TextureType::NORMAL_MAP) });
	textures.insert({ "polish", std::make_shared<Texture>("./textures/polish.png", TextureType::IMAGE) });

	models.push_back(std::make_unique<Model>(shaderProgram, ModelType::QUAD, qMat));
	models[0]->addTexture(textures["grass"]);
	models[0]->setDrawTexture(true);

	//Create FBO
	glGenFramebuffers(1, &fboShadowId);
	for (size_t i = 0; i < lights.size(); i++)
	{
		switch (lights[i].type)
		{
		case LightType::DIRECTIONAL:
		case LightType::SPOTLIGHT:
			createShadowMap(lights[i]);
			updateShadowMap(lights[i]);
		default:
			break;
		}
	}
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
		updateShadowMaps();
		//check if we are pointing the quad, which is not removable
		if (models[activeModel]->getType() == ModelType::QUAD)
		{
			//change the active model and if there is only quad, set to -1
			step();
			return;
		}
		printActiveModel();
	}
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
			//Check models
			if (model->name() == "cow")
				model->addTexture(textures["cow"]);
			else if (model->name() == "rook")
				model->addTexture(textures["marble"]);
			else if (model->name() == "spurs")
				model->addTexture(textures["spurs"]);
			else if(model->name() == "queen")
				model->addTexture(textures["stone"]);
			else if(model->getType() == ModelType::TETRAHEDRON || model->getType() == ModelType::TRIANGLE)
				model->addTexture(textures["polish"]);
			else if(model->name() == "knight")
				model->addTexture(textures["stone"]);
			model->setDrawTexture(true);
		}
		else
		{
			model->setDrawTexture(false);
			model->setNormalMapping(false);
			//model->setShaderProgram(shaderProgram);
			model->unBindTexture();
		}
	}
}
void PAG::Renderer::setNormalMappingToActiveModel()
{
	if (activeModel != -1)
	{
		Model* model = models[activeModel].get();
		//If it is not drawing normal mapping texture
		if (model->isDrawingTexture())
		{
			
			if (!model->isDrawingNormalMapping())
			{
				//Check models
				if (model->name() == "spurs")
				{
					//model->setShaderProgram(shaderProgramTextureNM);
					model->addTexture(textures["spursNM"]);
					model->setNormalMapping(true);
				}
				else if (model->name() == "knight")
				{
					//model->setShaderProgram(shaderProgramTextureNM);
					model->addTexture(textures["stoneNM"]);
					model->setNormalMapping(true);
				}	
			}
			else
			{
				model->setNormalMapping(false);
				model->removeNormalMapping();
				//model->setShaderProgram(shaderProgramTexture);
			}
		}
		else return;
	}
}

void PAG::Renderer::activateLight(Light& l, ShaderProgram* sp, Model* model)
{
	glm::vec3 lPos, lDir;
	glm::mat4 viewCL, projCL;
	//applied comments from practice 8
	glm::mat4 mInvTrans = glm::inverse(glm::transpose(camera.getViewMatrix()));
	glm::mat4 shadowMat = glm::scale(glm::mat4(1), glm::vec3(.5));
	shadowMat[3][0] = shadowMat[3][1] = shadowMat[3][2] = 0.5;

	switch (l.type)
	{
	case LightType::AMBIENT:
		sp->getFragmentShader().setUniformSubroutine("ambientColor", "light");
		sp->getFragmentShader().setUniformSubroutine("simpleColorAmbient", "color");
		sp->getFragmentShader().setUniform("Ia", l.ambient);
		sp->getFragmentShader().setUniform("Ka", model->getMaterial().ambient);
		break;
	//WORKS
	case LightType::POINT:
		sp->getFragmentShader().setUniformSubroutine("point", "light");
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
		viewCL = glm::lookAt(glm::vec3(5) * -l.direction, glm::vec3(0), glm::vec3(0, 1, 0));
		projCL = glm::ortho(-3.0, 3.0, -3.0, 3.0, .1, 10.0);
		shadowMat = shadowMat * projCL * viewCL * model->getModelMatrix();
		sp->getFragmentShader().setUniformSubroutine("directional", "light");
		sp->getFragmentShader().setUniform("Id", l.diffuse);
		sp->getFragmentShader().setUniform("Is", l.specular);
		//Apply transform
		glm::vec3 lDir = glm::vec3(mInvTrans * glm::vec4(l.direction, 0));
		lDir = glm::normalize(lDir);
		//lDir = glm::vec3(glm::normalize(mInvTrans * glm::vec4(l.direction, 0)));
		sp->getFragmentShader().setUniform("lDir", lDir);
		if (!model->isDrawingTexture())
			sp->getFragmentShader().setUniform("Kd", model->getMaterial().diffuse);
		sp->getFragmentShader().setUniform("Ks", model->getMaterial().specular);
		sp->getFragmentShader().setUniform("shininess", model->getMaterial().shininess);
		sp->getVertexShader().setUniform("matShadow", shadowMat);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, l.texID);
		sp->getFragmentShader().setUniform("samplerShadow", (unsigned)2);
		break;
	case LightType::SPOTLIGHT:
		viewCL = glm::lookAt(l.position, l.position + l.direction, glm::vec3(0, 1, 0));
		projCL = glm::perspective(2 * glm::radians(l.angle), 2048.0f / 2048.0f, .1f, 10.0f);
		shadowMat = shadowMat * projCL * viewCL * model->getModelMatrix();
		sp->getFragmentShader().setUniformSubroutine("spot", "light");
		sp->getFragmentShader().setUniform("sAngle", glm::radians(l.angle));
		sp->getFragmentShader().setUniform("Id", l.diffuse);
		sp->getFragmentShader().setUniform("Is", l.specular);
		//Apply transform pos
		lPos = glm::vec3(camera.getViewMatrix() * glm::vec4(l.position, 1));
		sp->getFragmentShader().setUniform("lPos", lPos);
		sp->getVertexShader().setUniform("matShadow", shadowMat);
		//Apply transform direction
		lDir = glm::normalize(camera.getViewMatrix() * glm::vec4(l.direction, 0));
		sp->getFragmentShader().setUniform("lDir", lDir);
		if (!model->isDrawingTexture())
			sp->getFragmentShader().setUniform("Kd", model->getMaterial().diffuse);
		sp->getFragmentShader().setUniform("Ks", model->getMaterial().specular);
		sp->getFragmentShader().setUniform("shininess", model->getMaterial().shininess);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, l.texID);
		sp->getFragmentShader().setUniform("samplerShadow", (unsigned)2);
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
	glm::mat4 projviewmod = camera.getProjMatrix() * camera.getViewMatrix() * glm::translate(glm::mat4(1), l.position);
	shaderProgram->useProgram();
	shaderProgram->getVertexShader().setUniform("matProjViewModel", projviewmod);
	//shaderProgram->getFragmentShader().setUniform("lightColor", l.diffuse);
	lightCube->setDrawTexture(false);
	shaderProgram->getFragmentShader().setUniform("Kd", l.diffuse);
	shaderProgram->getFragmentShader().setUniformSubroutine("wire", "light");
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
	std::string renderer = ((const char*)glGetString(GL_RENDERER));
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
		model->draw();
		if(l.type != LightType::AMBIENT)
			drawLightCube(l);
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error("PAG::Renderer::draw() -> " + (std::string)e.what());
	}
}

std::unique_ptr<PAG::Model> PAG::Renderer::createModel(ModelType type, std::unique_ptr<ShaderProgram>& sp, Material& mat)
{
	return std::make_unique<Model>(sp, type, mat);
}
std::unique_ptr<PAG::Model> PAG::Renderer::createModel(std::unique_ptr<ShaderProgram>& shaderProgram, std::string filename, Material mat, std::string name, bool NM = false)
{
	//if(!NM)
		return std::make_unique<Model>(shaderProgram, filename, mat, name);
	////else
	//{
	//	std::unique_ptr<Model> model = std::make_unique<Model>(shaderProgram, filename, mat, name);
	//	//model->setNormalMapping(true);
	//	//model->setDrawTexture(true);
	//	//model->addTexture(textures["spurs"]);
	//	//model->addTexture(textures["spursNM"]);
	//	return model;
	//}
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

void PAG::Renderer::createShadowMap(Light& l)
{
	glGenTextures(1, &l.texID);
	GLfloat borde[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//PLACEHOLDER
	GLsizei _anchoMS = 2048; 
	GLsizei _altoMS = 2048;
	glBindTexture(GL_TEXTURE_2D, l.texID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, _anchoMS, _altoMS, 0,
		GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, nullptr);
	//parameters...
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borde);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);
}

void PAG::Renderer::updateShadowMaps()
{
	for (size_t i = 0; i < lights.size(); i++)
	{
		if(lights[i].type == LightType::DIRECTIONAL || lights[i].type == LightType::SPOTLIGHT)
			updateShadowMap(lights[i]);
	}
}

void PAG::Renderer::updateShadowMap(Light& l)
{
	//Bind the texture to the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, fboShadowId);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, l.texID, 0);

	glReadBuffer(GL_NONE);
	glDrawBuffer(GL_NONE);
	//check framebuffer
	GLenum state = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (state != GL_FRAMEBUFFER_COMPLETE)
	{
		throw std::runtime_error("PAG::Renderer::createShadowMap(Light& l). Failed to update the shadow map");
	}
	//Activate texture unit
	glActiveTexture(GL_TEXTURE2);
	//Bind to FBO
	glBindTexture(GL_TEXTURE_2D, l.texID);
	//Ignore color
	glClear(GL_DEPTH_BUFFER_BIT);
	//Set viewport
	glViewport(0, 0, 2048, 2048);
	//change zbuffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	//Avoid shadow acne ???
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	//Take view and proj matrix from light
	glm::mat4 view, proj;
	if (l.type == LightType::DIRECTIONAL)
	{
		view = glm::lookAt(glm::vec3(5) * -l.direction, glm::vec3(0), glm::vec3(0, 1, 0));
		proj = glm::ortho(-3.0, 3.0, -3.0, 3.0, .1, 10.0);
	}
	else
	{
		view = glm::lookAt(l.position, l.position + l.direction, glm::vec3(0, 1, 0));
		proj = glm::perspective(2 * glm::radians(l.angle), 2048.0f / 2048.0f, .1f, 10.0f);
	}
	//use the shadow shaderprogram
	shaderProgramShadow->useProgram();
	//render the models
	for (size_t i = 0; i < models.size(); i++)
	{
		Model* model = models[i].get();
		glm::mat4 mvp = proj * view * model->getModelMatrix();
		shaderProgramShadow->getVertexShader().setUniform("matModVisProj", mvp);
		model->drawTriangles();
	}
	//back to normal
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, wViewport, hViewport);
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_CULL_FACE);
}

void PAG::Renderer::configViewport(int width, int height)
{
	wViewport = width;
	hViewport = height;
	camera.setViewport(width, height);
	glViewport(0, 0, width, height);
}

void PAG::Renderer::setRenderTypeToActiveModel(RenderType rt)
{
	if (activeModel != -1)
	{
		models[activeModel]->setDrawingMode(rt);
	}
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
	if (models.size() == 1)
	{
		activeModel = -1;
		return;
	}
	if (++activeModel >= models.size())
		activeModel = 1;
	if (models[activeModel]->getType() == ModelType::QUAD)
	{
		step();
		return;
	}
		

	printActiveModel();
}

void PAG::Renderer::addModel(std::string filename, std::string name)
{
	if (!checkExistingModel(name))
	{
		try
		{
			models.push_back(createModel(shaderProgram, filename, mat, name));
		}
		catch (const std::exception& e)
		{
			throw std::runtime_error("void PAG::Renderer::addModel(std::string filename, std::string name) -> " + (std::string)e.what());
		}

		Model* model = models.back().get();

		if (name == "cow")
		{
			model->move({ 2,.3,.5 });
			model->rotate(-90, { 1,0,0 });
			
		}
		if (name == "rook")
		{
			model->move({ 2,-.5,2 });
			model->scale(glm::vec3(.075));

		}
		if (name == "knight")
		{
			model->move({ -1,0,-1 });
			model->scale(glm::vec3(.1));
			model->rotate(270, { 1,0,0 });
			model->rotate(90, { 0,1,0 });
		}
		if (name == "spurs")
			model->move({ 0,.001,0 });
		if (activeModel == -1)
			activeModel = 1;
		else if (activeModel >= 0)
			activeModel++;
		updateShadowMaps();
		printActiveModel();
	}
	else
		Log::getInstance()->printMessage(msgType::INFO, "You added that model");

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
			activeModel = 1;
		else if (activeModel >= 0)
			activeModel++;

		updateShadowMaps();
		printActiveModel();
	}
	else
		Log::getInstance()->printMessage(msgType::INFO, "You added that model");
}
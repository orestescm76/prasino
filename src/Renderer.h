#pragma once
//Includes model and Shader Program
#include "Model.h"
#include "Log.h"
#include "Camera.h"
#include "Light.h"
namespace PAG
{
	class Renderer
	{
	private:
		static Renderer* instance;
		glm::vec4 backColor;
		std::vector<std::unique_ptr<Model>> models;
		std::unique_ptr<Model> lightCube;
		std::unique_ptr<ShaderProgram> shaderProgram;
		//std::shared_ptr<ShaderProgram> shaderProgramTexture;
		std::shared_ptr<ShaderProgram> shaderProgramTextureNM;
		std::unique_ptr<ShaderProgram> spLightCube;
		std::shared_ptr<ShaderProgram> shaderProgramShadow;
		Camera camera;
		Renderer();
		void configBackColor(glm::vec4 color);
		float wViewport, hViewport;
		RenderType renderType = RenderType::SOLID;
		Material mat;
		Material qMat;
		std::vector<Light> lights;
		//Activate the light and set the uniforms with the model shader program
		void activateLight(Light& l, ShaderProgram* sp, Model* model);
		void loadUniforms(ShaderProgram* sp, Model* model);
		void drawLightCube(Light& l);
		void draw(Light& l, Model* model);
		std::unique_ptr<Model> createModel(ModelType type, std::unique_ptr<ShaderProgram>& sp, Material& mat);
		std::unique_ptr<Model> createModel(std::unique_ptr<ShaderProgram>& shaderProgram, std::string filename, Material mat, std::string name, bool NM);
		int activeModel;
		bool checkExistingModel(ModelType type);
		bool checkExistingModel(std::string name);
		std::map<std::string, std::shared_ptr<Texture>> textures;
		unsigned int fboShadowId = 0;
		void createShadowMap(Light& l);
		void updateShadowMap(Light& l);
	public:
		static const std::string version;
		// Lo único que necesito estático de la clase Renderer es la instancia y el método de la instancia
		// ya que así o creo una instancia y hago render->getInstancia() o PAG::Renderer::getInstancia(), lo mismo da.
		static Renderer* getInstance(); 
		void refreshWindow();
		void changeColor(double yoffset);
		void start();
		void activeZBuffer();
		void addColor();
		void minusColor();
		void configViewport(int width, int height);
		void printInfo();
		void setRenderTypeToActiveModel(RenderType rt);
		void moveCamera(int key);
		void moveCamera(float xoffset, float yoffset);
		void setCamera(MovType mov);
		void resetCamera();
		void zoomCamera(float yoffset);
		void step();
		void addModel(ModelType type);
		void addModel(std::string filename, std::string name);
		void printActiveModel();
		void deleteActiveModel();
		void setTextureToActiveModel();
		void setNormalMappingToActiveModel();
		void updateShadowMaps();
		virtual ~Renderer();
	};
}

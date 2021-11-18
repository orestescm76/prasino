#pragma once
#include "ShaderProgram.h"
#include "Material.h"
namespace PAG
{
	enum class ModelType
	{
		TRIANGLE,
		TETRAHEDRON,
		LIGHT_CUBE
	};

	enum class RenderType
	{
		SOLID,
		WIRE
	};

	class Model
	{
		//EEDD
		std::vector<float> vertices;
		std::vector<float> normals;
		std::vector<unsigned int> indices;
		//IDENTIFICADORES
		unsigned int idVAO = 0;
		unsigned int idVBO = 0;
		unsigned int idColor = 0;
		unsigned int idIBO = 0;
		unsigned int idNormalVBO = 0;
		//Pointer to the shader program
		std::shared_ptr<ShaderProgram> sp;
		//Create models
		void createTriangle();
		void createTetrahedron();
		void createLightCube();
		//Other data
		RenderType renderType = RenderType::SOLID;
		ModelType modelType;
		Material material;
	public:
		Model();
		Model(GLfloat* v, GLfloat* c, GLuint* i, std::shared_ptr<ShaderProgram>& shaderProgram);
		Model(std::shared_ptr<ShaderProgram> shaderProgram, ModelType model, Material& m);
		Model(std::shared_ptr<ShaderProgram> shaderProgram, ModelType model);
		Model(const Model& model);

		void initModel();
		void draw();
		void setDrawingMode(RenderType mode);
		void useProgram();

		Material getMaterial();
		ShaderProgram* getShaderProgram();
		virtual ~Model();
	};
}
#pragma once
#include "ShaderProgram.h"
#include "Material.h"
namespace PAG
{
	enum class ModelType
	{
		TRIANGLE,
		TETRAHEDRON,
		EXTERN,
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
		//Model matrix
		glm::mat4 modelMatrix;
		//Create models
		void createTriangle();
		void createTetrahedron();
		void createLightCube();
		//Other data
		RenderType renderType = RenderType::SOLID;
		ModelType modelType;
		Material material;
		std::string mName;
		void processNode(aiNode* node, const aiScene* scene);
		void processMesh(aiMesh* mesh, const aiScene* scene);
	public:
		Model();
		Model(GLfloat* v, GLfloat* c, GLuint* i, std::shared_ptr<ShaderProgram>& shaderProgram);
		Model(std::shared_ptr<ShaderProgram> shaderProgram, ModelType model, Material& m);
		Model(std::shared_ptr<ShaderProgram> shaderProgram, ModelType model);
		Model(std::shared_ptr<ShaderProgram> shaderProgram, std::string filename, Material mat, std::string name);
		Model(const Model& model);
		Model& operator=(const Model& orig) = default;

		void initModel();
		void draw();
		void setDrawingMode(RenderType mode);
		void useProgram();

		Material getMaterial();
		ShaderProgram* getShaderProgram();
		glm::mat4 getModelMatrix();
		std::string name();
		virtual ~Model();
	};
}
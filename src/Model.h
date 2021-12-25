#pragma once
#include "ShaderProgram.h"
#include "Material.h"
#include "Texture.h"
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
		std::vector<float> texCoords;
		std::vector<float> tangents;
		std::vector<float> biTangents;
		//IDENTIFICADORES
		unsigned int idVAO = 0;
		unsigned int idVBO = 0;
		unsigned int idColor = 0;
		unsigned int idIBO = 0;
		unsigned int idNormalVBO = 0;
		unsigned int idVBOTex = 0;
		unsigned int idTangent = 0;
		unsigned int idBiTangent = 0;
		//Pointer to the shader program
		std::shared_ptr<ShaderProgram> sp;
		//pointer to a texture
		std::vector<std::shared_ptr<Texture>> textures;
		bool drawTexture = false;
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
		glm::vec3 position = {0,0,0};
		void initModel();
		bool normalMapping = false;
	public:
		Model();
		Model(GLfloat* v, GLfloat* c, GLuint* i, std::shared_ptr<ShaderProgram>& shaderProgram);
		Model(std::shared_ptr<ShaderProgram> shaderProgram, ModelType model, Material& m);
		Model(std::shared_ptr<ShaderProgram> shaderProgram, ModelType model);
		Model(std::shared_ptr<ShaderProgram> shaderProgram, std::string filename, Material mat, std::string name);
		Model(const Model& model);
		Model& operator=(const Model& orig) = default;

		void draw();
		void setDrawingMode(RenderType mode);
		void useProgram();
		void addTexture(std::shared_ptr<Texture>& tex);
		void deleteTexutres();
		void setShaderProgram(std::shared_ptr<ShaderProgram>& shaderProgram);
		void setDrawTexture(bool flag);
		Material getMaterial();
		ShaderProgram* getShaderProgram();
		glm::mat4 getModelMatrix();
		std::string name();
		ModelType getType();
		bool isDrawingTexture();
		bool isDrawingNormalMapping();
		void setNormalMapping(bool nm);
		//Transformations
		//Translate. This doesn't translate to the origin.
		void move(glm::vec3 pos);
		//Rotate. This translates to the origin, rotates and then translates.
		void rotate(float deg, glm::vec3 axis);
		//Scale.
		void scale(glm::vec3 vec);
		//Translate. Translates to the origin and then translates to the position of choice.
		void translate(glm::vec3 pos);
		//Resets the model matrix.
		void resetMatrix();
		void removeNormalMapping();
		void unBindTexture();
		virtual ~Model();
	};
}
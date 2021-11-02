#pragma once
#include "ShaderProgram.h"
#include "Material.h"
namespace PAG
{
	enum class ModelType
	{
		TRIANGLE,
		TETRAHEDRON
	};

	enum class RenderType
	{
		SOLID,
		WIRE
	};

	class Model
	{
	protected:
		//EEDD
		std::vector<GLfloat> vertices;
		std::vector<GLuint> indices;
		//IDENTIFICADORES
		GLuint idVAO = 0;
		GLuint idVBO = 0;
		GLuint idColor = 0;
		GLuint idIBO = 0;
		//Pointer to the shader program
		std::shared_ptr<ShaderProgram> sp;
		//Create models
		void createTriangle();
		void createTetrahedron();
		//Other data
		GLenum drawingMode = GL_FILL;
		ModelType modelType;
		Material material;
	public:
		Model();
		Model(GLfloat* v, GLfloat* c, GLuint* i, std::shared_ptr<ShaderProgram>& shaderProgram);
		Model(std::shared_ptr<ShaderProgram> shaderProgram, ModelType model, Material& m);
		Model(const Model& model);

		void initModel();
		void draw();
		void setDrawingMode(RenderType mode);
		void useProgram();

		Material getMaterial();
		virtual ~Model();
	};
}
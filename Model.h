#pragma once
#include "ShaderProgram.h"
namespace PAG
{
	enum class Models
	{
		TRIANGLE,
		TETRAHEDRON
	};
	class Model
	{
	protected:
		//GLfloat* vertices;
		//GLfloat* colors;
		//GLuint* indices;
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
		GLenum drawingMode = GL_FILL;
		Models modelType;
	public:
		Model();
		Model(GLfloat* v, GLfloat* c, GLuint* i, std::shared_ptr<ShaderProgram>& shaderProgram);
		Model(std::shared_ptr<ShaderProgram> shaderProgram, Models model);
		Model(const Model& model);

		void initModel();
		void draw();
		void setDrawingMode(GLenum mode);
		virtual ~Model();
	};
}
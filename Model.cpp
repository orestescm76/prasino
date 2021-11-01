#include "pch.h"
#include "Model.h"
#include "Log.h"

PAG::Model::Model()
{

}

PAG::Model::Model(std::shared_ptr<ShaderProgram> shaderProgram, Models model): sp(shaderProgram), modelType(model)
{
	switch (modelType)
	{
	case PAG::Models::TRIANGLE:
		createTriangle();
		break;
	case PAG::Models::TETRAHEDRON:
		createTetrahedron();
		break;
	default:
		break;
	}
	initModel();
}



PAG::Model::Model(GLfloat* v, GLfloat* c, GLuint* i, std::shared_ptr<ShaderProgram>& shaderProgram): sp(shaderProgram)
{
	initModel();
}
//creo que no es lo mejor
PAG::Model::Model(const Model& model): sp(model.sp)
{
	initModel();
}

void PAG::Model::createTriangle()
{
	/*
	 * Το τρίγωνο είναι ένα από τα βασικά σχήματα στην γεωμετρία.
	 */
	//vertices
	vertices = { -.5, -.5, 0,
		.5, -.5, 0,
		.0,  .5, 0 };
	//indices para pintarlo
	indices = { 0, 1, 2 };
	////colores de los vertices
	//GLfloat colorsArr[9] = { 0.0f, 0.733f, 0.176f,
	//					0.835f, 0.188f, 0.196f,
	//	//0.678f, 0.847f, 0.902f };
	//	0.114f, 0.118f, 0.2f };

}

void PAG::Model::createTetrahedron()
{
	/*
	* Τετράεδρο
	* Είναι το πολύεδρο που έχει τέσσερις έδρες, δηλαδή η τριγωνική πυραμίδα.
	*/
	vertices = { 0, 1, 0,
				1, 0, 0,
				0, 0, 1,
				0, 0, 0 };
	indices = { 0, 3, 1,
				0, 1, 2,
				0, 2, 3,
				1, 3, 2 };
}

void PAG::Model::initModel()
{
	//generamos el vao y la vinculamos
	Log::getInstance()->printMessage(msgType::INFO, "Creating VAO");
	glGenVertexArrays(1, &idVAO);
	glBindVertexArray(idVAO);
	//SIN ENTRELAZAR
	Log::getInstance()->printMessage(msgType::INFO, "Creating VBO");
	glGenBuffers(1, &idVBO);
	glBindBuffer(GL_ARRAY_BUFFER, idVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(),
		GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
		nullptr);
	glEnableVertexAttribArray(0);
	//indices
	Log::getInstance()->printMessage(msgType::INFO, "Creating IBO");
	glGenBuffers(1, &idIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(),
		GL_STATIC_DRAW);
}

void PAG::Model::draw()
{
	sp->useProgram();
	glBindVertexArray(idVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO);
	glPolygonMode(GL_FRONT_AND_BACK, drawingMode);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
}

void PAG::Model::setDrawingMode(GLenum mode)
{
	drawingMode = mode;
}

PAG::Model::~Model()
{
	if(idVBO != 0)
		glDeleteBuffers(1, &idVBO);
	if(idColor != 0)
		glDeleteVertexArrays(1, &idColor);
	if(idIBO != 0)
		glDeleteBuffers(1, &idIBO);
	if(idVAO != 0)
		glDeleteVertexArrays(1, &idVAO);
	//std::vector destroys on its own
}

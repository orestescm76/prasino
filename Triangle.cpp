#include "pch.h"
#include "Triangle.h"
#include "Log.h"

PAG::Triangle::Triangle(): Model()
{
	//Copy the model data
	vertices = verticesArr;
	indices = indicesArr;
	colors = colorsArr;
}

PAG::Triangle::Triangle(std::shared_ptr<ShaderProgram>& shaderProgram): Model(shaderProgram)
{
	//Copy the model data
	vertices = verticesArr;
	indices = indicesArr;
	colors = colorsArr;

	initModel();
}

void PAG::Triangle::initModel()
{
	//generamos el vao y la vinculamos
	Log::getInstance()->printMessage(msgType::INFO, "Creating triangle VAO");
	glGenVertexArrays(1, &idVAO);
	glBindVertexArray(idVAO);
	//SIN ENTRELAZAR
	Log::getInstance()->printMessage(msgType::INFO, "Creating triangle VBO");
	glGenBuffers(1, &idVBO);
	glBindBuffer(GL_ARRAY_BUFFER, idVBO);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), vertices,
		GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
		nullptr);
	glEnableVertexAttribArray(0);
	//generamos el VBO de los colores, pongo 1 para luego especificarlo en el layout del shader	
	glGenBuffers(1, &idColor);
	glBindBuffer(GL_ARRAY_BUFFER, idColor);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), nullptr);
	glEnableVertexAttribArray(1);
	//indices
	Log::getInstance()->printMessage(msgType::INFO, "Creating triangle IBO");
	glGenBuffers(1, &idIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(GLuint), indices,
		GL_STATIC_DRAW);
}

PAG::Triangle::~Triangle()
{
	if (idVBO != 0)
		glDeleteVertexArrays(1, &idVBO);
	if (idColor != 0)
		glDeleteVertexArrays(1, &idColor);
	if (idIBO != 0)
		glDeleteVertexArrays(1, &idIBO);
	if (idVAO != 0)
		glDeleteVertexArrays(1, &idVAO);
}


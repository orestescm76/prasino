#include "pch.h"
#include "Tetrahedron.h"
#include "Log.h"

PAG::Tetrahedron::Tetrahedron()
{
	//Copy the model data
	vertices = verticesArr;
	indices = indicesArr;
}

PAG::Tetrahedron::Tetrahedron(std::shared_ptr<ShaderProgram>& shaderProgram): Model(shaderProgram)
{
	//Copy the model data
	vertices = verticesArr;
	indices = indicesArr;

	initModel();
}

void PAG::Tetrahedron::initModel()
{
	//generamos el vao y la vinculamos
	Log::getInstance()->printMessage(msgType::INFO, "Creating tetrahedron VAO");
	glGenVertexArrays(1, &idVAO);
	glBindVertexArray(idVAO);
	//SIN ENTRELAZAR
	Log::getInstance()->printMessage(msgType::INFO, "Creating tetrahedron VBO");
	glGenBuffers(1, &idVBO);
	glBindBuffer(GL_ARRAY_BUFFER, idVBO);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), vertices,
		GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
		nullptr);
	glEnableVertexAttribArray(0);
	//indices
	Log::getInstance()->printMessage(msgType::INFO, "Creating tetrahedron IBO");
	glGenBuffers(1, &idIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12 * sizeof(GLuint), indices,
		GL_STATIC_DRAW);
}

void PAG::Tetrahedron::draw()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	sp->useProgram();
	//PAG::Log::getInstance()->printMessage(PAG::msgType::INFO, sp.use_count());
	glBindVertexArray(idVAO);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);
}

PAG::Tetrahedron::~Tetrahedron()
{
	if (idVBO != 0)
		glDeleteVertexArrays(1, &idVBO);
	if (idIBO != 0)
		glDeleteVertexArrays(1, &idIBO);
	if (idVAO != 0)
		glDeleteVertexArrays(1, &idVAO);
}

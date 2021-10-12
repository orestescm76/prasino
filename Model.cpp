#include "pch.h"
#include "Model.h"
#include "Log.h"
PAG::Model::Model(): vertices(nullptr), indices(nullptr), colors(nullptr)
{

}

PAG::Model::Model(GLfloat* v, GLfloat* c, GLuint* i, std::string filevs, std::string filefs): vertices(v), colors(c), indices(i), sp(filevs, filefs)
{
	initModel();
}
//creo que no es lo mejor
PAG::Model::Model(const Model& model): vertices(model.vertices), colors(model.colors), indices(model.indices), sp(model.sp)
{
	initModel();
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
	Log::getInstance()->printMessage(msgType::INFO, "Creating IBO");
	glGenBuffers(1, &idIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(GLuint), indices,
		GL_STATIC_DRAW);
}

void PAG::Model::draw()
{
	sp.useProgram();
	glBindVertexArray(idVAO);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
}

void PAG::Model::erase()
{
	sp.deactivate();
	glBindVertexArray(0);
	
}

PAG::Model::~Model()
{
	if(idVBO != 0)
		glDeleteVertexArrays(1, &idVBO);
	if(idColor != 0)
		glDeleteVertexArrays(1, &idColor);
	if(idIBO != 0)
		glDeleteVertexArrays(1, &idIBO);
	if(idVAO != 0)
		glDeleteVertexArrays(1, &idVAO);
}

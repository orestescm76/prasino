#include "pch.h"
#include "Model.h"
#include "Log.h"


PAG::Model::Model(std::shared_ptr<ShaderProgram> shaderProgram): sp(shaderProgram)
{
	vertices = nullptr;
	colors = nullptr;
	indices = nullptr;
}

PAG::Model::Model()
{
	vertices = nullptr;
	colors = nullptr;
	indices = nullptr;
}

PAG::Model::Model(GLfloat* v, GLfloat* c, GLuint* i, std::shared_ptr<ShaderProgram>& shaderProgram): vertices(v), colors(c), indices(i), sp(shaderProgram)
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


}

void PAG::Model::draw()
{
	sp->useProgram();
	glBindVertexArray(idVAO);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
}

PAG::Model::~Model()
{
	//if(idVBO != 0)
	//	glDeleteVertexArrays(1, &idVBO);
	//if(idColor != 0)
	//	glDeleteVertexArrays(1, &idColor);
	//if(idIBO != 0)
	//	glDeleteVertexArrays(1, &idIBO);
	//if(idVAO != 0)
	//	glDeleteVertexArrays(1, &idVAO);
	//delete[] vertices;
	//delete[] colors;
	//delete[] indices;
}

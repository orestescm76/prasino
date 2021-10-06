#include "Model.h"

PAG::Model::Model()
{
}

PAG::Model::~Model()
{
	glDeleteVertexArrays(1, &idVBO);
	glDeleteVertexArrays(1, &idColor);
	glDeleteVertexArrays(1, &idIBO);
	glDeleteVertexArrays(1, &idVAO);
}

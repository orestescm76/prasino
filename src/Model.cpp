#include "pch.h"
#include "Model.h"
#include "Log.h"

PAG::Model::Model()
{

}

PAG::Model::Model(std::shared_ptr<ShaderProgram> shaderProgram, ModelType model, Material& m): sp(shaderProgram), modelType(model), material(m),
vertices(), normals(), indices()
{
	switch (modelType)
	{
	case PAG::ModelType::TRIANGLE:
		createTriangle();
		break;
	case PAG::ModelType::TETRAHEDRON:
		createTetrahedron();
		break;
	case PAG::ModelType::LIGHT_CUBE:
		createLightCube();
		break;
	default:
		break;
	}
	initModel();
}

PAG::Model::Model(std::shared_ptr<ShaderProgram> shaderProgram, ModelType model) : sp(shaderProgram), modelType(model),
vertices(), normals(), indices()
{
	switch (modelType)
	{
	case PAG::ModelType::TRIANGLE:
		createTriangle();
		break;
	case PAG::ModelType::TETRAHEDRON:
		createTetrahedron();
		break;
	case PAG::ModelType::LIGHT_CUBE:
		createLightCube();
		break;
	default:
		break;
	}
	initModel();
}

PAG::Model::Model(GLfloat* v, GLfloat* c, GLuint* i, std::shared_ptr<ShaderProgram>& shaderProgram): sp(shaderProgram),
vertices(), normals(), indices()
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
		.0,  .5, 0,
		-.5, -.5, 0,
		.5, -.5, 0,
		.0,  .5, 0 };
	//indices para pintarlo
	indices = { 0, 1, 2,
				3, 4, 5};

	normals = {0,0,1,
				0,0,1,
				0,0,1,
				0,0,-1,
				0,0,-1,
				0,0,-1 };
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
				0, 0, 0,

				0, 1, 0,
				0, 0, 0,
				0, 0, 1,
				
				0, 0, 1,
				0, 0, 0,
				1, 0, 0,
					
				0, 1, 0,
				0, 0, 1,
				1, 0, 0 };
	
	indices = {0,1,2,
				3,4,5,
				6,7,8,
				9,10,11};

	normals = { 0, 0, -1,
				0, 0, -1,
				0, 0, -1,

				-1, 0, 0,
				-1, 0, 0,
				-1, 0, 0,
				
				0, -1, 0,
				0, -1, 0,
				0, -1, 0,

				1,1,1,
				1,1,1,
				1,1,1
	};
}

void PAG::Model::createLightCube()
{
	vertices = {
		-0.1f, -0.1f,  0.1f,
		-0.1f, -0.1f, -0.1f,
		 0.1f, -0.1f, -0.1f,
		 0.1f, -0.1f,  0.1f,
		-0.1f,  0.1f,  0.1f,
		-0.1f,  0.1f, -0.1f,
		 0.1f,  0.1f, -0.1f,
		 0.1f,  0.1f,  0.1f };

	indices = {
		0, 1, 2,
		0, 2, 3,
		0, 4, 7,
		0, 7, 3,
		3, 7, 6,
		3, 6, 2,
		2, 6, 5,
		2, 5, 1,
		1, 5, 4,
		1, 4, 0,
		4, 5, 6,
		4, 6, 7 };
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
	//normales, si hubiera
	if (normals.size() != 0)
	{
		Log::getInstance()->printMessage(msgType::INFO, "Creating Normals VBO");
		glGenBuffers(1, &idNormalVBO);
		glBindBuffer(GL_ARRAY_BUFFER, idNormalVBO);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), normals.data(),
			GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
			nullptr);
		glEnableVertexAttribArray(1);
	}
	//indices
	Log::getInstance()->printMessage(msgType::INFO, "Creating IBO");
	glGenBuffers(1, &idIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(),
		GL_STATIC_DRAW);
}

void PAG::Model::draw()
{
	glBindVertexArray(idVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO);
	glPolygonMode(GL_FRONT_AND_BACK, drawingMode);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
}

void PAG::Model::setDrawingMode(PAG::RenderType mode)
{
	switch (mode)
	{
	case PAG::RenderType::SOLID:
		drawingMode = GL_FILL;
		break;
	case PAG::RenderType::WIRE:
		sp->getFragmentShader().setUniformSubroutine("", "wire");
		drawingMode = GL_LINE;
		break;
	default:
		break;
	}
}

PAG::Material PAG::Model::getMaterial()
{
	return material;
}

void PAG::Model::useProgram()
{
	sp->useProgram();
}

PAG::Model::~Model()
{
	if(idVBO != 0)
		glDeleteBuffers(1, &idVBO);
	if(idColor != 0)
		glDeleteBuffers(1, &idColor);
	if(idIBO != 0)
		glDeleteBuffers(1, &idIBO);
	if(idVAO != 0)
		glDeleteVertexArrays(1, &idVAO);
	if(idNormalVBO != 0)
		glDeleteBuffers(1, &idNormalVBO);
	//std::vector destroys on its own
}

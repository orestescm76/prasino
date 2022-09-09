#include "pch.h"
#include "Model.h"
#include "Log.h"

void PAG::Model::processNode(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh, scene);
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

void PAG::Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		vertices.push_back(mesh->mVertices[i].x);
		vertices.push_back(mesh->mVertices[i].y);
		vertices.push_back(mesh->mVertices[i].z);
		if (mesh->HasNormals())
		{
			normals.push_back(mesh->mNormals[i].x);
			normals.push_back(mesh->mNormals[i].y);
			normals.push_back(mesh->mNormals[i].z);
		}
		if (mesh->mTextureCoords[0])
		{
			texCoords.push_back(mesh->mTextureCoords[0][i].x);
			texCoords.push_back(mesh->mTextureCoords[0][i].y);
		}
		else
		{
			texCoords.push_back(0.0f);
			texCoords.push_back(0.0f);
		}
		if (mesh->mTangents)
		{
			tangents.push_back(mesh->mTangents[i].x);
			tangents.push_back(mesh->mTangents[i].y);
			tangents.push_back(mesh->mTangents[i].z);

			biTangents.push_back(mesh->mBitangents[i].x);
			biTangents.push_back(mesh->mBitangents[i].x);
			biTangents.push_back(mesh->mBitangents[i].x);
		}
	}
	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
	//modelMatrix = glm::scale(modelMatrix, glm::vec3(1));
	
}

PAG::Model::Model()
{

}

PAG::Model::Model(std::unique_ptr<ShaderProgram>& shaderProgram, ModelType model, Material& m) : sp(shaderProgram.get()), modelType(model), material(m),
vertices(), normals(), indices(), modelMatrix(1), mName(), textures()
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
	case PAG::ModelType::QUAD:
		createQuad();
	default:
		break;
	}
	initModel();
}

PAG::Model::Model(std::unique_ptr<ShaderProgram>& shaderProgram, ModelType model) : sp(shaderProgram.get()), modelType(model),
vertices(), normals(), indices(), modelMatrix(1), mName(), textures()
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
	case PAG::ModelType::QUAD:
		createQuad();
	default:
		break;
	}
	initModel();
}

PAG::Model::Model(std::unique_ptr<ShaderProgram>& shaderProgram, std::string filename, Material mat, std::string name): sp(shaderProgram.get()), modelType(ModelType::EXTERN), modelMatrix(1),
normals(), vertices(), indices(), material(mat), mName(name), textures()
{
	Log::getInstance()->printMessage(msgType::INFO, "Loading " + filename);
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		Log::getInstance()->printMessage(msgType::ERROR, "Failed to load the model! Maybe I couldn't find it?");
		throw std::runtime_error("Failed to load the model");
	}
	processNode(scene->mRootNode, scene);
	Log::getInstance()->printMessage(msgType::OK, "Loaded " + filename);
	Log::getInstance()->printMessage(msgType::INFO, "Num vertices: " + std::to_string(vertices.size()));
	Log::getInstance()->printMessage(msgType::INFO, "Num triangles: " + std::to_string(indices.size()/3));
	Log::getInstance()->printMessage(msgType::INFO, "Num texcoords: " + std::to_string(texCoords.size() / 2));
	initModel();
}

PAG::Model::Model(GLfloat* v, GLfloat* c, GLuint* i, std::shared_ptr<ShaderProgram>& shaderProgram): sp(shaderProgram.get()),
vertices(), normals(), indices(), modelMatrix(1), mName(), textures()
{
	initModel();
}
//creo que no es lo mejor
PAG::Model::Model(const Model& model): sp(model.sp), modelMatrix(model.modelMatrix), mName(model.mName)
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

	texCoords = { 1.0f, 0.0f,
				  0.0f, 0.0f,
				  0.5f, 1.0f,
				1.0f, 0.0f,
				  0.0f, 0.0f,
				  0.5f, 1.0f
	};
	mName = "Triangle";
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
	vertices = { 0.0f, 1.0f, 0.0f,
				1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f,

				0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 1.0f,
				0.0f, 0.0f, 0.0f,
				
				0.0f, 0.0f, 1.0f,
				1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f,
				
				0.0f, 1.0f, 0.0f,
				1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f };

	texCoords = { 1.0f, 1.0f,
				 0.0f, 0.0f,
				  1.0f, 0.0f,
				

				0.0f,1.0f,
				1.0f,0.0f,
				0.0f,0.0f,



				0.0f,0.0f,
				1.0f,0.0f,
				0.0f,1.0f,
				.5f,1.0f,
				1.0f,0.0f,
				0.0f,0.0f,

	};
	indices = {0,1,2,
				3,4,5,
				6,7,8,
				9,10,11};

	glm::vec3 aux(1, 1, 1);
	aux = glm::normalize(aux);

	normals = { 0.0f, 0.0f, -1.0f,
				0.0f, 0.0f, -1.0f,
				0.0f, 0.0f, -1.0f,

				-1.0f, 0.0f, 0.0f,
				-1.0f, 0.0f, 0.0f,
				-1.0f, 0.0f, 0.0f,
				
				0.0f, -1.0f, 0.0f,
				0.0f, -1.0f, 0.0f,
				0.0f, -1.0f, 0.0f,

				aux.x,aux.y,aux.z,
				aux.x,aux.y,aux.z,
				aux.x,aux.y,aux.z
	};
	mName = "Tetrahedron";
	//vertices = { 0, 1, 0,
	//		1, 0, 0,
	//		0, 0, 1,
	//		0, 0, 0 };
	//indices = { 0, 1, 3,
	//            0, 3, 2,
	//            3, 1, 2,
	//            0, 2, 1 };
	//normals = {0,1,0,
	//		   1,0,0,
	//			0,0,1,
	//			-1,-1,-1};
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

void PAG::Model::createQuad()
{
	/*
	* Τετράγωνο
	*/
 //vertices
	vertices = { 25, -.5, 25,
				-25, -.5, 25,
				-25, -.5, -25,
				25, -.5,  25,
				-25, -.5, -25,
				25, -.5, -25};
	//indices para pintarlo
	indices = { 0, 1, 2,
				3, 4, 5 };

	normals = { 0,1,0,
				0,1,0, 
				0,1,0, 
				0,1,0, 
				0,1,0, 
				0,1,0};

	texCoords = { 25, 0,
				  0, 0,
				  0, 25,
				25, 0,
				  0, 25,
				  25,25
	};
	mName = "Quad";
}

void PAG::Model::initModel()
{
	//genero la matriz de translado al 0,0,0
	modelMatrix = glm::mat4(1);
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
	if (normals.size() >=3)
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
	if (texCoords.size() >= 2)
	{
		// Creación de un VBO para las coordenadas de textura. Atributo 2
		Log::getInstance()->printMessage(msgType::INFO, "Creating Texcoords VBO");
		glGenBuffers(1, &idVBOTex);
		glBindBuffer(GL_ARRAY_BUFFER, idVBOTex);
		glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(GLfloat), texCoords.data(), 
			GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat),
			nullptr);
		glEnableVertexAttribArray(2);
	}
	if (tangents.size() >= 3)
	{
		Log::getInstance()->printMessage(msgType::INFO, "Creating Tangents VBO");
		glGenBuffers(1, &idTangent);
		glBindBuffer(GL_ARRAY_BUFFER, idTangent);
		glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(GLfloat), tangents.data(),
			GL_STATIC_DRAW);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
			nullptr);
		glEnableVertexAttribArray(3);
	}
	if (biTangents.size() >= 3)
	{
		Log::getInstance()->printMessage(msgType::INFO, "Creating BiTangents VBO");
		glGenBuffers(1, &idBiTangent);
		glBindBuffer(GL_ARRAY_BUFFER, idBiTangent);
		glBufferData(GL_ARRAY_BUFFER, biTangents.size() * sizeof(GLfloat), biTangents.data(),
			GL_STATIC_DRAW);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
			nullptr);
		glEnableVertexAttribArray(4);
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
	for (size_t i = 0; i < textures.size(); i++)
	{
		if (drawTexture)
		{
			//sp->getFragmentShader().setUniformSubroutine("", "textured");
			sp->getFragmentShader().setUniform("texSampler", textures[i]->getTexID());
			GLuint idLuz = glGetSubroutineUniformLocation(sp->getIdSP(), GL_FRAGMENT_SHADER, "light");
			GLuint idCol = glGetSubroutineUniformLocation(sp->getIdSP(), GL_FRAGMENT_SHADER, "color");
			sp->subroutineIndex[idLuz] = glGetSubroutineIndex(sp->getIdSP(), GL_FRAGMENT_SHADER, "ambientColor");
			sp->subroutineIndex[idCol] = glGetSubroutineIndex(sp->getIdSP(), GL_FRAGMENT_SHADER, "textured");
			std::cout << "idLuz: " << idLuz << "\nidCol: " << idCol << "\nambientColor: " << glGetSubroutineIndex(sp->getIdSP(), GL_FRAGMENT_SHADER, "ambientColor") << 
				"\ntextured: " << glGetSubroutineIndex(sp->getIdSP(), GL_FRAGMENT_SHADER, "textured") << "\n";
			sp->getFragmentShader().setUniformSubroutine(sp->subroutineIndex, 2);
			textures[i]->activate();
			if (normalMapping)
			{
				i++;
				sp->getFragmentShader().setUniform("texSamplerNM", textures[i]->getTexID());
				textures[i]->activate();
			}
		}

	}
	//Bind the vertices and indices
	glBindVertexArray(idVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO);
	//Check how do we render.
	switch (renderType)
	{
	case RenderType::WIRE:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//sp->getFragmentShader().setUniformSubroutine("", "wire");
		break;
	case RenderType::SOLID:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	}

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
}
void PAG::Model::drawTriangles()
{
	//Bind the vertices and indices
	glBindVertexArray(idVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
}
void PAG::Model::setDrawingMode(PAG::RenderType mode)
{
	renderType = mode;
}

void PAG::Model::addTexture(std::shared_ptr<Texture>& tex)
{
	textures.push_back(tex);
}

void PAG::Model::deleteTexutres()
{
	textures.clear();
}

PAG::Material PAG::Model::getMaterial()
{
	return material;
}

PAG::ShaderProgram* PAG::Model::getShaderProgram()
{
	return sp;
}

glm::mat4 PAG::Model::getModelMatrix()
{
	return modelMatrix;
}

std::string PAG::Model::name()
{
	return mName;
}

PAG::ModelType PAG::Model::getType()
{
	return modelType;
}

bool PAG::Model::isDrawingTexture()
{
	return drawTexture;
}

bool PAG::Model::isDrawingNormalMapping()
{
	return normalMapping;
}

void PAG::Model::setNormalMapping(bool nm)
{
	normalMapping = nm;
}

void PAG::Model::move(glm::vec3 pos)
{
	modelMatrix = glm::translate(modelMatrix, pos);
	position += pos;

}

void PAG::Model::rotate(float deg, glm::vec3 axis)
{
	modelMatrix *= glm::rotate(glm::mat4(1), glm::radians(deg), axis);
	//modelMatrix = glm::translate(modelMatrix, -position);
	//
	//modelMatrix = glm::translate(modelMatrix, position);
}

void PAG::Model::scale(glm::vec3 vec)
{
	modelMatrix = glm::scale(modelMatrix, vec);
}

void PAG::Model::translate(glm::vec3 pos)
{
	modelMatrix = glm::translate(modelMatrix, -position);
	modelMatrix = glm::translate(modelMatrix, pos);
	position = pos;
}

void PAG::Model::useProgram()
{
	sp->useProgram();
}

void PAG::Model::setShaderProgram(std::unique_ptr<ShaderProgram>& shaderProgram)
{
	sp = shaderProgram.get();
}

void PAG::Model::setDrawTexture(bool flag)
{
	drawTexture = flag;
}

void PAG::Model::resetMatrix()
{
	modelMatrix = glm::mat4(1);
}

void PAG::Model::removeNormalMapping()
{
	auto it = textures.begin();
	it++;
	//iterator points to the normal map
	textures.erase(it);
}

void PAG::Model::unBindTexture()
{
	for (size_t i = 0; i < textures.size(); i++)
	{
		if (textures[i].get())
		{
			textures[i].reset();
		}
	}
	textures.clear();
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
	if (idVBOTex != 0)
		glDeleteBuffers(1, &idVBOTex);
	if (idTangent != 0)
		glDeleteBuffers(1, &idTangent);
	if (idBiTangent != 0)
		glDeleteBuffers(1, &idBiTangent);
	//std::vector destroys on its own
}

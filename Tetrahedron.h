#pragma once
#include "Model.h"
/*
* Τετράεδρο
* Είναι το πολύεδρο που έχει τέσσερις έδρες, δηλαδή η τριγωνική πυραμίδα. 
*/
namespace PAG
{
	class Tetrahedron : public Model
	{
		GLfloat verticesArr[12] = { 0, 1, 0,
								   1, 0, 0,
								   0, 0, 1,
								   0, 0, 0 };
		//indices para pintarlo
		GLuint indicesArr[12] = { 0, 3, 1,
								 0, 1, 2,
								 0, 2, 3,
								 1, 3, 2};
		//colores de los vertices
		//GLfloat colorsArr[9] = { 0.0f, 0.733f, 0.176f,
		//					0.835f, 0.188f, 0.196f,
		//	//0.678f, 0.847f, 0.902f };
		//	0.114f, 0.118f, 0.2f };
	public:
		Tetrahedron();
		Tetrahedron(std::shared_ptr<ShaderProgram>& shaderProgram);
		void initModel();
		void draw();
		~Tetrahedron();

	};
}



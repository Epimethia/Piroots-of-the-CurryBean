// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// Author		:	James Devenport
// Email		:	james.dev@mediadesign.school.nz
// File Name	:	EntityManager.cpp
// Description	:	Implimentation for the EntityManager class. Makes sure that
//					object vertices are only loaded once, to save on execution time

#include "EntityManager.h"

std::shared_ptr<GLuint> EntityManager::CUBEMESH = nullptr;
std::shared_ptr<GLuint> EntityManager::PYRAMIDMESH = nullptr;
std::shared_ptr<GLuint> EntityManager::MODELMESH = nullptr;

std::shared_ptr<EntityManager> EntityManager::EntityManagerPtr = nullptr;

std::shared_ptr<EntityManager> EntityManager::GetInstance() {
	if (EntityManagerPtr == nullptr) {
		EntityManagerPtr = std::shared_ptr<EntityManager>(new EntityManager());
	}
	return EntityManagerPtr;
}

void EntityManager::DestroyInstance() {
	EntityManagerPtr = nullptr;
}

std::shared_ptr<GLuint> EntityManager::GetVAO(SHAPE _Shape) {
	if (_Shape == CUBE_MESH) {
		if (CUBEMESH == nullptr) {	//If the cube vao hasnt been generated yet
			GLfloat CubeVerts[] = {
				// Positions             // Normal Coords        // TexCoords
				// Front Face            
				-1.0f, 1.0f, 1.0f,       0.0f, 0.0f, 1.0f,      0.0f, 0.0f,
				1.0f, 1.0f, 1.0f,        0.0f, 0.0f, 1.0f,      1.0f, 0.0f,
				1.0f, -1.0f, 1.0f,       0.0f, 0.0f, 1.0f,      1.0f, 1.0f,
				-1.0f, -1.0f, 1.0f,      0.0f, 0.0f, 1.0f,      0.0f, 1.0f,

				// Right Face            
				1.0f, 1.0f, 1.0f,        1.0f, 0.0f, 0.0f,      0.0f, 0.0f,
				1.0f, 1.0f, -1.0f,       1.0f, 0.0f, 0.0f,      1.0f, 0.0f,
				1.0f, -1.0f, -1.0f,      1.0f, 0.0f, 0.0f,      1.0f, 1.0f,
				1.0f, -1.0f, 1.0f,       1.0f, 0.0f, 0.0f,      0.0f, 1.0f,

				// Back Face             
				1.0f, 1.0f, -1.0f,       0.0f, 0.0f, -1.0f,     0.0f, 0.0f,
				-1.0f, 1.0f, -1.0f,      0.0f, 0.0f, -1.0f,     1.0f, 0.0f,
				-1.0f, -1.0f, -1.0f,     0.0f, 0.0f, -1.0f,     1.0f, 1.0f,
				1.0f, -1.0f, -1.0f,      0.0f, 0.0f, -1.0f,     0.0f, 1.0f,

				// Left Face             
				-1.0f, 1.0f, -1.0f,      -1.0f, 0.0f, 0.0f,     0.0f, 0.0f,
				-1.0f, 1.0f, 1.0f,       -1.0f, 0.0f, 0.0f,     1.0f, 0.0f,
				-1.0f, -1.0f, 1.0f,      -1.0f, 0.0f, 0.0f,     1.0f, 1.0f,
				-1.0f, -1.0f, -1.0f,     -1.0f, 0.0f, 0.0f,     0.0f, 1.0f,

				// Top Face              
				-1.0f, 1.0f, -1.0f,      0.0f, 1.0f, 0.0f,      0.0f, 0.0f,
				1.0f, 1.0f, -1.0f,       0.0f, 1.0f, 0.0f,      1.0f, 0.0f,
				1.0f, 1.0f, 1.0f,        0.0f, 1.0f, 0.0f,      1.0f, 1.0f,
				-1.0f, 1.0f, 1.0f,       0.0f, 1.0f, 0.0f,      0.0f, 1.0f,

				// Bottom Face           
				-1.0f, -1.0f, 1.0f,      0.0f, -1.0f, 0.0f,     0.0f, 0.0f,
				1.0f, -1.0f, 1.0f,       0.0f, -1.0f, 0.0f,     1.0f, 0.0f,
				1.0f, -1.0f, -1.0f,      0.0f, -1.0f, 0.0f,     1.0f, 1.0f,
				-1.0f, -1.0f, -1.0f,     0.0f, -1.0f, 0.0f,     0.0f, 1.0f,
			};

			//Defining Cube Indices
			GLuint CubeIndices[] = {
				0, 1, 2,		0, 2, 3,		// Front Face
				4, 5, 6,		4, 6, 7,		// Right Face
				8, 9, 10,		8, 10, 11,		// Back Face
				12, 13, 14,		12, 14, 15,		// Left Face
				16, 17, 18,		16, 18, 19,		// Top Face
				20, 21, 22,		20, 22, 23,		// Bottom Face
			};

			GLuint VBO;
			GLuint EBO;
			GLuint VAO;

			//Generating buffers
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);

			glBindVertexArray(VAO);

			//Binding and setting buffer data
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVerts), CubeVerts, GL_STATIC_DRAW);

			//Enabling the positional floats
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
				(GLvoid*)0);
			glEnableVertexAttribArray(0);

			//Enabling Normal Floats
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
				(GLvoid*)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(1);

			//Enabling the Texture floats
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
				(GLvoid*)(6 * sizeof(GLfloat)));
			glEnableVertexAttribArray(2);

			//Generating EBO
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(CubeIndices), CubeIndices, GL_STATIC_DRAW);
			CUBEMESH = std::make_shared<GLuint>(VAO);
		}
		return CUBEMESH;
	}

	if (_Shape == PYRAMID_MESH) {
		if (PYRAMIDMESH == nullptr) {	//If the cube vao hasnt been generated yet
			GLfloat PyramidVerts[] = {
				// Positions          	// Colors			// Tex Coords
				-1.0f, 0.0f, -1.0f,   	1.0f, 1.0f, 0.0f,	0.0f, 1.0f, // 0	// Base
				1.0f, 0.0f, -1.0f,   	1.0f, 0.0f, 0.0f,	1.0f, 1.0f, // 1
				1.0f, 0.0f,  1.0f,   	0.0f, 1.0f, 0.0f,	1.0f, 0.0f, // 2
				-1.0f, 0.0f,  1.0f,   	0.0f, 0.0f, 1.0f,	0.0f, 0.0f, // 3
				-1.0f, 0.0f, -1.0f,   	1.0f, 1.0f, 0.0f,	0.0f, 1.0f, // 4	// Side 1
				-1.0f, 0.0f,  1.0f,   	0.0f, 0.0f, 1.0f,	1.0f, 1.0f, // 5
				-1.0f, 0.0f,  1.0f,   	0.0f, 0.0f, 1.0f,	0.0f, 1.0f, // 6	// Side 2
				1.0f, 0.0f,  1.0f,   	0.0f, 1.0f, 0.0f,	1.0f, 1.0f, // 7
				1.0f, 0.0f,  1.0f,   	0.0f, 1.0f, 0.0f,	0.0f, 1.0f, // 8	// Side 3
				1.0f, 0.0f, -1.0f,   	1.0f, 0.0f, 0.0f,	1.0f, 1.0f, // 9
				1.0f, 0.0f, -1.0f,   	1.0f, 0.0f, 0.0f,	0.0f, 1.0f, // 10	// Side 4
				-1.0f, 0.0f, -1.0f,   	1.0f, 1.0f, 0.0f,	1.0f, 1.0f, // 11
				0.0f, 1.0f,  0.0f,  	0.0f, 1.0f, 1.0f,	0.5f, 0.0f, // 12   // Top Point
			};


			//Defining Pyramid Indices
			GLuint PyramidIndices[] = {
				4, 12, 5,	// Side 1
				6, 12, 7,	// Side 2
				8, 12, 9,	// Side 3
				10, 12, 11,	// Side 4
				3, 2, 1,	// Bottom Triangle 1
				3, 1, 0,	// Bottom Triangle 2
			};

			GLuint VBO;
			GLuint EBO;
			GLuint VAO;

			//Generating buffers
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);

			glBindVertexArray(VAO);

			//Binding and setting buffer data
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(PyramidVerts), PyramidVerts, GL_STATIC_DRAW);

			//Enabling the positional floats
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
				(GLvoid*)0);
			glEnableVertexAttribArray(0);

			//Enabling Normal Floats
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
				(GLvoid*)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(1);

			//Enabling the Texture floats
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
				(GLvoid*)(6 * sizeof(GLfloat)));
			glEnableVertexAttribArray(2);

			//Generating EBO
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(PyramidIndices), PyramidIndices, GL_STATIC_DRAW);
			PYRAMIDMESH = std::make_shared<GLuint>(VAO);
		}
		return PYRAMIDMESH;
	}

	if (_Shape == PLAYER_MESH) {

		return MODELMESH;
	}
	return nullptr;
}

//Name:					EntityManager()
//Parameters:		None
//Return Type:		None
//Description:		Entity Manager constructor that generates all the verts on startup
EntityManager::EntityManager() {
}

//Name:					~EntityManager()
//Parameters:		None
//Return Type:		None
//Description:		Destructor for the entity manager
EntityManager::~EntityManager() {

}
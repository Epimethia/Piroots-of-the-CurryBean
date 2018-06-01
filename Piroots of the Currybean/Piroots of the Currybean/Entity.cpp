#include "Entity.h"

Entity::Entity() {
	ObjPos = { 0.0f, 0.0f, 0.0f };
	VAO = 0;
	Texture = 0;
	ModelMatrix = glm::mat4();
	VPMatrix = glm::mat4();
	NumTris = 0;
};
Entity::~Entity() {};

void Entity::Draw(GLuint _Program, glm::mat4 _VPMatrix) {
	glUseProgram(_Program);
	//Binding the array
	glBindVertexArray(VAO);

	//Setting back face culling
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE);

	//Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Setting and binding the correct texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);

	//Sending the texture to the GPU via uniform
	glUniform1i(glGetUniformLocation(_Program, "tex"), 0);

	//Translating the cube (x,y,z)
	glm::mat4 TranslationMatrix = glm::translate(glm::mat4(), ObjPos / 375.0f);

	//Z Rotation
	glm::mat4 RotateZ =
		glm::rotate(
		glm::mat4(),
		glm::radians(ObjRotation.z),
		glm::vec3(0.0f, 0.0f, 1.0f)
		);

	//Y Rotation
	glm::mat4 RotateY =
		glm::rotate(
		glm::mat4(),
		glm::radians(ObjRotation.y),
		glm::vec3(0.0f, 1.0f, 0.0f)
		);

	//X Rotation
	glm::mat4 RotateX =
		glm::rotate(
		glm::mat4(),
		glm::radians(ObjRotation.x),
		glm::vec3(1.0f, 0.0f, 0.0f)
		);

	glm::mat4 RotationMatrix = RotateX * RotateY * RotateZ;

	glm::mat4 ScaleMatrix = glm::scale(glm::mat4(), ObjScale);

	ModelMatrix = TranslationMatrix * RotationMatrix * ScaleMatrix;


	glm::mat4 MVP = _VPMatrix * ModelMatrix;

	GLuint  transformLoc = glGetUniformLocation(_Program, "MVP");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(MVP));

	//Drawing the entity
	glDrawElements(GL_TRIANGLES, 100, GL_UNSIGNED_INT, 0);

	//Disabling backface culling
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	//Clearing the vertex array
	glBindVertexArray(0);
};

#pragma region CUBE FUNCTION DECLARATIONS
//Cube Constructor
CUBE::CUBE() {
	ObjPos = { 0.0f, 0.0f, 0.0f };
	VAO = 0;
	Texture = 0;
	ModelMatrix = glm::mat4();
	VPMatrix = glm::mat4();
	NumTris = 36;
}

//Cube Constructor overload
CUBE::CUBE(const char* _SpriteRef, glm::vec3 _Pos, glm::vec3 _Scale, glm::vec3 _Rot) {
	//Defining Cube Vertices
	GLfloat CubeVerts[] = {
		// Positions             // Color Coords        // TexCoords
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

	//Enabling Color Floats
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
		(GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//Enabling the Texture floats
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
		(GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	//Generating EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(CubeIndices), CubeIndices,
		GL_STATIC_DRAW);

	//Generating and binding the texture
	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);

	//Getting the image from filepath
	int width, height;
	unsigned char* image = SOIL_load_image(
		_SpriteRef,
		&width,
		&height,
		0,
		SOIL_LOAD_RGBA
	);

	//Generating the texture from image data
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		width,
		height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		image
	);

	//Generating mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);

	//Setting Texture wrap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Setting texture filters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Freeing up data
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Setting position, scale and rotation
	ObjPos = _Pos;
	ObjScale = _Scale;
	ObjRotation = _Rot;
}

//Cube Destructor
CUBE::~CUBE() {
};
#pragma endregion

#pragma region PYRAMID FUNCTION DECLARATIONS
//Pyramid Constructor
PYRAMID::PYRAMID() {
	ObjPos = { 0.0f, 0.0f, 0.0f };
	ObjScale = { 0.0f, 0.0f, 0.0f };
	ObjRotation = { 0.0f, 0.0f, 0.0f };
	VAO = 0;
	Texture = 0;
	ModelMatrix = glm::mat4();
	VPMatrix = glm::mat4();
}

//Pyramid Overload Constructor
PYRAMID::PYRAMID(const char* _TextureRef, glm::vec3 _Pos, glm::vec3 _Scale, glm::vec3 _Rot) {
	ObjPos = _Pos;
	ObjScale = _Scale;
	ObjRotation = _Rot;
	//Defining Pyramid Vertices
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

	//Enabling Color Floats
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
		(GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//Enabling the Texture floats
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
		(GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	//Generating EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(PyramidIndices), PyramidIndices,
		GL_STATIC_DRAW);


	//Generating and binding the texture
	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);

	//Getting the image from filepath
	int width, height;
	unsigned char* image = SOIL_load_image(
		_TextureRef,
		&width,
		&height,
		0,
		SOIL_LOAD_RGBA
	);

	//Generating the texture from image data
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		width,
		height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		image
	);

	//Generating mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);

	//Setting Texture wrap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Setting texture filters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Freeing up data
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void PYRAMID::Draw(GLuint _Program, glm::mat4 _VPMatrix) {
	ObjRotation.y += 1.0f;
	glUseProgram(_Program);
	//Binding the array
	glBindVertexArray(VAO);

	//Setting back face culling
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE);

	//Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Setting and binding the correct texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);

	//Sending the texture to the GPU via uniform
	glUniform1i(glGetUniformLocation(_Program, "tex"), 0);

	//Translating the cube (x,y,z)
	glm::mat4 TranslationMatrix = glm::translate(glm::mat4(), ObjPos / 375.0f);

	//Z Rotation
	glm::mat4 RotateZ =
		glm::rotate(
			glm::mat4(),
			glm::radians(ObjRotation.z),
			glm::vec3(0.0f, 0.0f, 1.0f)
		);

	//Y Rotation
	glm::mat4 RotateY =
		glm::rotate(
			glm::mat4(),
			glm::radians(ObjRotation.y),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);

	//X Rotation
	glm::mat4 RotateX =
		glm::rotate(
			glm::mat4(),
			glm::radians(ObjRotation.z+90.0f),
			glm::vec3(1.0f, 0.0f, 0.0f)
		);

	glm::mat4 RotationMatrix = RotateX * RotateY * RotateZ;

	glm::mat4 ScaleMatrix = glm::scale(glm::mat4(), ObjScale);

	ModelMatrix = TranslationMatrix * RotationMatrix * ScaleMatrix;

	glm::mat4 MVP = _VPMatrix * ModelMatrix;

	GLuint  transformLoc = glGetUniformLocation(_Program, "MVP");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(MVP));

	//Drawing the entity
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	//Disabling backface culling
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	//Clearing the vertex array
	glBindVertexArray(0);
}

PYRAMID::~PYRAMID() {};

#pragma endregion

#pragma region PLANE FUNCTION DECLARATIONS
PLANE::PLANE(const char* _SpriteRef, glm::vec3 _Pos, glm::vec3 _Scale, glm::vec3 _Rot) {
	NumTris = 32;
	GLfloat PlaneVerts[] = {
		//Positional               //Color
		-1.0, 1.0, 0.0,          0.257, 0.722, 1.0,		0.0, 0.0,		//A
		-0.5, 1.0, 0.2,          0.257, 0.722, 1.0,		0.25, 0.0,		//B
		0.0, 1.0, 0.0,           0.257, 0.722, 1.0,		0.50, 0.0,		//C
		0.5, 1.0, -0.2,          0.257, 0.722, 1.0,		0.75, 0.0,		//D
		1.0, 1.0, 0.0,           0.257, 0.722, 1.0,		1.0, 0.0,		//e

		-1.0,  0.5, 0.2,         0.257, 0.722, 1.0,		0.0, 0.25,		//f
		-0.5, 0.5, 0.0,          0.257, 0.722, 1.0,		0.25, 0.25,			//g
		0.0, 0.5, -0.2,          0.257, 0.722, 1.0,		0.50, 0.25,			//h
		0.5, 0.5, 0.0,           0.257, 0.722, 1.0,		0.75, 0.25,			//i
		1.0, 0.5, 0.2,           0.257, 0.722, 1.0,		1.0, 0.25,			//j

		-1.0, 0.0, 0.0,          0.257, 0.722, 1.0,		0.0, 0.50,		//k
		-0.5, 0.0, 0.2,          0.257, 0.722, 1.0,		0.25, 0.50,		//l
		0.0, 0.0, 0.0,           0.257, 0.722, 1.0,		0.50, 0.50,		//m
		0.5, 0.0, -0.2,          0.257, 0.722, 1.0,		0.75, 0.50,		//n
		1.0, 0.0, 0.0,           0.257, 0.722, 1.0,		1.0, 0.50,		//p

		-1.0, -0.5, 0.2,         0.257, 0.722, 1.0,		0.0, 0.75,			//p
		-0.5, -0.5, 0.0,         0.257, 0.722, 1.0,		0.25, 0.75,		//q
		0.0, -0.5, -0.2,         0.257, 0.722, 1.0,		0.50, 0.75,		//r
		0.5, -0.5, 0.0,          0.257, 0.722, 1.0,		0.75, 0.75,		//s
		1.0, -0.5, 0.2,          0.257, 0.722, 1.0,		1.0, 0.75,		//t

		-1.0, -1.0, 0.0,         0.257, 0.722, 1.0,		0.0, 1.0,		//u
		-0.5, -1.0, 0.2,         0.257, 0.722, 1.0,		0.25, 1.0,		//v
		0.0, -1.0, 0.0,          0.257, 0.722, 1.0,		0.50, 1.0,		//w
		0.5, -1.0, -0.2,         0.257, 0.722, 1.0,		0.75, 1.0,		//x
		1.0, -1.0, 0.0,          0.257, 0.722, 1.0,		1.0, 1.0,		//y
	};

	//Defining Cube Indices
	GLuint PlaneIndices[] = {
		0, 1, 5,    5, 1, 6,		//abfg
		1, 2, 6,    6, 2, 7,		//bcgh
		2, 3, 7,    7, 3, 8,		//cdhi
		3, 4, 8,    8, 4, 9,		//deij
		5, 6, 10,	10, 6, 11,		//fgkl
		6, 7, 11,	11, 7, 12,		//ghlm
		7, 8, 12,	12, 8, 13,		//himn
		8, 9, 13,	13, 9, 14,		//ijno
		10, 11, 15,	15, 11, 16,		//klpq
		11, 12, 16,	16, 12, 17,		//lmqr
		12, 13, 17,	17, 13, 18,		//mnrs
		13, 14, 18, 18, 14, 19,		//nost
		15, 16, 20, 20, 16, 21,		//pqaz
		16, 17, 21, 21, 17, 22,		//rswv
		17, 18, 22, 22, 18, 23,
		18, 19, 23, 23, 19, 24		//stvu
	};

	GLuint VBO;
	GLuint EBO;

	//Generating buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	//Binding and setting buffer data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(PlaneVerts), PlaneVerts, GL_STATIC_DRAW);

	//Enabling the positional floats
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
		(GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Enabling Color Floats
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
		(GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//Enabling the Texture floats
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
		(GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	//Generating EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(PlaneIndices), PlaneIndices,
		GL_STATIC_DRAW);

	//Generating and binding the texture
	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);

	//Getting the image from filepath
	int width, height;
	unsigned char* image = SOIL_load_image(
		_SpriteRef,
		&width,
		&height,
		0,
		SOIL_LOAD_RGBA
	);

	//Generating the texture from image data
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		width,
		height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		image
	);

	//Generating mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);

	//Setting Texture wrap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Setting texture filters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Freeing up data
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Setting position, scale and rotation
	ObjPos = _Pos;
	ObjScale = _Scale;
	ObjRotation = _Rot;
}

void PLANE::Draw(GLuint _Program, glm::mat4 _VPMatrix) {

	glUseProgram(_Program);
	//Binding the array
	glBindVertexArray(VAO);

	//Setting back face culling
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE);

	//Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Setting and binding the correct texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);

	//Sending the texture to the GPU via uniform
	glUniform1i(glGetUniformLocation(_Program, "tex"), 0);

	//Translating the cube (x,y,z)
	glm::mat4 TranslationMatrix = glm::translate(glm::mat4(), ObjPos / 375.0f);

	//Z Rotation
	glm::mat4 RotateZ =
		glm::rotate(
		glm::mat4(),
		glm::radians(ObjRotation.z),
		glm::vec3(0.0f, 0.0f, 1.0f)
		);

	//Y Rotation
	glm::mat4 RotateY =
		glm::rotate(
		glm::mat4(),
		glm::radians(ObjRotation.y),
		glm::vec3(0.0f, 1.0f, 0.0f)
		);

	//X Rotation
	glm::mat4 RotateX =
		glm::rotate(
		glm::mat4(),
		glm::radians(ObjRotation.x),
		glm::vec3(1.0f, 0.0f, 0.0f)
		);

	glm::mat4 RotationMatrix = RotateX * RotateY * RotateZ;

	glm::mat4 ScaleMatrix = glm::scale(glm::mat4(), ObjScale);

	ModelMatrix = TranslationMatrix * RotationMatrix * ScaleMatrix;


	glm::mat4 MVP = _VPMatrix * ModelMatrix;

	GLuint  transformLoc = glGetUniformLocation(_Program, "MVP");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(MVP));

	//Drawing the entity
	glDrawElements(GL_TRIANGLES, 100, GL_UNSIGNED_INT, 0);

	//Disabling backface culling
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	//Clearing the vertex array
	glBindVertexArray(0);
}
#pragma endregion

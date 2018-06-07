#include "Entity.h"

Entity::Entity() {
	ObjPos = { 0.0f, 0.0f, 0.0f };
	VAO = 0;
	Texture = 0;
	ModelMatrix = glm::mat4();
	VPMatrix = glm::mat4();
};
Entity::~Entity() {};

void Entity::Render(GLuint _Program, glm::mat4 _VPMatrix) {
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
}

//Cube Constructor overload
CUBE::CUBE(const char* _SpriteRef, glm::vec3 _Pos, glm::vec3 _Scale, glm::vec3 _Rot) {
	//Defining Cube Vertices
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
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

void CUBE::Render(GLuint _Program, glm::mat4 _VPMatrix) {
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

void PYRAMID::Render(GLuint _Program, glm::mat4 _VPMatrix) {
	ObjRotation.x += 10.0f;
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
	glm::mat4 RotateX =
		glm::rotate(
			glm::mat4(),
			glm::radians(ObjRotation.x),
			glm::vec3(1.0f, 0.0f, 0.0f)
		);

	//Y Rotation
	glm::mat4 RotateY =
		glm::rotate(
			glm::mat4(),
			glm::radians(ObjRotation.y),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);

	//X Rotation
	glm::mat4 RotateZ =
		glm::rotate(
		glm::mat4(),
			glm::radians(ObjRotation.z + 35.0f),
			glm::vec3(0.0f, 0.0f, 1.0f)
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


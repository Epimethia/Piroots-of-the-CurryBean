#include "Entity.h"

Entity::Entity() {
	ObjPos = { 0.0f, 0.0f, 0.0f };
	VAO = 0;
	Texture = 0;
	ModelMatrix = glm::mat4();
	VPMatrix = glm::mat4();
};
Entity::~Entity() {};

void Entity::Render(GLuint _Program, glm::mat4 _VPMatrix) {};

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
	ObjPos = _Pos;
	ObjScale = _Scale;
	ObjRotation = _Rot;
	VAO = EntityManager::GetVAO(CUBE_MESH);

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
}

void CUBE::Render(GLuint _Program, glm::mat4 _VPMatrix) {
	glUseProgram(_Program);
	//Binding the array

	glBindVertexArray(*VAO);

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
CUBE::~CUBE() {};
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
PYRAMID::PYRAMID(const char* _SpriteRef, glm::vec3 _Pos, glm::vec3 _Scale, glm::vec3 _Rot) {
	ObjPos = _Pos;
	ObjScale = _Scale;
	ObjRotation = _Rot;
	VAO = EntityManager::GetVAO(PYRAMID_MESH);

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
}

void PYRAMID::Render(GLuint _Program, glm::mat4 _VPMatrix) {
	glUseProgram(_Program);
	//Binding the array
	glBindVertexArray(*VAO);

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


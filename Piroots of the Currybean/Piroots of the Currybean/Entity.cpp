#include "Entity.h"

Entity::Entity() {
}

//Name:					Entity()
//Parameters:		SHAPE, const char*, glm::vec3, glm::vec3, glm::vec3,
//Return Type:		None
//Description:		Overloaded Constructor to create an entity with the specified
//							mesh.
Entity::Entity(ENTITY_TYPE _EntityType, GLuint _Shader, glm::vec3 _Pos) {
	Shader = _Shader;
	if (_EntityType == CUBE_PICKUP_ENTITY) {
		ObjScale = glm::vec3(1.0f, 1.0f, 1.0f);
		ObjRotation = glm::vec3(0.0f, 0.0f, 0.0f);
		ObjPos = _Pos;
		VAO = EntityManager::GetMesh(CUBE_PICKUP_ENTITY)->VAO;
		NumIndices = EntityManager::GetMesh(CUBE_PICKUP_ENTITY)->NumIndices;
		Texture = EntityManager::GetMesh(CUBE_PICKUP_ENTITY)->Texture;
	}

};

Entity::~Entity() {};

void Entity::Render() {	
	glUseProgram(Shader);

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
	glUniform1i(glGetUniformLocation(Shader, "tex"), 0);

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

	glm::mat4 MVP = VPMatrix * ModelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(Shader, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
	glUniformMatrix4fv(glGetUniformLocation(Shader, "model"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));
	glUniform3fv(glGetUniformLocation(Shader, "camPos"), 1, glm::value_ptr(Camera::GetPos()));
	//Drawing the entity
	glDrawElements(GL_TRIANGLES, NumIndices, GL_UNSIGNED_INT, 0);

	//Disabling backface culling
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	//Clearing the vertex array
	glBindVertexArray(0);
};

void Entity::Process(glm::mat4 _VPMatrix) {
	VPMatrix = _VPMatrix;

	//REMOVE
	//DO OTHER PROCESSING STUFF IN HERE

	Render();
}

PickUp::PickUp(glm::vec3 _Pos, GLuint _Shader) {
	Shader = _Shader;
	ObjScale = glm::vec3(0.05f, 0.05f, 0.05f);
	ObjRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	ObjPos = _Pos;
	VAO = EntityManager::GetMesh(CUBE_PICKUP_ENTITY)->VAO;
	NumIndices = EntityManager::GetMesh(CUBE_PICKUP_ENTITY)->NumIndices;
	Texture = EntityManager::GetMesh(CUBE_PICKUP_ENTITY)->Texture;
};

void PickUp::Process(glm::mat4 _VPMatrix) {
	ObjRotation.z += 5.0f;
	VPMatrix = _VPMatrix;
	Render();
}

EnemySmall::EnemySmall(glm::vec3 _Pos, GLuint _Shader) {

}

void EnemySmall::Process(glm::mat4 _VPMatrix) {

}

Wave::Wave(glm::vec3 _Pos, GLuint _Shader) {
	Shader = _Shader;
	ObjScale = glm::vec3(0.3f, 0.2f, 0.3f);
	ObjRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	ObjPos = _Pos;
	model = EntityManager::GetModel(WAVE_ENTITY, Shader);
};

void Wave::Process(glm::mat4 _VPMatrix) {
	VPMatrix = _VPMatrix;
	Render();
}

void Wave::Render() {
	glm::mat4 TranslationMatrix = glm::translate(glm::mat4(), ObjPos / 375.0f);

	//X Rotation
	glm::mat4 RotateX =
		glm::rotate(
			glm::mat4(),
			glm::radians(ObjRotation.x + 90.0f),
			glm::vec3(1.0f, 0.0f, 0.0f)
		);

	glm::mat4 ScaleMatrix = glm::scale(glm::mat4(), glm::vec3(ObjScale));
	glm::mat4 ModelMatrix = TranslationMatrix * RotateX * ScaleMatrix;

	model->Render(ModelMatrix);
}
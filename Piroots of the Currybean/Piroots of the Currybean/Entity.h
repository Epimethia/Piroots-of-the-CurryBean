#pragma once
#include "Resource.h"
#include "EntityManager.h"

class Entity {
public:
	Entity();
	Entity(ENTITY_TYPE _EntityType, const char* _SpriteRef, GLuint _Shader,
		glm::vec3 _Pos, glm::vec3 _Scale, glm::vec3 _Rot);
	virtual ~Entity();
	virtual glm::vec3& GetPos() { return ObjPos; };
	virtual glm::vec3& GetScale() { return ObjScale; };
	virtual glm::vec3& GetRotation() { return ObjRotation; };
	virtual void Render();
	virtual void Process(glm::mat4 _VPMatrix);


protected:
	GLuint VAO;
	unsigned int NumIndices;
	GLuint Texture;
	GLuint Shader;
	glm::vec3 ObjPos;
	glm::vec3 ObjScale;
	glm::vec3 ObjRotation;

	glm::mat4 ModelMatrix;
	glm::mat4 VPMatrix;
};


#pragma once
#include "Resource.h"
#include "EntityManager.h"

class Entity {
public:
	Entity();
	Entity(ENTITY_TYPE _EntityType, GLuint _Shader, glm::vec3 _Pos);
	virtual ~Entity();
	virtual glm::vec3& GetPos() { return ObjPos; };
	virtual glm::vec3& GetScale() { return ObjScale; };
	virtual glm::vec3& GetRotation() { return ObjRotation; };
	virtual glm::vec3& GetVelocity() { return Velocity; };
	virtual void Render();
	virtual void Process(glm::mat4 _VPMatrix);

protected:
	GLuint VAO;
	unsigned int NumIndices;
	std::shared_ptr<Model> model;
	GLuint Texture;
	GLuint Shader;

	glm::vec3 ObjPos;
	glm::vec3 ObjScale;
	glm::vec3 ObjRotation;
	glm::vec3 Velocity = { 0.0f, 1000.0f, 0.0f };

	glm::mat4 ModelMatrix;
	glm::mat4 VPMatrix;
};

class PickUp : public Entity {
public:
	PickUp(glm::vec3 _Pos, GLuint _Shader);
	void Process(glm::mat4 _VPMatrix);
};

class EnemySmall : public Entity {
public:
	EnemySmall(glm::vec3 _Pos, GLuint _Shader);
	void Process(glm::mat4 _VPMatrix);
};

class Wave : public Entity {
public:
	Wave(glm::vec3 _Pos, GLuint _Shader);
	void Process(glm::mat4 _VPMatrix);
	void Render();
};

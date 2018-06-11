#pragma once
#include "Resource.h"
#include "EntityManager.h"
#include "Clock.h"

class Entity {
public:
	Entity();
	Entity(ENTITY_TYPE _EntityType, GLuint _Shader, glm::vec3 _Pos);
	virtual ~Entity();
	virtual glm::vec3& GetPos() { return ObjPos; };
	virtual glm::vec3& GetScale() { return ObjScale; };
	virtual glm::vec3& GetRotation() { return ObjRotation; };
	virtual glm::vec3& GetVelocity() { return Velocity; };
	virtual void Process(float _DeltaTime);

protected:
	virtual void Render();

	GLuint VAO;
	unsigned int NumIndices;
	GLuint Texture;
	GLuint Shader;

	glm::vec3 ObjPos;
	glm::vec3 ObjScale;
	glm::vec3 ObjRotation;
	glm::vec3 Velocity;

	glm::mat4 ModelMatrix;
	glm::mat4 VPMatrix;
	
};

class ModelEntity : public Entity{
public:
	ModelEntity();
	ModelEntity(ENTITY_TYPE _EntityType, GLuint _Shader, glm::vec3 _Pos);
	virtual void Process(float _DeltaTime);
protected:
	virtual void Render();
	std::shared_ptr<Model> model;
};

class PickUp : public Entity {
public:
	PickUp(glm::vec3 _Pos, GLuint _Shader);
	void Process(float _DeltaTime);
};

class Bullet : public Entity {
public :
	Bullet(glm::vec3 _Velocity, glm::vec3 _Pos, GLuint _Shader);
	void Process(float _DeltaTime);
private:
	float MaxSpeed;
};

class Wave : public ModelEntity {
public:
	Wave(glm::vec3 _Pos, GLuint _Shader);
	void Process(float _DeltaTime);
private:
	void Render();
};

class Player : public ModelEntity {
public:
	Player(glm::vec3 _Pos, GLuint _Shader);
	glm::vec3& GetTarget() { return Target; };
	void Process(float _DeltaTime);
protected:
	glm::vec3 Target;
	void Render();
};
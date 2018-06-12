#pragma once
#include "Resource.h"
#include "EntityManager.h"
#include "Clock.h"
#include "AutonomousMove.h"

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


class PickUp : public Entity {
public:
	PickUp(glm::vec3 _Pos, GLuint _Shader);
	void Process(float _DeltaTime);
private:
	float ZBobbing;
};

class Bullet : public Entity {
public :
	Bullet(glm::vec3 _Velocity, glm::vec3 _Pos, GLuint _Shader);
	void Process(float _DeltaTime);
private:
	float MaxSpeed;
};

class ModelEntity : public Entity {
public:
	ModelEntity();
	ModelEntity(ENTITY_TYPE _EntityType, GLuint _Shader, glm::vec3 _Pos);
	virtual void Process(float _DeltaTime);
protected:
	virtual void Render();
	std::shared_ptr<Model> model;
};

class Wave : public ModelEntity {
public:
	Wave(glm::vec3 _Pos, GLuint _Shader);
	void Process(float _DeltaTime);
private:
	void Render();
};

class AutoAgent : public ModelEntity {
public:
	AutoAgent();
	AutoAgent(glm::vec3 _Pos, GLuint _Shader);
	glm::vec3& GetTarget() { return Target; };
	float GetSpeed() { return MaxSpeed; };
	float GetForce() { return MaxForce; }
	void Process(float _DeltaTime);
protected:
	glm::vec3 Target;
	float MaxSpeed;
	float MaxForce;
	void Render();
};

class SmallEnemy : public AutoAgent {
public:
	SmallEnemy(glm::vec3 _Pos, GLuint _Shader, std::shared_ptr<Entity> _TargetEntity);
	void Process(float _DeltaTime);
private: 
	std::shared_ptr<Entity> TargetEntity;
};
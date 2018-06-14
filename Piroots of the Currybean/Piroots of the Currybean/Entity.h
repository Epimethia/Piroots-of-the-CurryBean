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
	virtual glm::vec3& GetVelocity() { return ObjVel; };
	virtual void Process(float _DeltaTime);

	ENTITY_TYPE Type;

protected:
	virtual void Render();

	GLuint VAO;
	unsigned int NumIndices;
	GLuint Texture;
	GLuint Shader;

	glm::vec3 ObjPos;
	glm::vec3 ObjScale;
	glm::vec3 ObjRotation;
	glm::vec3 ObjVel;

	glm::mat4 ModelMatrix;
	glm::mat4 VPMatrix;

};

class PickUp : public Entity {
public:
	PickUp(glm::vec3 _Pos, ENTITY_TYPE _Type);
	void Process(float _DeltaTime);
	ENTITY_TYPE Type;
private:
	float ZBobbing;
};

class Bullet : public Entity {
public:
	Bullet(glm::vec3 _Velocity, glm::vec3 _Pos);
	void Process(float _DeltaTime);
private:
	float MaxSpeed;
};

class Text {
public:

private:

};



class ModelEntity : public Entity {
public:
	ModelEntity();
	ModelEntity(ENTITY_TYPE _EntityType, glm::vec3 _Pos);
	virtual void Process(float _DeltaTime);
protected:
	virtual void Render();
	std::shared_ptr<Model> model;
};

class Wave : public ModelEntity {
public:
	Wave(glm::vec3 _Pos);
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

class Player : public AutoAgent {
public:
	Player(glm::vec3 _Pos);
	void CreateBullet(glm::vec3 Velocity);
	void Process(float _DeltaTime);
	std::vector<std::shared_ptr<Bullet>>& GetBulletVect() { return BulletVect; };
	bool bShoot;
	float PowerUpDuration;
	ENTITY_TYPE State;

private:
	std::vector<std::shared_ptr<Bullet>> BulletVect;
	void Render();

	float ShootCooldown;
	float ShootTimer;
	glm::vec3 BulletVelocity;
};

class PursueEnemy : public AutoAgent {
public:
	PursueEnemy(glm::vec3 _Pos, std::shared_ptr<Entity> _TargetEntity);
	void Process(float _DeltaTime);
private:
	std::shared_ptr<Entity> TargetEntity;
	float ShootTimer = 5.0f;
};

class WanderEnemy : public AutoAgent {
public:
	WanderEnemy(glm::vec3 _Pos, std::shared_ptr<Entity> _TargetEntity);
	void Process(float _DeltaTime);
private:
	std::shared_ptr<Entity> TargetEntity;
};
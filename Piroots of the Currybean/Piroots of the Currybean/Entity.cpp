#include "Entity.h"

#pragma region ENTITY BASE CLASS DEFINITIONS
Entity::Entity() {
}

//Name:					Entity()
//Parameters:		SHAPE, const char*, glm::vec3, glm::vec3, glm::vec3,
//Return Type:		None
//Description:		Overloaded Constructor to create an entity with the specified
//							mesh.
Entity::Entity(ENTITY_TYPE _EntityType, GLuint _Shader, glm::vec3 _Pos) {
	Shader = _Shader;
	ObjVel = { 0.0f, 1000.0f, 0.0f };
	ObjScale = glm::vec3(1.0f, 1.0f, 1.0f);
	ObjRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	ObjPos = _Pos;
	VAO = EntityManager::GetMesh(_EntityType)->VAO;
	NumIndices = EntityManager::GetMesh(_EntityType)->NumIndices;
	Texture = EntityManager::GetMesh(_EntityType)->Texture;
	Type = _EntityType;
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

void Entity::Process(float _DeltaTime) {
	VPMatrix = Camera::GetMatrix();

	//REMOVE
	//DO OTHER PROCESSING STUFF IN HERE

	Render();
}
#pragma endregion


#pragma region PICKUP FUNCTION DEFINITIONS
PickUp::PickUp(glm::vec3 _Pos, GLuint _Shader) {
	Shader = _Shader;
	ObjVel = { 0.0f, 0.0f, 0.0f };
	ObjScale = glm::vec3(0.05f, 0.05f, 0.05f);
	ObjRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	ObjPos = _Pos;
	ZBobbing = 0.0f;
	VAO = EntityManager::GetMesh(CUBE_PICKUP)->VAO;
	NumIndices = EntityManager::GetMesh(CUBE_PICKUP)->NumIndices;
	Texture = EntityManager::GetMesh(CUBE_PICKUP)->Texture;
};

void PickUp::Process(float _DeltaTime) {
	ObjRotation.z += (75.0f * _DeltaTime);
	ObjPos.z += (sin(ZBobbing ))* 7.0f;
	ZBobbing += (10.0f * _DeltaTime);
	VPMatrix = Camera::GetMatrix();
	Render();
}
#pragma endregion


#pragma region BULLET FUNCTION DEFINITIONS
Bullet::Bullet(glm::vec3 _Velocity, glm::vec3 _Pos) {
	ObjPos = _Pos;
	ObjPos.z += 25.0f;
	ObjScale = glm::vec3(0.04f, 0.04f, 0.04f);
	ObjRotation = glm::vec3();
	ObjVel = _Velocity;
	MaxSpeed = 1000.0f;
	VAO = EntityManager::GetMesh(BULLET_ENTITY)->VAO;
	NumIndices = EntityManager::GetMesh(BULLET_ENTITY)->NumIndices;
	Texture = EntityManager::GetMesh(BULLET_ENTITY)->Texture;
	Shader = EntityManager::GetMesh(BULLET_ENTITY)->Shader;
	Type = BULLET_ENTITY;
}

void Bullet::Process(float _DeltaTime) {
	VPMatrix = Camera::GetMatrix();
	if (ObjVel == glm::vec3(0.0f, 0.0f, 0.0f)) return;
	ObjPos += glm::normalize(ObjVel) * MaxSpeed * _DeltaTime;
	Render();
}
#pragma endregion


#pragma region MODEL ENTITY FUNCTION DEFINITIONS
ModelEntity::ModelEntity() {

}

ModelEntity::ModelEntity(ENTITY_TYPE _EntityType, glm::vec3 _Pos) {
	ObjScale = glm::vec3(0.1f, 0.1f, 0.1f);
	ObjRotation = glm::vec3();
	ObjPos = _Pos;
	ObjVel = glm::vec3();
	model = EntityManager::GetModel(PLAYER_ENTITY);
	Type = _EntityType;
}

void ModelEntity::Render() {
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

void ModelEntity::Process(float _DeltaTime) {
	VPMatrix = Camera::GetMatrix();
	Render();
}
#pragma endregion


#pragma region WAVE FUNCTION DEFINITIONS
Wave::Wave(glm::vec3 _Pos) {
	ObjScale = glm::vec3(0.3f, 0.2f, 0.3f);
	ObjRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	ObjPos = _Pos;
	model = EntityManager::GetModel(WAVE_ENTITY);
	Type = WAVE_ENTITY;
};

void Wave::Process(float _DeltaTime) {
	VPMatrix = Camera::GetMatrix();
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
#pragma endregion


#pragma region AUTONOMOUS AGENT FUNCTION DEFINITIONS
AutoAgent::AutoAgent() {};

AutoAgent::AutoAgent(glm::vec3 _Pos, GLuint _Shader) {
	Shader = _Shader;
	ObjScale = glm::vec3(0.05f, 0.05f, -0.05f);
	ObjRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	ObjPos = _Pos;
	ObjVel = { 0.0f, 0.0f, 0.0f };
	Target = { 0.0f, 0.0f, 0.0f };
	model = EntityManager::GetModel(PLAYER_ENTITY);
	Type = PLAYER_ENTITY;
}

void AutoAgent::Process(float _DeltaTime) {
	VPMatrix = Camera::GetMatrix();
	ObjVel += AutoMove::Seek(ObjPos, ObjVel, Target);
	ObjPos += ObjVel * 30.0f * _DeltaTime;
	Render();
}

void AutoAgent::Render() {
	glm::mat4 TranslationMatrix = glm::translate(glm::mat4(), glm::vec3(ObjPos.x, ObjPos.y, ObjPos.z + 30.0f) / 375.0f);

	float PI = 3.14159265359f;
	float angle;
	angle = atan2f(ObjVel.x, ObjVel.y) * (180.0f/PI);

	//X Rotation
	glm::mat4 RotateX =
		glm::rotate(
			glm::mat4(),
			glm::radians(ObjRotation.x + 90.0f),
			glm::vec3(1.0f, 0.0f, 0.0f)
		);

	glm::mat4 RotateY =
		glm::rotate(
			glm::mat4(),
			glm::radians(ObjRotation.y + (angle * -1.0f)),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);

	glm::mat4 ScaleMatrix = glm::scale(glm::mat4(), glm::vec3(ObjScale));
	glm::mat4 ModelMatrix = TranslationMatrix * (RotateX * RotateY) * ScaleMatrix;

	model->Render(ModelMatrix);
}
#pragma endregion


#pragma region PLAYER FUNCTION DEFINITIONS
Player::Player(glm::vec3 _Pos) {
	ObjPos = _Pos;
	ObjScale = glm::vec3(0.05f, 0.05f, -0.05f);
	ObjRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	ObjVel = { 0.0f, 0.0f, 0.0f };
	Target = { 0.0f, 0.0f, 0.0f };
	MaxSpeed = 20.0f;
	MaxForce = 0.0f;
	model = EntityManager::GetModel(PLAYER_ENTITY);
	Type = PLAYER_ENTITY;
	ShootCooldown = 3.0f;
	ShootTimer = 0.0f;
	bShoot = false;
}

void Player::CreateBullet(glm::vec3 Velocity) {
	BulletVelocity = Velocity;
	bShoot = true;
}

void Player::Process(float _DeltaTime) {
	VPMatrix = Camera::GetMatrix();
	ShootTimer += 10.f * _DeltaTime;
	
	if (ShootTimer >= ShootCooldown) {
		if (bShoot) {
			ShootTimer = 0.0f;
			BulletVect.push_back(std::make_shared<Bullet>(
				BulletVelocity,
				ObjPos
				));
		}
	}

	ObjVel += AutoMove::Seek(ObjPos, ObjVel, Target);
	ObjPos += ObjVel * MaxSpeed * _DeltaTime;
	Render();

	//Checking that bullets dont go out of range
	for (unsigned int i = 0; i < BulletVect.size(); ++i) {
		if (BulletVect[i]->GetPos().y >= 3300.0f ||
			BulletVect[i]->GetPos().y <= -3300.0f ||
			BulletVect[i]->GetPos().x >= 3300.0f ||
			BulletVect[i]->GetPos().x <= -3300.0f) {
			BulletVect.erase(BulletVect.begin(), BulletVect.begin() + i);
			continue;
		} 
		BulletVect[i]->Process(_DeltaTime);
	}
}

void Player::Render() {
	glm::mat4 TranslationMatrix = glm::translate(glm::mat4(), glm::vec3(ObjPos.x, ObjPos.y, ObjPos.z + 30.0f) / 375.0f);

	float PI = 3.14159265359f;
	float angle;
	angle = atan2f(ObjVel.x, ObjVel.y) * (180.0f / PI);

	//X Rotation
	glm::mat4 RotateX =
		glm::rotate(
			glm::mat4(),
			glm::radians(ObjRotation.x + 90.0f),
			glm::vec3(1.0f, 0.0f, 0.0f)
		);

	glm::mat4 RotateY =
		glm::rotate(
			glm::mat4(),
			glm::radians(ObjRotation.y + (angle * -1.0f)),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);

	glm::mat4 ScaleMatrix = glm::scale(glm::mat4(), glm::vec3(ObjScale));
	ModelMatrix = TranslationMatrix * (RotateX * RotateY) * ScaleMatrix;

	model->Render(ModelMatrix);
}
#pragma endregion


#pragma region SEEK ENEMY FUNCTION DEFINITIONS
SeekEnemy::SeekEnemy(glm::vec3 _Pos, std::shared_ptr<Entity> _TargetEntity) {
	ObjScale = glm::vec3(0.03f, 0.03f, -0.03f);
	ObjRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	ObjPos = _Pos;
	ObjVel = { 1.0f, 0.0f, 0.0f };
	TargetEntity = _TargetEntity;
	MaxForce = 1.0f;
	MaxSpeed = 7.0f;
	model = EntityManager::GetModel(PLAYER_ENTITY);
	Type = SEEK_ENEMY;
}

void SeekEnemy::Process(float _DeltaTime) {
	VPMatrix = Camera::GetMatrix();
	ObjVel += AutoMove::Persue(ObjPos, ObjVel, TargetEntity->GetPos(), TargetEntity->GetVelocity());
	ObjPos += ObjVel * MaxSpeed * _DeltaTime;
	Render();
}
#pragma endregion

#pragma region WANDER ENEMY FUNCTIONS
WanderEnemy::WanderEnemy(glm::vec3 _Pos, std::shared_ptr<Entity> _TargetEntity) {
	ObjScale = glm::vec3(0.06f, 0.06f, -0.06f);
	ObjRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	ObjPos = _Pos;
	ObjVel = { 1.0f, 0.0f, 0.0f };
	TargetEntity = _TargetEntity;
	MaxForce = 1.0f;
	MaxSpeed = 10.0f;
	model = EntityManager::GetModel(PLAYER_ENTITY);
	Type = WANDER_ENEMY;
}

void WanderEnemy::Process(float _DeltaTime) {
	VPMatrix = Camera::GetMatrix();
	ObjVel += AutoMove::Wander(ObjPos, ObjVel, MaxSpeed, MaxForce);
	ObjPos += ObjVel * MaxSpeed * _DeltaTime;
	Render();
}
#pragma endregion


#pragma once
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// Author		:	James Devenport
// Email		:	james.dev@mediadesign.school.nz
// File Name	:	EntityManager.h
// Description	:	Header file defining the EntityManager class. Makes sure that
//					things such as object vertices are only loaded once, to save 
//					on execution time

#include "Resource.h"
#include "Model.h"
#include "ShaderLoader.h"


enum ENTITY_TYPE {
	
	//Entities
	PLAYER_ENTITY,
	BULLET_ENTITY,
	WAVE_ENTITY,
	SEEK_ENEMY,
	WANDER_ENEMY,

	//Powerups
	ATTACK_POWERUP,
	SPEED_POWERUP,
	NONE
};

struct MESH {
	GLuint VAO;
	unsigned int NumIndices;
	GLuint Texture;
	GLuint Shader;
};

class EntityManager {
public:
	static std::shared_ptr<EntityManager> GetInstance();
	static void DestroyInstance();

	static std::shared_ptr<MESH> GetMesh(ENTITY_TYPE _EntityType);
	static std::shared_ptr<Model> GetModel(ENTITY_TYPE _EntityType);
	~EntityManager();

private:
	EntityManager();

	static std::shared_ptr<EntityManager> EntityManagerPtr;

	static GLuint WaveShader;
	static GLuint ObjectShader;
	static GLuint ModelShader;
	static GLuint TextShader;

	static std::shared_ptr<MESH> Cube_Mesh;
	static std::shared_ptr<MESH> Pyramid_Mesh;
	static std::shared_ptr<MESH> Sphere_Mesh;

	static std::shared_ptr<Model> Wave_Model;
	static std::shared_ptr<Model> Player_Model;

	ShaderLoader SL;
};
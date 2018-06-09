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
#include "Mesh.h"


enum ENTITY_TYPE {
	CUBE_ENTITY,
	PLAYER_ENTITY,
	ENEMY_ENTITY,
	CUBEMAP_ENTITY,
	WAVE_ENTITY,
	ENEMY_SMALL_ENTITY,
	ENEMY_MEDIUM_ENTITY,
	ENEMY_BIG_ENTITY
};

struct MESH {
	GLuint VAO;
	unsigned int NumIndices;
};

class EntityManager {
public:
	static std::shared_ptr<EntityManager> GetInstance();
	static void DestroyInstance();

	static std::shared_ptr<MESH> GetMesh(ENTITY_TYPE _EntityType);
	static std::shared_ptr<Model> GetModel(ENTITY_TYPE _EntityType, GLuint _Program);
	~EntityManager();

private:
	EntityManager();

	static std::shared_ptr<EntityManager> EntityManagerPtr;


	static std::shared_ptr<MESH> Cube_Mesh;
	static std::shared_ptr<MESH> Pyramid_Mesh;

	static std::shared_ptr<Model> Wave_Model;
	static std::shared_ptr<Model> Player_Model;
};
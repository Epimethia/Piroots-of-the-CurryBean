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
enum TYPE {
	CUBE_MESH,
	PYRAMID_MESH,
	PLAYER_MESH,
	CUSTOM_MESH,
	ENEMY_SMALL_MESH,
	ENEMY_MEDIUM_MESH,
	ENEMY_BIG_MESH
};

struct MESH {
	std::shared_ptr<GLuint> VAO;
	unsigned int NumIndices;
};

class EntityManager {
public:
	static std::shared_ptr<EntityManager> GetInstance();
	static void DestroyInstance();

	static std::shared_ptr<MESH> GetMesh(TYPE _Shape);
	~EntityManager();

private:
	EntityManager();

	static std::shared_ptr<EntityManager> EntityManagerPtr;

	static std::shared_ptr<MESH> CUBEMESH;
	static std::shared_ptr<MESH> PYRAMIDMESH;
	static std::shared_ptr<MESH> MODELMESH;
};
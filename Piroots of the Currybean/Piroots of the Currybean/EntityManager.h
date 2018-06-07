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
#include "Entity.h"
enum SHAPE {
	CUBE_MESH,
	PYRAMID_MESH,
	PLAYER_MESH,
	ENEMY_SMALL_MESH,
	ENEMY_MEDIUM_MESH,
	ENEMY_BIG_MESH
};

class EntityManager {
public:
	static std::shared_ptr<EntityManager> GetInstance();
	static void DestroyInstance();
	static std::shared_ptr<GLuint> GetVAO(SHAPE _SHAPE);
	~EntityManager();

private:
	EntityManager();

	static std::shared_ptr<EntityManager> EntityManagerPtr;

	static std::shared_ptr<GLuint> CUBEMESH;
	static std::shared_ptr<GLuint> PYRAMIDMESH;
	static std::shared_ptr<GLuint> MODELMESH;
};
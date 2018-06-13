#pragma once
#include "Resource.h"
#include "ShaderLoader.h"
#include "Entity.h"
#include "Camera.h"
#include "InputManager.h"
#include "Mesh.h"
#include "CubeMap.h"
#include "EntityManager.h"

//Singleton scene manager
class GameManager {
public:
	~GameManager();
	static void DestroyInstance();
	static std::shared_ptr<GameManager> GetInstance();
	void DrawScene(float _DeltaTime);

	void Init();
	void GameLoop(float _DeltaTime);

private:
	GameManager();
	static std::shared_ptr<GameManager> SceneManagerPtr;

	//Shaders
	ShaderLoader SL;
	GLuint CubeMapShader;

	//World Objects
	CubeMap CM;
	std::shared_ptr<Player> PlayerObj;
	std::shared_ptr<Wave> WaveObj;

	//Gameplay Elements
	std::vector<std::shared_ptr<AutoAgent>> EnemyVect;
	std::vector<std::shared_ptr<PickUp>> PickUpVect;

	float SpawnLimiter = 3.0f;
	float SpawnTimer = 0.0f;
};
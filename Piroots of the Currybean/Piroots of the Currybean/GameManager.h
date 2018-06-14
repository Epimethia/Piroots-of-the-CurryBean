#pragma once
#include "Resource.h"
#include "ShaderLoader.h"
#include "Entity.h"
#include "Camera.h"
#include "InputManager.h"
#include "Mesh.h"
#include "CubeMap.h"
#include "EntityManager.h"
#include "Text.h"

enum GameState {
	START,
	GAME,
	END
};

//Singleton scene manager
class GameManager {
public:
	~GameManager();
	static void DestroyInstance();
	static std::shared_ptr<GameManager> GetInstance();
	void DrawScene();

	void GameLoop(float _DeltaTime);

private:
	GameManager();
	static std::shared_ptr<GameManager> SceneManagerPtr;
	float DeltaTime;
	GameState CurrentState;

	void DrawMenu();
	void DrawGame();
	void DrawEnd();

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

	std::shared_ptr<Text> t = std::make_shared<Text>("HEllo", ARIAL, glm::vec3(0.0f, 0.0f, 0.0f));

};
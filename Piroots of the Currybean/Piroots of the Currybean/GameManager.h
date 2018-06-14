#pragma once
#include "Resource.h"
#include "Entity.h"
#include "InputManager.h"
#include "CubeMap.h"
#include "EntityManager.h"
#include "Text.h"
#include "Menu.h"


enum GameState {
	START,
	GAME,
	OPTION,
	PAUSE,
	END
};

//Singleton scene manager
class GameManager {
public:
	~GameManager();
	static void DestroyInstance();
	static std::shared_ptr<GameManager> GetInstance();
	void DrawScene();
	static GameState& GetState() { return CurrentState; };
	void RestartGame();
	void GameLoop(float _DeltaTime);
	void ToggleMusic();
	bool MusicState() { return PlayMusic; };

private:
	GameManager();
	static std::shared_ptr<GameManager> SceneManagerPtr;
	float DeltaTime;

	static GameState CurrentState;

	void DrawMenu();
	void DrawGame();
	void DrawEnd();
	void DrawOption();

	//Menu Elements
	std::shared_ptr<Text> Title0;
	std::shared_ptr<Text> Title1;
	std::shared_ptr<Text> Title2;
	std::shared_ptr<Text> ScoreText;

	std::shared_ptr<Menu> StartMenu;
	std::shared_ptr<Menu> EndMenu;
	std::shared_ptr<Menu> OptionMenu;

	//Shaders
	ShaderLoader SL;
	GLuint CubeMapShader;
	GLuint TextShader;
	unsigned int Score;

	//World Objects
	CubeMap CM;
	std::shared_ptr<Player> PlayerObj;
	std::shared_ptr<Wave> WaveObj;

	//Entity Vectors
	std::vector<std::shared_ptr<AutoAgent>> EnemyVect;
	std::vector<std::shared_ptr<PickUp>> PickUpVect;

	float SpawnLimiter = 3.0f;
	float SpawnTimer = 0.0f;

	//Sound
	SoundManager sm;
	bool PlayMusic;

	//Other
	std::shared_ptr<Model> UIBoat;
	float ZBobbing = 0.0f;
	glm::mat4 StartMenuMatrix;
	glm::mat4 EndMenuMatrix;
};
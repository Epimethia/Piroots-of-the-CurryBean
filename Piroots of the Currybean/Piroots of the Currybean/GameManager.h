#pragma once
#include "Resource.h"
#include "Entity.h"
#include "InputManager.h"
#include "CubeMap.h"
#include "EntityManager.h"
#include "Text.h"
#include "Menu.h"
#include "Server.h"
#include "Client.h"
#include "ServerManager.h"

class GameManager {
public:
	~GameManager();
	//Singleton Methods
	static void DestroyInstance();
	static std::shared_ptr<GameManager> GetInstance();

	void GameLoop(float _DeltaTime);
	void RestartGame();
	void DrawScene();
	static GameState& GetState() { return CurrentState; };

	void ToggleMusic();
	bool MusicState() { return PlayMusic; };

private:
	GameManager();
	static std::shared_ptr<GameManager> SceneManagerPtr;

	//Game Variables
	float DeltaTime;
	float SpawnLimiter = 3.0f;
	float SpawnTimer = 0.0f;
	static GameState CurrentState;
	unsigned int Score;

	//Menu Elements
	void DrawMenu();
	void DrawGame();
	void DrawEnd();
	void DrawOption();
	void DrawServerOption();
	void DrawHostLobby();
	void DrawClientLobby();
	void DrawServerSelect();

	std::shared_ptr<Text> Title0;
	std::shared_ptr<Text> Title1;
	std::shared_ptr<Text> EndGameTitle;
	std::shared_ptr<Text> ScoreText;
	std::shared_ptr<Text> MultiplayerTitle0;
	std::shared_ptr<Text> MultiplayerTitle1;
	std::shared_ptr<Text> WaitingForParty;

	std::vector<std::string> StartOpt;
	std::vector<std::string> MultiOpt;
	std::vector<std::string> OptOpt;
	std::vector<std::string> EndOpt;

	std::shared_ptr<Menu> StartMenu;
	std::shared_ptr<Menu> EndMenu;
	std::shared_ptr<Menu> OptionMenu;
	std::shared_ptr<Menu> MultiplayerMenu;
	std::shared_ptr<Menu> ServerList;

	std::shared_ptr<Model> UIBoat;
	glm::mat4 StartMenuMatrix;
	glm::mat4 EndMenuMatrix;
	float ZBobbing = 0.0f;

	//Shaders
	ShaderLoader SL;
	GLuint CubeMapShader;
	GLuint TextShader;

	//World Objects
	CubeMap CM;
	std::shared_ptr<Player> Player0;
	std::shared_ptr<Player> Player1;
	std::shared_ptr<Wave> WaveObj;

	//Entity Vectors
	std::vector<std::shared_ptr<AutoAgent>> EnemyVect;
	std::vector<std::shared_ptr<PickUp>> PickUpVect;

	//Sound
	bool PlayMusic;
	SoundManager sm;

	//Networking
	bool _Connected = false;
	bool _ServerChosen = false;
	EEntityType InstanceType;
};
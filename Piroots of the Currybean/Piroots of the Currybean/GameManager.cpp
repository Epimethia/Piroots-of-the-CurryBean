#include "GameManager.h"
std::shared_ptr<GameManager> GameManager::SceneManagerPtr;

GameManager::~GameManager() {
	Camera::DestroyInstance();
}

GameManager::GameManager() {
}

void GameManager::DrawScene(int _LevelIndex) {
	Level1.Draw();
}

void GameManager::DestroyInstance() {
	SceneManagerPtr = nullptr;
}

void GameManager::GameLoop() {
	InputManager::ProcessKeyInput(Level1.pu);

}

std::shared_ptr<GameManager> GameManager::GetInstance() {
	if (SceneManagerPtr == nullptr) SceneManagerPtr = std::shared_ptr<GameManager>(new GameManager());
	return SceneManagerPtr;
}

void GameManager::Init() {
	Clock::GetInstance();
	GenerateLevels();
	InputManager();
	Camera::GetInstance();
	EntityManager::GetInstance();
	Level1.Init();
}

void GameManager::GenerateLevels() {
	//Start Menu Generation
	std::vector<std::string> String;
	String.push_back("egwge");
	Level1.CM = CubeMap(String);
	//Level1.Wave = Model(WAVE_MODEL, Level1.WaveShader);
}



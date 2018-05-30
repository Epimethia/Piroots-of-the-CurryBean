#include "GameManager.h"
std::shared_ptr<GameManager> GameManager::SceneManagerPtr;
GLuint GameManager::ObjectShaders;

GameManager::~GameManager() {}

GameManager::GameManager() {

}

void GameManager::DrawScene(int _LevelIndex) {

	LevelVect[CurrentLevel].Draw(ObjectShaders, Camera::GetMatrix());
}

void GameManager::DestroyInstance() {
	SceneManagerPtr = nullptr;
}

void GameManager::GameLoop() {
	InputManager::ProcessKeyInput(LevelVect[CurrentLevel].bleh);
}

std::shared_ptr<GameManager> GameManager::GetInstance() {
	if (SceneManagerPtr == nullptr) SceneManagerPtr = std::shared_ptr<GameManager>(new GameManager());
	return SceneManagerPtr;
}

void GameManager::Init() {
	//generating shaders
	ObjectShaders = sl.CreateProgram(VERT_SHADER, FRAG_SHADER);
	GenerateLevels();
	//CInputManager();
	//Camera::GetInstance();
}

void GameManager::GenerateLevels() {

	//Start Menu Generation
	Level Level1;
	LevelVect.push_back(Level1);

}



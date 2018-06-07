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
	GLfloat currentTime = static_cast<GLfloat>(glutGet(GLUT_ELAPSED_TIME));// Get current time.
	currentTime = currentTime; // Convert millisecond to seconds
	GLint currentTimeLoc = glGetUniformLocation(GetCurrentLevel().WaveShader, "currentTime");
	glUniform1f(currentTimeLoc, currentTime);// set value
	InputManager::ProcessKeyInput(Level1.bleh);
}

std::shared_ptr<GameManager> GameManager::GetInstance() {
	if (SceneManagerPtr == nullptr) SceneManagerPtr = std::shared_ptr<GameManager>(new GameManager());
	return SceneManagerPtr;
}

void GameManager::Init() {
	//generating shaders
	GenerateLevels();
	InputManager();
	Camera::GetInstance();
	EntityManager::GetInstance();
	Level1.Init();
}

void GameManager::GenerateLevels() {
	//Start Menu Generation
	Level1.bleh = CUBE(
		PLAYER_SPRITE,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.1f, 0.1f, 0.1f),
		glm::vec3(0.0f, 0.0f, 90.0f)
	);

	std::vector<std::string> String;
	String.push_back("egwge");
	Level1.CM = CubeMap(String);

	//Level1.Player = Model(MODEL_A, Level1.ModelShader);
	Level1.Wave = Model(WAVE_MODEL, Level1.WaveShader);
}



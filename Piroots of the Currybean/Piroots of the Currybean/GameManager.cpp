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
	GLfloat currentTime = glutGet(GLUT_ELAPSED_TIME);// Get current time.
	currentTime = currentTime / 1000; // Convert millisecond to seconds
	GLint currentTimeLoc = glGetUniformLocation(ObjectShaders, "currentTime");
	glUniform1f(currentTimeLoc, currentTime);// set value
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
	InputManager();
	//Camera::GetInstance();
}

void GameManager::GenerateLevels() {

	//Start Menu Generation
	Level Level1;
	Level1.bleh = CUBE(
		PLAYER_SPRITE,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.1f, 0.1f, 0.1f),
		glm::vec3(0.0f, 0.0f, 0.0f)
	);

	Level1.pyramid = PYRAMID(
		PLAYER_SPRITE,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.1f, 0.1f, 0.1f),
		glm::vec3(0.0f, 0.0f, 0.0f)
	);

	Level1.planeboi = PLANE(
		PLAYER_SPRITE,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.0f, 0.0f, 0.0f)
	);
	LevelVect.push_back(Level1);

}



#include "GameManager.h"
std::shared_ptr<GameManager> GameManager::SceneManagerPtr;

GameManager::~GameManager() {
	for (int i = 0; i < static_cast<int>(LevelVect.size()); ++i) {
		LevelVect.pop_back();
	}
	LevelVect.clear();
	sl.~ShaderLoader();
	sl2.~ShaderLoader();
	ObjectShaders = 0;
	Camera::DestroyInstance();

}

GameManager::GameManager() {

}

void GameManager::DrawScene(int _LevelIndex) {

	LevelVect[CurrentLevel].Draw(ObjectShaders, Camera::GetMatrix());
}

void GameManager::DestroyInstance() {
	SceneManagerPtr = nullptr;
}

void GameManager::GameLoop() {
	GLfloat currentTime = static_cast<GLfloat>(glutGet(GLUT_ELAPSED_TIME));// Get current time.
	currentTime = currentTime; // Convert millisecond to seconds
	GLint currentTimeLoc = glGetUniformLocation(GetCurrentLevel().WaveShader, "currentTime");
	glUniform1f(currentTimeLoc, currentTime);// set value
	InputManager::ProcessKeyInput(LevelVect[CurrentLevel].bleh);
}

std::shared_ptr<GameManager> GameManager::GetInstance() {
	if (SceneManagerPtr == nullptr) SceneManagerPtr = std::shared_ptr<GameManager>(new GameManager());
	return SceneManagerPtr;
}

void GameManager::Init() {
	//generating shaders
	VLDEnable();
	ObjectShaders = sl.CreateProgram(VERT_SHADER, FRAG_SHADER);
	VLDDisable();
	GenerateLevels();
	InputManager();
	Camera::GetInstance();
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

	Level1.Player = Model(MODEL_A, Level1.ModelShader);

	LevelVect.push_back(Level1);
}



#include "GameManager.h"
std::shared_ptr<GameManager> GameManager::SceneManagerPtr;

GameManager::~GameManager() {
	Camera::DestroyInstance();
}

GameManager::GameManager() {
}

void GameManager::DrawScene(float _DeltaTime) {

	//Drawing the player
	PlayerObj->Process(_DeltaTime);

	//Drawing all other Entities
	for (auto it : EnemyVect) it->Process(_DeltaTime);

	//Drawing the cube map
	CM.Render(CubeMapShader, Camera::GetMatrix());

	//Drawing the sea
	WaveObj->Process(_DeltaTime);
}

void GameManager::DestroyInstance() {
	SceneManagerPtr = nullptr;
}

void GameManager::GameLoop(float _DeltaTime) {
	//Spawning Enemies
	if (EnemyVect.size() < 10) {
		SpawnTimer += 10.0f * _DeltaTime;
		if (SpawnTimer >= SpawnLimiter) {
			SpawnTimer = 0.0f;
			glm::vec3 SpawnPos = {
				static_cast<float>((rand() % 6000) - 3000),
				static_cast<float>((rand() % 6000) - 3000),
				0.0f
			};
			if ((rand() % 2) == 0) {
				EnemyVect.push_back(std::make_shared<SeekEnemy>(SpawnPos, PlayerObj));
				std::cout << "Spawned Seek Ship\n";
			} else {
				EnemyVect.push_back(std::make_shared<WanderEnemy>(SpawnPos, PlayerObj));
				std::cout << "Spawned Wander Ship\n";
			}
		}
	}
	InputManager::ProcessKeyInput(PlayerObj);

	//Checking Collisions !INEFFICIENT BUT DONT KNOW HOW TO DO QUAD MAPS
	//Checking every entity
	for (unsigned int i = 0; i < EnemyVect.size(); ++i) {

		//If the Entity is an enemy
		if (EnemyVect[i]->Type == SEEK_ENEMY || EnemyVect[i]->Type == WANDER_ENEMY) {
			//Check against every bullet
			for (unsigned int j = 0; j < PlayerObj->GetBulletVect().size(); ++j) {
				if (abs(glm::distance(PlayerObj->GetBulletVect()[j]->GetPos(), EnemyVect[i]->GetPos())) <= 65.0f) {
					//Destroy if colliding with a bullet
					EnemyVect.erase(EnemyVect.begin() + i);
					PlayerObj->GetBulletVect().erase(PlayerObj->GetBulletVect().begin() + j);
					break;
				}
			}
		}
		
		
	}

}

std::shared_ptr<GameManager> GameManager::GetInstance() {
	if (SceneManagerPtr == nullptr) SceneManagerPtr = std::shared_ptr<GameManager>(new GameManager());
	return SceneManagerPtr;
}

void GameManager::Init() {
	srand(static_cast<int>(time(NULL)));
	InputManager();
	Camera::GetInstance();
	EntityManager::GetInstance();
	PlayerObj = std::make_shared<Player>(glm::vec3(1000.0f, 1000.0f, -0.2f));
	WaveObj = std::make_shared<Wave>(glm::vec3(0.0f, 0.0f, 0.0f));
	EnemyVect.push_back(std::make_shared<SeekEnemy>(glm::vec3(0.0f, 0.0f, 0.0f), PlayerObj));

	CubeMapShader = SL.CreateProgram(CUBEMAP_VERT_SHADER, CUBEMAP_FRAG_SHADER);
}





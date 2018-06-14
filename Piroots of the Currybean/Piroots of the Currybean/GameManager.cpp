#include "GameManager.h"
std::shared_ptr<GameManager> GameManager::SceneManagerPtr;

GameManager::~GameManager() {
	Camera::DestroyInstance();
}

GameManager::GameManager() {
	srand(static_cast<int>(time(NULL)));
	InputManager();
	Camera::GetInstance();
	EntityManager::GetInstance();
	PlayerObj = std::make_shared<Player>(glm::vec3(1000.0f, 1000.0f, -0.2f));
	WaveObj = std::make_shared<Wave>(glm::vec3(0.0f, 0.0f, 0.0f));
	CubeMapShader = SL.CreateProgram(CUBEMAP_VERT_SHADER, CUBEMAP_FRAG_SHADER);
	CurrentState = START;
}

void GameManager::DrawMenu() {
}

void GameManager::DrawGame() {
	PlayerObj->Process(DeltaTime);
	//Drawing all other Entities
	for (auto it : EnemyVect) it->Process(DeltaTime);
	for (auto it : PickUpVect) it->Process(DeltaTime);
	CM.Render(CubeMapShader, Camera::GetMatrix());
	WaveObj->Process(DeltaTime);
}

void GameManager::DrawEnd() {

}


void GameManager::DrawScene() {
	switch (CurrentState) {
		case START: {
			DrawMenu();
			break;
		}
		case GAME: {
			DrawGame();
			break;

		}
		case END: {
			DrawEnd();
			break;
		}
		default:break;
	}
}

void GameManager::DestroyInstance() {
	SceneManagerPtr = nullptr;
}

void GameManager::GameLoop(float _DeltaTime) {
	DeltaTime = _DeltaTime;
	//Spawning Enemies
	if (EnemyVect.size() < 1) {
		SpawnTimer += 10.0f * _DeltaTime;
		if (SpawnTimer >= SpawnLimiter) {
			SpawnTimer = 0.0f;
			glm::vec3 SpawnPos = {
				static_cast<float>((rand() % 6000) - 3000),
				static_cast<float>((rand() % 6000) - 3000),
				0.0f
			};
			if ((rand() % 2) == 0) {
				EnemyVect.push_back(std::make_shared<PursueEnemy>(glm::vec3(), PlayerObj));
			} else {
				EnemyVect.push_back(std::make_shared<WanderEnemy>(glm::vec3(), PlayerObj));
			}
		}
	}

	if (PickUpVect.size() < 10) {
		glm::vec3 SpawnPos = {
			static_cast<float>((rand() % 6000) - 3000),
			static_cast<float>((rand() % 6000) - 3000),
			0.0f
		};
		int num = (rand() % 2);
		if (num == 1) {
			PickUpVect.push_back(std::make_shared<PickUp>(SpawnPos, SPECIAL_POWERUP));
		} 
		else if (num == 0) {
			PickUpVect.push_back(std::make_shared<PickUp>(SpawnPos, SPECIAL_POWERUP));
		}
	}
	InputManager::ProcessKeyInput(PlayerObj);

	//Checking Collisions !INEFFICIENT BUT DONT KNOW HOW TO DO QUAD MAPS
	//Checking every entity
	for (unsigned int i = 0; i < EnemyVect.size(); ++i) {
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

	for (unsigned int i = 0; i < PickUpVect.size(); ++i) {
		if (abs(glm::distance(PlayerObj->GetPos(), PickUpVect[i]->GetPos()) <= 100.0f)) {
			PlayerObj->State = PickUpVect[i]->Type;
			PlayerObj->PowerUpDuration = 100.0f;
			PickUpVect.erase(PickUpVect.begin() + i);
		}
	}
}

std::shared_ptr<GameManager> GameManager::GetInstance() {
	if (SceneManagerPtr == nullptr) SceneManagerPtr = std::shared_ptr<GameManager>(new GameManager());
	return SceneManagerPtr;
}




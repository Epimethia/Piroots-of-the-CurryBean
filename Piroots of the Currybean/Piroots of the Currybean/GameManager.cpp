#include "GameManager.h"
std::shared_ptr<GameManager> GameManager::SceneManagerPtr;
GameState GameManager::CurrentState;

GameManager::~GameManager() {
	Camera::DestroyInstance();
	EntityManager::DestroyInstance();
}

GameManager::GameManager() {
	//Seeding the random number generator
	srand(static_cast<int>(time(NULL)));
	//Initializing singletons
	InputManager();
	Camera::GetInstance();
	EntityManager::GetInstance();
	DeltaTime = Clock::GetDeltaTime();

	//Music
	sm.Init();
	sm.LoadAudio();
	sm.PlayBGM();
	PlayMusic = true;

	//Initializing consisten objects
	PlayerObj = std::make_shared<Player>(glm::vec3(0.0f, 1000.0f, -0.2f));
	WaveObj = std::make_shared<Wave>(glm::vec3(0.0f, 0.0f, 0.0f));

	//Other Variables
	CubeMapShader = SL.CreateProgram(CUBEMAP_VERT_SHADER, CUBEMAP_FRAG_SHADER);
	TextShader = SL.CreateProgram(TEXT_VERT_SHADER, TEXT_FRAG_SHADER);
	CurrentState = START;
	Score = 0;

	//initializing Text and menus
	Title0 = std::make_shared<Text>("Piroots of", PIRATEFONT, glm::vec2(20.0f, 630.0f), TextShader, 80);
	Title1 = std::make_shared<Text>("the CurryBeans", PIRATEFONT, glm::vec2(90.0f, 550.0f), TextShader, 80);
	Title2 = std::make_shared<Text>("You Died!", PIRATEFONT, glm::vec2(90.0f, 500.0f), TextShader, 80);
	ScoreText = std::make_shared<Text>(std::to_string(Score), PIRATEFONT, glm::vec2(20.0f, 700.0f), TextShader, 60);

	std::vector<std::string> StartOpt;
	StartOpt.push_back(std::string("Play"));
	StartOpt.push_back(std::string("Option"));
	StartOpt.push_back(std::string("Quit"));

	StartMenu = std::make_shared<Menu>(StartOpt, glm::vec2(90.0f, 300.0f));

	std::vector<std::string> EndOpt;
	EndOpt.push_back(std::string("Retry"));
	EndOpt.push_back(std::string("Main Menu"));
	EndOpt.push_back(std::string("Quit"));

	EndMenu = std::make_shared<Menu>(EndOpt, glm::vec2(90.0f, 280.0f));

	std::vector<std::string> OptOpt;
	OptOpt.push_back(std::string("Background Music: "));
	OptOpt.push_back(std::string("Back--"));

	OptionMenu = std::make_shared<Menu>(OptOpt, glm::vec2(40.0f, 280.0f));

	UIBoat = EntityManager::GetModel(PLAYER_ENTITY);
	glm::mat4 RotationMatrix =
		glm::rotate(glm::mat4(), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::rotate(glm::mat4(), glm::radians(220.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 ScaleMatrix = glm::scale(glm::mat4(), glm::vec3(0.05f, 0.05f, 0.05f));
	StartMenuMatrix = RotationMatrix * ScaleMatrix;

	
	RotationMatrix =
		glm::rotate(glm::mat4(), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::rotate(glm::mat4(), glm::radians(-140.0f), glm::vec3(0.0f, 1.0f, 0.0f))*
		glm::rotate(glm::mat4(), glm::radians(140.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ScaleMatrix = glm::scale(glm::mat4(), glm::vec3(0.05f, 0.05f, 0.05f));
	EndMenuMatrix = RotationMatrix * ScaleMatrix;
}

void GameManager::ToggleMusic() {
	if (PlayMusic == true) {
		PlayMusic = false;
	}
	else  PlayMusic = true;
}

void GameManager::DrawMenu() {
	Camera::GetPos() = { 0.0f, 0.0f, 0.0f };
	CM.Render(CubeMapShader, Camera::GetMatrix());
	glm::mat4 TranslationMatrix = glm::translate(glm::mat4(), glm::vec3(-140.0f + (cos(ZBobbing) * 10.0f), 180.0f, 40.0f + (sin(ZBobbing) * 20.0f)) / 375.0f);
	UIBoat->Render(TranslationMatrix * StartMenuMatrix);
	WaveObj->Process(DeltaTime);
	Title0->Render();
	Title1->Render();
	StartMenu->Process();
	ZBobbing += 0.03f * DeltaTime;
}

void GameManager::DrawGame() {
	PlayerObj->Process(DeltaTime);
	//Drawing all other Entities
	for (auto it : EnemyVect) it->Process(DeltaTime);
	for (auto it : PickUpVect) it->Process(DeltaTime);
	CM.Render(CubeMapShader, Camera::GetMatrix());
	WaveObj->Process(DeltaTime);
	ScoreText->SetText(std::string("SCORE: ") + std::to_string(Score));
	ScoreText->Render();
}

void GameManager::DrawEnd() {
	Camera::GetPos() = { 0.0f, 0.0f, 0.0f };
	CM.Render(CubeMapShader, Camera::GetMatrix());
	glm::mat4 TranslationMatrix = glm::translate(glm::mat4(), glm::vec3(-170.0f + (cos(ZBobbing) * 10.0f), 150.0f, 40.0f + (sin(ZBobbing) * 20.0f)) / 375.0f);
	UIBoat->Render(TranslationMatrix * EndMenuMatrix);
	WaveObj->Process(DeltaTime);
	ScoreText->SetText(std::string("SCORE: ") + std::to_string(Score));
	ScoreText->SetPosition(glm::vec2(90.0f, 450.0f));
	ScoreText->Render();
	Title2->Render();
	EndMenu->Process();
	ZBobbing += 0.03f * DeltaTime;
}

void GameManager::DrawOption() {
	Camera::GetPos() = { 0.0f, 0.0f, 0.0f };
	CM.Render(CubeMapShader, Camera::GetMatrix());
	glm::mat4 TranslationMatrix = glm::translate(glm::mat4(), glm::vec3(-140.0f + (cos(ZBobbing) * 10.0f), 180.0f, 40.0f + (sin(ZBobbing) * 20.0f)) / 375.0f);
	WaveObj->Process(DeltaTime);
	Title0->Render();
	Title1->Render();

	//If BGM is on
	if (PlayMusic == true) OptionMenu->ReplaceOption(0, std::string("Background Music: ON"));
	else OptionMenu->ReplaceOption(0, std::string("Background Music: OFF"));

	OptionMenu->Process();

	ZBobbing += 0.03f * DeltaTime;

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
		case OPTION: {
			DrawOption();
			break;
		}
		default:break;
	}
}

void GameManager::DestroyInstance() {
	SceneManagerPtr = nullptr;
}

void GameManager::GameLoop(float _DeltaTime) {
	if (PlayMusic == true) sm.ResumeBGM();
	else sm.PauseBGM();

	if (CurrentState == START) {
		InputManager::ProcessKeyInput(StartMenu);
		return;
	}
	else if (CurrentState == OPTION) {
		InputManager::ProcessKeyInput(OptionMenu);
		return;
	}
	else if (CurrentState == GAME) {
		//If the player still exists
		if (PlayerObj->State == DEAD) {
			CurrentState = END;
		}
		if (PlayerObj != nullptr) {
			DeltaTime = _DeltaTime;
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
					if ((rand() % 5) == 0) {
						EnemyVect.push_back(std::make_shared<WanderEnemy>(SpawnPos, PlayerObj));
					} else {
						EnemyVect.push_back(std::make_shared<PursueEnemy>(SpawnPos, PlayerObj));
					}
				}
			}

			if (PickUpVect.size() < 3) {
				glm::vec3 SpawnPos = {
					static_cast<float>((rand() % 6000) - 3000),
					static_cast<float>((rand() % 6000) - 3000),
					0.0f
				};
				int num = (rand() % 10);
				if (num < 4) {
					PickUpVect.push_back(std::make_shared<PickUp>(SpawnPos, ATTACK_POWERUP));
				} 
				else if (num < 8) {
					PickUpVect.push_back(std::make_shared<PickUp>(SpawnPos, SPEED_POWERUP));
				}
				else {
					PickUpVect.push_back(std::make_shared<PickUp>(SpawnPos, SPECIAL_POWERUP));
				}
			}
			InputManager::ProcessKeyInput(PlayerObj);

			//Checking every entity
			for (unsigned int i = 0; i < EnemyVect.size(); ++i) {
				//Check against every bullet
				for (unsigned int j = 0; j < PlayerObj->GetBulletVect().size(); ++j) {
					if (abs(glm::distance(PlayerObj->GetBulletVect()[j]->GetPos(), EnemyVect[i]->GetPos())) <= EnemyVect[i]->HitRadius) {
						//Destroy if colliding with a bullet
						EnemyVect.erase(EnemyVect.begin() + i);
						PlayerObj->GetBulletVect().erase(PlayerObj->GetBulletVect().begin() + j);
						Score += 10;
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
	}

	else if (CurrentState == END) {
		InputManager::ProcessKeyInput(EndMenu);
		return;
	}
}

std::shared_ptr<GameManager> GameManager::GetInstance() {
	if (SceneManagerPtr == nullptr) SceneManagerPtr = std::shared_ptr<GameManager>(new GameManager());
	return SceneManagerPtr;
}

void GameManager::RestartGame() {
	EnemyVect.clear();
	PickUpVect.clear();

	PlayerObj->State = NONE;
	PlayerObj->GetPos() = glm::vec3(0.0f, 1000.0f, -0.2f);
	PlayerObj->GetVelocity() = glm::vec3();
	PlayerObj->GetBulletVect().clear();

	Score = 0;
	ScoreText->SetPosition(glm::vec2(20.0f, 700.0f));
}




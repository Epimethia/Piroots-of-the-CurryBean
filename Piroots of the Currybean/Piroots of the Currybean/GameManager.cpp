#include "GameManager.h"
std::shared_ptr<GameManager> GameManager::SceneManagerPtr;
GameState GameManager::CurrentState;

GameManager::~GameManager() {
	Camera::DestroyInstance();
	EntityManager::DestroyInstance();
	sm.~SoundManager();
}

GameManager::GameManager() {
	//Seeding the random number generator
	srand(static_cast<int>(time(NULL)));
	InputManager();
	Camera::GetInstance();
	EntityManager::GetInstance();
	DeltaTime = Clock::GetDeltaTime();
	ServerManager::GetInstance();

	#pragma region Music
	sm.Init();
	sm.LoadAudio();
	sm.PlayBGM();
	PlayMusic = true;
	#pragma endregion


	//Initializing consistent objects
	PlayerObj = std::make_shared<Player>(glm::vec3(0.0f, 1000.0f, -0.2f));
	WaveObj = std::make_shared<Wave>(glm::vec3(0.0f, 0.0f, 0.0f));

	//Other Variables
	CubeMapShader = SL.CreateProgram(CUBEMAP_VERT_SHADER, CUBEMAP_FRAG_SHADER);
	TextShader = SL.CreateProgram(TEXT_VERT_SHADER, TEXT_FRAG_SHADER);
	CurrentState = START_MENU;
	Score = 0;

	#pragma region Text and Menu Initializing
	//initializing Text and menus
	Title0 = std::make_shared<Text>("Piroots of", PIRATEFONT, glm::vec2(20.0f, 630.0f), TextShader, 80);
	Title1 = std::make_shared<Text>("the CurryBeans", PIRATEFONT, glm::vec2(90.0f, 550.0f), TextShader, 80);
	EndGameTitle = std::make_shared<Text>("You Died!", PIRATEFONT, glm::vec2(90.0f, 500.0f), TextShader, 80);
	MultiplayerTitle0 = std::make_shared<Text>("Multiplayer", PIRATEFONT, glm::vec2(20.0f, 630.0f), TextShader, 75);
	MultiplayerTitle1 = std::make_shared<Text>("Server List", PIRATEFONT, glm::vec2(90.0f, 540.0f), TextShader, 50);
	ScoreText = std::make_shared<Text>("SCORE: 0", PIRATEFONT, glm::vec2(20.0f, 700.0f), TextShader, 60);
	WaitingForParty = std::make_shared<Text>("Waiting for Party...", PIRATEFONT, glm::vec2(200.0f, 570.0f), TextShader, 40);
	#pragma endregion


	#pragma region StartMenu
	std::vector<std::string> StartOpt;
	StartOpt.push_back("Singleplayer");
	StartOpt.push_back("Multiplayer");
	StartOpt.push_back("Option");
	StartOpt.push_back("Quit");

	StartMenu = std::make_shared<Menu>(StartOpt, glm::vec2(90.0f, 300.0f));
	#pragma endregion

	#pragma region EndMenu
	std::vector<std::string> EndOpt;
	EndOpt.push_back("Retry");
	EndOpt.push_back("Main Menu");
	EndOpt.push_back("Quit");

	EndMenu = std::make_shared<Menu>(EndOpt, glm::vec2(90.0f, 280.0f));
	#pragma endregion

	#pragma region OptionMenu
	std::vector<std::string> OptOpt;
	OptOpt.push_back("");
	OptOpt.push_back("Back");

	OptionMenu = std::make_shared<Menu>(OptOpt, glm::vec2(40.0f, 280.0f));
	#pragma endregion

	#pragma region MultiplayerMenu
	std::vector<std::string> MultiOpt;
	MultiOpt.push_back("Host Lobby");
	MultiOpt.push_back("Join Lobby");
	MultiOpt.push_back("Back");

	MultiplayerMenu = std::make_shared<Menu>(MultiOpt, glm::vec2(90.0f, 300.0f));
	#pragma endregion

	#pragma region ServerList
	std::vector<std::string> ServerListOpt;
	ServerListOpt.push_back("----");
	ServerListOpt.push_back("----");
	ServerListOpt.push_back("----");
	ServerListOpt.push_back("----");
	ServerListOpt.push_back("----");
	ServerListOpt.push_back("----");
	ServerListOpt.push_back("Back");
	ServerList = std::make_shared<Menu>(ServerListOpt, glm::vec2(90.0f, 460.0f));
	#pragma endregion

	#pragma region Aesthetic Boat Model
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
	#pragma endregion

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
	ZBobbing += 0.03f * DeltaTime;

	WaveObj->Process(DeltaTime);

	Title0->Render();
	Title1->Render();
	StartMenu->Render();
}

void GameManager::DrawGame() {
	PlayerObj->Process(DeltaTime);
	//Drawing all other Entities
	for (auto it : EnemyVect) it->Process(DeltaTime);
	for (auto it : PickUpVect) it->Process(DeltaTime);
	CM.Render(CubeMapShader, Camera::GetMatrix());
	WaveObj->Process(DeltaTime);
	ScoreText->Render();
}

void GameManager::DrawEnd() {
	Camera::GetPos() = { 0.0f, 0.0f, 0.0f };
	CM.Render(CubeMapShader, Camera::GetMatrix());
	glm::mat4 TranslationMatrix = glm::translate(glm::mat4(), glm::vec3(-170.0f + (cos(ZBobbing) * 10.0f), 150.0f, 40.0f + (sin(ZBobbing) * 20.0f)) / 375.0f);
	UIBoat->Render(TranslationMatrix * EndMenuMatrix);
	WaveObj->Process(DeltaTime);
	ScoreText->SetPosition(glm::vec2(90.0f, 450.0f));
	ScoreText->Render();
	EndGameTitle->Render();
	EndMenu->Render();
	ZBobbing += 0.03f * DeltaTime;
}

void GameManager::DrawOption() {
	Camera::GetPos() = { 0.0f, 0.0f, 0.0f };
	CM.Render(CubeMapShader, Camera::GetMatrix());
	glm::mat4 TranslationMatrix = glm::translate(glm::mat4(), glm::vec3(-140.0f + (cos(ZBobbing) * 10.0f), 180.0f, 40.0f + (sin(ZBobbing) * 20.0f)) / 375.0f);
	WaveObj->Process(DeltaTime);
	Title0->Render();
	Title1->Render();
	OptionMenu->Render();
	ZBobbing += 0.03f * DeltaTime;
}

void GameManager::DrawServerOption() {
	CM.Render(CubeMapShader, Camera::GetMatrix());
	WaveObj->Process(DeltaTime);
	MultiplayerTitle0->Render();
	MultiplayerMenu->Render();
}

void GameManager::DrawHostLobby() {
	CM.Render(CubeMapShader, Camera::GetMatrix());
	WaveObj->Process(DeltaTime);
	MultiplayerTitle0->Render();
	WaitingForParty->Render();
}

void GameManager::DrawClientLobby() {
	CM.Render(CubeMapShader, Camera::GetMatrix());
	WaveObj->Process(DeltaTime);
	MultiplayerTitle0->Render();
	WaitingForParty->Render();
}

void GameManager::DrawServerSelect() {
	CM.Render(CubeMapShader, Camera::GetMatrix());
	WaveObj->Process(DeltaTime);
	MultiplayerTitle0->Render();
	MultiplayerTitle1->Render();
	ServerList->Render();
}

void GameManager::DrawScene() {
	switch (CurrentState) {
		case START_MENU: {
			DrawMenu();
			break;
		}
		case GAME_PLAY: {
			DrawGame();
			break;
		}
		case END_MENU: {
			DrawEnd();
			break;
		}
		case OPTION_MENU: {
			DrawOption();
			break;
		}
		case MULTIPLAYER_LOBBY: {
			DrawServerOption();
			break;
		}
		case HOST_LOBBY: {
			ServerManager::GetInstance();
			DrawHostLobby();
			break;
		}
		case SERVER_SELECT: {
			DrawServerSelect();
			break;
		}
		case CLIENT_LOBBY: {
			DrawClientLobby();
			break;
		}
		default:break;
	}
}

void GameManager::DestroyInstance() {
	SceneManagerPtr = nullptr;
}

void GameManager::GameLoop(float _DeltaTime) {
	/*if (PlayMusic == true) sm.ResumeBGM();
	else sm.PauseBGM();*/

	if (CurrentState == START_MENU) {
		int TempOutput = NULL;
		StartMenu->Process(TempOutput);
		InputManager::ProcessKeyInput();

		switch (TempOutput) {
			case 0: {
				CurrentState = GAME_PLAY;
				break;
			}
			case 1: {
				MultiplayerTitle0->SetText("Multiplayer");
				CurrentState = MULTIPLAYER_LOBBY;
				break;
			}
			case 2: {
				CurrentState = OPTION_MENU;
				break;
			}
			case 3: {
				glutLeaveMainLoop();
				break;
			}
			default:break;
		}
		return;
	}

	else if (CurrentState == OPTION_MENU) {
		//If BGM is on
		if (PlayMusic == true) OptionMenu->ReplaceOption(0, "Background Music: ON");
		else OptionMenu->ReplaceOption(0, "Background Music: OFF");
		int TempOutput = NULL;
		OptionMenu->Process(TempOutput);
		InputManager::ProcessKeyInput();

		switch (TempOutput) {
			case 0: {
				ToggleMusic();
				break;
			}
			case 1: {
				CurrentState = START_MENU;
				break;
			}

			default:break;
		}
		return;

	}

	else if (CurrentState == END_MENU) {
		int TempOutput = NULL;
		EndMenu->Process(TempOutput);
		InputManager::ProcessKeyInput();

		switch (TempOutput) {
			case 0: {
				CurrentState = GAME_PLAY;
				RestartGame();
				break;
			}
			case 1: {
				CurrentState = START_MENU;
				RestartGame();
				break;
			}
			case 2: {
				glutLeaveMainLoop();
				break;
			}
			default:break;
		}
		return;
	}

	else if (CurrentState == MULTIPLAYER_LOBBY) {
		int TempOutput = NULL;
		MultiplayerMenu->Process(TempOutput);
		InputManager::ProcessKeyInput();

		switch (TempOutput) {
			case 0: {
				MultiplayerTitle0->SetText("Multiplayer - Host");
				CurrentState = HOST_LOBBY;
				break;
			}
			case 1: {
				MultiplayerTitle0->SetText("Multiplayer - Client");
				CurrentState = SERVER_SELECT;
				break;
			}
			case 2: {
				CurrentState = START_MENU;
				break;
			}
			default:break;
		}
		return;
	}

	else if (CurrentState == SERVER_SELECT) {
		if (_Connected == false) {
			ServerManager::GetInstance()->StartClient();
			_Connected = true;
		}

		if (_ServerChosen == false) {
			int TempOutput = NULL;
			std::string LastAddr;
			std::string ServerAddr;
			int NumOptions = ServerManager::GetInstance()->GetServerList().size();
			for (unsigned int i = 0; i < NumOptions; ++i) {
				ServerAddr = ToString(ServerManager::GetInstance()->GetServerList()[i]);
				if (ServerAddr != LastAddr) {
					ServerList->ReplaceOption(i, ServerAddr);
					LastAddr = ServerAddr;
				}
			}
			ServerList->Process(TempOutput);
			InputManager::ProcessKeyInput();

			switch (TempOutput) {
			case 0: {
				//ServerPort 0
				if (TempOutput <= NumOptions - 1) {
					ServerManager::GetInstance()->SelectServer(0);
					CurrentState = CLIENT_LOBBY;
					_ServerChosen = true;
				};
				break;
			}
			case 1: {
				//ServerPort 1
				if (TempOutput <= NumOptions - 1) {
					ServerManager::GetInstance()->SelectServer(1);
					CurrentState = CLIENT_LOBBY;
					_ServerChosen = true;
				};
				break;
			}
			case 2: {
				//ServerPort 2
				if (TempOutput <= NumOptions - 1) {
					ServerManager::GetInstance()->SelectServer(2);
					CurrentState = CLIENT_LOBBY;
					_ServerChosen = true;
				};
				break;
			}
			case 3: {
				if (TempOutput <= NumOptions - 1) {
					ServerManager::GetInstance()->SelectServer(3);
					CurrentState = CLIENT_LOBBY;
					_ServerChosen = true;
				};
				break;
			}
			case 4: {
				if (TempOutput <= NumOptions - 1) {
					ServerManager::GetInstance()->SelectServer(4);
					CurrentState = CLIENT_LOBBY;
					_ServerChosen = true;
				};
				break;
			}
			case 5: {
				if (TempOutput <= NumOptions - 1) {
					ServerManager::GetInstance()->SelectServer(5);
					CurrentState = CLIENT_LOBBY;
					_ServerChosen = true;
				};
				break;
			}
			case 6: {
				MultiplayerTitle0->SetText("Multiplayer");
				CurrentState = MULTIPLAYER_LOBBY;
				break;
			}
			default:break;
			}
		}
		return;
	}

	else if (CurrentState == CLIENT_LOBBY) {
		ServerManager::GetInstance()->ProcessServer();
	}

	else if (CurrentState == GAME_PLAY) {
		//If the player still exists
		if (PlayerObj->State == DEAD) {
			CurrentState = END_MENU;
			return;
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
					}
					else {
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
						ScoreText->SetText("SCORE: " + std::to_string(Score));
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
	ScoreText->SetText("SCORE: " + std::to_string(Score));
}




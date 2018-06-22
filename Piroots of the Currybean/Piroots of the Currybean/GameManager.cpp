#include "GameManager.h"

std::shared_ptr<GameManager> GameManager::SceneManagerPtr;
GameState GameManager::CurrentState;

GameManager::~GameManager() {
	Camera::DestroyInstance();
	EntityManager::DestroyInstance();
	sm.~SoundManager();
	ServerManager::GetInstance()->StopNetworkEntity();
	ServerManager::DestroyInstance();
};

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
	Player0 = std::make_shared<Player>(glm::vec3(0.0f, 1000.0f, -0.2f));
	Player1 = std::make_shared<Player>(glm::vec3(0.0f, 0.0f, -0.2f));
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
	ServerListMenu = std::make_shared<Menu>(ServerListOpt, glm::vec2(90.0f, 460.0f));
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

};

void GameManager::ToggleMusic() {
	if (PlayMusic == true) {
		PlayMusic = false;
	} else  PlayMusic = true;
};

void GameManager::DrawMenu() {
	Camera::GetPos() = { 0.0f, 0.0f, 0.0f };
	CM.Render(CubeMapShader, Camera::GetMatrix());

	glm::mat4 TranslationMatrix = glm::translate(glm::mat4(), glm::vec3(-140.0f + (cos(ZBobbing) * 10.0f), 180.0f, 40.0f + (sin(ZBobbing) * 20.0f)) / 375.0f);
	UIBoat->Render(TranslationMatrix * StartMenuMatrix);
	ZBobbing += 2.0f * DeltaTime;

	WaveObj->Process(DeltaTime);

	Title0->Render();
	Title1->Render();
	StartMenu->Render();
};

void GameManager::DrawGame() {
	Player0->Process(DeltaTime);
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
	ZBobbing += 2.0f * DeltaTime;
}

void GameManager::DrawOption() {
	Camera::GetPos() = { 0.0f, 0.0f, 0.0f };
	CM.Render(CubeMapShader, Camera::GetMatrix());
	glm::mat4 TranslationMatrix = glm::translate(glm::mat4(), glm::vec3(-140.0f + (cos(ZBobbing) * 10.0f), 180.0f, 40.0f + (sin(ZBobbing) * 20.0f)) / 375.0f);
	WaveObj->Process(DeltaTime);
	Title0->Render();
	Title1->Render();
	OptionMenu->Render();
	ZBobbing += 2.0f * DeltaTime;
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
	MultiplayerTitle1->Render();
	ServerListMenu->Render();
}

void GameManager::DrawServerSelect() {
	CM.Render(CubeMapShader, Camera::GetMatrix());
	WaveObj->Process(DeltaTime);
	MultiplayerTitle0->Render();
	MultiplayerTitle1->Render();
	ServerListMenu->Render();
}

void GameManager::DrawMultiplayerGame() {
	Player0->Process(DeltaTime);
	Player1->Process(DeltaTime);
	for (auto it : EnemyVect) it->Process(DeltaTime);
	for (auto it : PickUpVect) it->Process(DeltaTime);
	CM.Render(CubeMapShader, Camera::GetMatrix());
	WaveObj->Process(DeltaTime);
	ScoreText->Render();
}

void GameManager::GameLoop() {
	//If the player still exists
	if (Player0->State == DEAD) {
		CurrentState = END_MENU;
		return;
	}
	if (Player0 != nullptr) {
		//Spawning Enemies
		if (EnemyVect.size() < 10) {
			SpawnTimer += 10.0f * DeltaTime;
			if (SpawnTimer >= SpawnLimiter) {
				SpawnTimer = 0.0f;
				glm::vec3 SpawnPos = {
					static_cast<float>((rand() % 6000) - 3000),
					static_cast<float>((rand() % 6000) - 3000),
					0.0f
				};
				if ((rand() % 5) == 0) {
					EnemyVect.push_back(std::make_shared<WanderEnemy>(SpawnPos, Player0));
				} else {
					EnemyVect.push_back(std::make_shared<PursueEnemy>(SpawnPos, Player0));
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
			} else if (num < 8) {
				PickUpVect.push_back(std::make_shared<PickUp>(SpawnPos, SPEED_POWERUP));
			} else {
				PickUpVect.push_back(std::make_shared<PickUp>(SpawnPos, SPECIAL_POWERUP));
			}
		}
		InputManager::ProcessKeyInput(Player0);

		//Checking every entity
		for (unsigned int i = 0; i < EnemyVect.size(); ++i) {
			//Check against every bullet
			for (unsigned int j = 0; j < Player0->GetBulletVect().size(); ++j) {
				if (abs(glm::distance(Player0->GetBulletVect()[j]->GetPos(), EnemyVect[i]->GetPos())) <= EnemyVect[i]->HitRadius) {
					//Destroy if colliding with a bullet
					EnemyVect.erase(EnemyVect.begin() + i);
					Player0->GetBulletVect().erase(Player0->GetBulletVect().begin() + j);
					Score += 10;
					ScoreText->SetText("SCORE: " + std::to_string(Score));
					break;
				}
			}
		}

		for (unsigned int i = 0; i < PickUpVect.size(); ++i) {
			if (abs(glm::distance(Player0->GetPos(), PickUpVect[i]->GetPos()) <= 100.0f)) {
				Player0->State = PickUpVect[i]->Type;
				Player0->PowerUpDuration = 100.0f;
				PickUpVect.erase(PickUpVect.begin() + i);
			}
		}
	}
}

void GameManager::DrawScene() {
	switch (CurrentState) {
		case START_MENU:
			DrawMenu();
			break;
		case SINGLEPLAYER_GAME:
			DrawGame();
			break;
		case END_MENU:
			DrawEnd();
			break;
		case OPTION_MENU:
			DrawOption();
			break;
		case MULTIPLAYER_MENU:
			DrawServerOption();
			break;
		case HOST_LOBBY:
			DrawHostLobby();
			break;
		case CLIENT_GAME:
			DrawMultiplayerGame();
			break;
		case HOST_GAME:
			DrawMultiplayerGame();
			break;
		case CLIENT_LOBBY:
			DrawClientLobby();
			break;
		default:break;
	}
}

void GameManager::DestroyInstance() {
	SceneManagerPtr = nullptr;
}

void GameManager::ProcessGame(float _DeltaTime) {
	DeltaTime = _DeltaTime;
	if (PlayMusic == true) sm.ResumeBGM();
	else sm.PauseBGM();

	switch (CurrentState) {

		case START_MENU: {
			int TempOutput = NULL;
			StartMenu->Process(TempOutput);
			InputManager::ProcessKeyInput();

			switch (TempOutput) {
				case 0:
					CurrentState = SINGLEPLAYER_GAME;
					break;
				case 1:
					MultiplayerTitle0->SetText("Multiplayer");
					CurrentState = MULTIPLAYER_MENU;
					break;
				case 2:
					CurrentState = OPTION_MENU;
					break;
				case 3:
					glutLeaveMainLoop();
					break;
				default:break;
			}
			break;
		}

		case SINGLEPLAYER_GAME: {
			GameLoop();
			break;
		}

		case OPTION_MENU: {
			//If BGM is on
			if (PlayMusic == true) OptionMenu->ReplaceOption(0, "Background Music: ON");
			else OptionMenu->ReplaceOption(0, "Background Music: OFF");

			int TempOutput = NULL;
			OptionMenu->Process(TempOutput);
			InputManager::ProcessKeyInput();

			switch (TempOutput) {
				case 0:
					ToggleMusic();
					break;
				case 1:
					CurrentState = START_MENU;
					break;
				default:break;
			}
			break;
		}

		case MULTIPLAYER_MENU: {
			int TempOutput = NULL;
			MultiplayerMenu->Process(TempOutput);
			InputManager::ProcessKeyInput();

			switch (TempOutput) {
				case 0: {
					std::cout << "Host Selected\n";
					ServerManager::GetInstance()->StartNetwork(SERVER);
					CurrentState = HOST_LOBBY;
					break;
				}
				case 1: {
					std::cout << "Client Selected\n";
					ServerManager::GetInstance()->StartNetwork(CLIENT);
					CurrentState = CLIENT_LOBBY;
					break;
				}
				case 2: {
					CurrentState = START_MENU;
					return;
				}
				default:break;
			}
			break;
		}

		case HOST_LOBBY: {
			InputManager::ProcessKeyInput(Player0);
			ServerManager::GetInstance()->ProcessNetworkEntity();
			if (ServerManager::GetInstance()->LobbyReady()) {
				CurrentState = HOST_GAME;
			}
			//Player1->GetPos() = ServerManager::GetInstance()->GetPlayerPos();
			break;
		}

		case HOST_GAME: {
			InputManager::ProcessKeyInput(Player0);
			ServerManager::GetInstance()->ProcessNetworkEntity();
			std::cout << ServerManager::GetInstance()->GetPlayerPos().x;
			Player1->GetPos() = ServerManager::GetInstance()->GetPlayerPos();
			break;
		}

		case CLIENT_GAME: {
			GameLoop();
			std::stringstream ss;
			ss << std::fixed << std::setprecision(1);
			ss << Player0->GetPos().x << ' ' << Player0->GetPos().y;

			TPacket Data;
			Data.Serialize(PLAYERPOS, const_cast<char*>(ss.str().c_str()));
			ServerManager::GetInstance()->SendPacket(Data.PacketData);
			break;
		}

		case CLIENT_LOBBY: {
			//Getting the server list and putting them into the menu options
			ServerManager::GetInstance()->ProcessNetworkEntity();
			int NumServers = ServerManager::GetInstance()->GetServerAddrs().size();
			for (int i = 0; i < NumServers; ++i) {
				ServerListMenu->ReplaceOption(i, ToString(ServerManager::GetInstance()->GetServerAddrs()[i]));
			}
			int TempOutput = NULL;
			ServerListMenu->Process(TempOutput);
			InputManager::ProcessKeyInput();

			if (TempOutput < NumServers) {
				ServerManager::GetInstance()->SelectServer(TempOutput);
				
				CurrentState = CLIENT_GAME;
			}
			else if (TempOutput == 6) {
				ServerManager::GetInstance()->StopNetworkEntity();
				CurrentState = MULTIPLAYER_MENU;
			}
			break;
			
		}

		case END_MENU: {
			int TempOutput = NULL;
			EndMenu->Process(TempOutput);
			InputManager::ProcessKeyInput();
			switch (TempOutput) {
				case 0:
					CurrentState = SINGLEPLAYER_GAME;
					RestartGame();
					break;
				case 1:
					CurrentState = START_MENU;
					RestartGame();
					break;
				case 2:
					glutLeaveMainLoop();
					break;
				default:break;
			}

			break;
		}
		default: break;
	}
}

std::shared_ptr<GameManager> GameManager::GetInstance() {
	if (SceneManagerPtr == nullptr) SceneManagerPtr = std::shared_ptr<GameManager>(new GameManager());
	return SceneManagerPtr;
}

void GameManager::RestartGame() {
	EnemyVect.clear();
	PickUpVect.clear();

	Player0->State = NONE;
	Player0->GetPos() = glm::vec3(0.0f, 1000.0f, -0.2f);
	Player0->GetVelocity() = glm::vec3();
	Player0->GetBulletVect().clear();

	Score = 0;
	ScoreText->SetPosition(glm::vec2(20.0f, 700.0f));
	ScoreText->SetText("SCORE: " + std::to_string(Score));
}




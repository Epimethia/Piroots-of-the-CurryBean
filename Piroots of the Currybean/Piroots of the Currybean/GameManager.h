#pragma once
#include "Resource.h"
#include "ShaderLoader.h"
#include "Entity.h"
#include "Camera.h"
#include "InputManager.h"
#include "Mesh.h"
#include "CubeMap.h"
#include "EntityManager.h"

struct Level {
	CUBE bleh;
	//PYRAMID pyramid;
	Model Player;
	Model Wave;
	CubeMap CM;

	//UPDATE SHADERLOADER TO NOT REQUIRE MULTIPLE LOADERS
	//-------------------------------------------------------
	std::shared_ptr<ShaderLoader> SL0 = std::make_shared<ShaderLoader>();
	std::shared_ptr<ShaderLoader> SL1 = std::make_shared<ShaderLoader>();
	std::shared_ptr<ShaderLoader> SL2 = std::make_shared<ShaderLoader>();
	std::shared_ptr<ShaderLoader> SL3 = std::make_shared<ShaderLoader>();

	GLuint WaveShader;
	GLuint ModelShader;
	GLuint ObjectShader;
	GLuint CubeMapShader;
	//-------------------------------------------------------

	std::vector<std::shared_ptr<Entity>> EntityVect;
	void Init() {
		WaveShader = SL0->CreateProgram(WAVE_VERT_SHADER, WAVE_FRAG_SHADER);
		ModelShader = SL1->CreateProgram(MODEL_VERT_SHADER, MODEL_FRAG_SHADER);
		ObjectShader = SL2->CreateProgram(VERT_SHADER, FRAG_SHADER);
		CubeMapShader = SL3->CreateProgram(CUBEMAP_VERT_SHADER, CUBEMAP_FRAG_SHADER);

		for (int i = 0; i < 10; ++i) {
			auto cube = std::make_shared<CUBE>(
				PLAYER_SPRITE,
				glm::vec3(i * 100.0f, 0.0f, 0.0f),
				glm::vec3(0.1f, 0.1f, 0.1f),
				glm::vec3(0.0f, 0.0f, 90.0f)
			);
			
			auto pyr = std::make_shared<PYRAMID>(
				PLAYER_SPRITE,
				glm::vec3(0.0f, 0.0f, i * 100.0f),
				glm::vec3(0.3f, 0.3f, 0.3f),
				glm::vec3(0.0f, 0.0f, 0.0f)
			);
			EntityVect.push_back(cube);
			EntityVect.push_back(pyr);
		}

	}

	void Draw() {
		CM.Render(CubeMapShader, Camera::GetMatrix());
		bleh.Render(ObjectShader, Camera::GetMatrix());
		//pyramid.Render(ObjectShader, Camera::GetMatrix());

		for (auto it : EntityVect) {
			it->Render(ObjectShader, Camera::GetMatrix());
		}

		glFrontFace(GL_CCW);
		//Player.Render(ModelShader);
		Wave.Render(WaveShader);
		glFrontFace(GL_CW);
	}
	~Level() {

	}
};

//Singleton scene manager
class GameManager {
public:
	~GameManager();
	void DrawScene(int _LevelIndex);
	static void DestroyInstance();
	static std::shared_ptr<GameManager> GetInstance();

	void Init();
	void GameLoop();
	//CEntity PlayerObj;
	Level& GetCurrentLevel() { return Level1; };

private:
	GameManager();
	static std::shared_ptr<GameManager> SceneManagerPtr;

	void GenerateLevels();
	//std::vector<Level> LevelVect;
	Level Level1;

	int CurrentLevel = 0;
};



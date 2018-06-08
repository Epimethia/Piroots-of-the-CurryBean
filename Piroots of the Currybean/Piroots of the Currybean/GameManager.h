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
	Model Player;
	Model Wave;
	CubeMap CM;
	std::shared_ptr<Entity> Cube;

	std::shared_ptr<ShaderLoader> SL = std::make_shared<ShaderLoader>();

	GLuint WaveShader;
	GLuint ModelShader;
	GLuint ObjectShader;
	GLuint CubeMapShader;

	/*
	std::shared_ptr<GLuint> WaveShader;
	std::shared_ptr<GLuint>  ModelShader;
	std::shared_ptr<GLuint>  ObjectShader;
	std::shared_ptr<GLuint>  CubeMapShader;
	*/

	std::vector<std::shared_ptr<Entity>> EntityVect;
	void Init() {
		WaveShader = SL->CreateProgram(WAVE_VERT_SHADER, WAVE_FRAG_SHADER);
		ModelShader = SL->CreateProgram(MODEL_VERT_SHADER, MODEL_FRAG_SHADER);
		ObjectShader = SL->CreateProgram(VERT_SHADER, FRAG_SHADER);
		CubeMapShader = SL->CreateProgram(CUBEMAP_VERT_SHADER, CUBEMAP_FRAG_SHADER);
		Cube = std::make_shared<Entity>(
			CUBE_MESH,
			PLAYER_SPRITE,
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.1f, 0.1f, 0.1f),
			glm::vec3(0.0f, 0.0f, 0.0f)
		);
	}
	

	void Draw() {
		CM.Render(CubeMapShader, Camera::GetMatrix());
		Cube->Render(ModelShader, Camera::GetMatrix());
		for (auto it : EntityVect) {
			it->Render(ObjectShader, Camera::GetMatrix());
		}
		glFrontFace(GL_CCW);
		Player.Render(ModelShader);
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
	static void DestroyInstance();
	static std::shared_ptr<GameManager> GetInstance();

	void DrawScene(int _LevelIndex);
	void Init();
	void GameLoop();
	Level Level1;


private:
	GameManager();
	static std::shared_ptr<GameManager> SceneManagerPtr;
	void GenerateLevels();
	int CurrentLevel = 0;
};



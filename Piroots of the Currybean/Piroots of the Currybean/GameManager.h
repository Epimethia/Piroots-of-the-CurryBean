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
	Model Pug;
	CubeMap CM;
	std::shared_ptr<PickUp> pu;
	std::shared_ptr<PickUp> pu1;
	std::shared_ptr<Entity> Cube;

	std::shared_ptr<ShaderLoader> SL = std::make_shared<ShaderLoader>();

	GLuint WaveShader;
	GLuint ModelShader;
	GLuint ObjectShader;
	GLuint CubeMapShader;

	std::vector<std::shared_ptr<Entity>> EntityVect;
	void Init() {
		WaveShader = SL->CreateProgram(WAVE_VERT_SHADER, WAVE_FRAG_SHADER);
		ModelShader = SL->CreateProgram(MODEL_VERT_SHADER, MODEL_FRAG_SHADER);
		ObjectShader = SL->CreateProgram(VERT_SHADER, FRAG_SHADER);
		CubeMapShader = SL->CreateProgram(CUBEMAP_VERT_SHADER, CUBEMAP_FRAG_SHADER);

		pu = std::make_shared<PickUp>(glm::vec3(-375.0f, -375.0f, 0.0f), ObjectShader);
		//EntityVect.push_back(std::make_shared<PickUp>(glm::vec3(0.0f, 0.0f, 0.0f), ObjectShader));

		EntityVect.push_back(std::make_shared<Wave>(glm::vec3(0.0f, 0.0f, 0.0f), WaveShader));
		
	}
	

	void Draw() {
		CM.Render(CubeMapShader, Camera::GetMatrix());

		for (auto it : EntityVect) {
			it->Process(Camera::GetMatrix());
		}
		pu->Process(Camera::GetMatrix());
		//pu1->Process(Camera::GetMatrix());
		
		glFrontFace(GL_CCW);
		

		//Pug.Render(ModelShader);
		//Wave.Render(WaveShader);
		glFrontFace(GL_CW);
		//Cube->Process(Camera::GetMatrix());

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



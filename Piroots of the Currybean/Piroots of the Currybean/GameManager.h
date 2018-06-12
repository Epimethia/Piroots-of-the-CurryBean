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
	std::shared_ptr<AutoAgent> pu;
	std::shared_ptr<PickUp> pu1;
	std::shared_ptr<Entity> Cube;

	std::shared_ptr<ShaderLoader> SL = std::make_shared<ShaderLoader>();

	GLuint WaveShader;
	GLuint ModelShader;
	GLuint ObjectShader;
	GLuint CubeMapShader;

	void CreateBullet() {
		BulletVect.push_back(std::make_shared<Bullet>(
			pu->GetVelocity(), 
			glm::vec3(pu->GetPos().x, pu->GetPos().y, pu->GetPos().z + 20.0f), 
			ObjectShader)
		);
	}

	std::vector<std::shared_ptr<Entity>> EntityVect;
	std::vector<std::shared_ptr<Bullet>> BulletVect;
	void Init() {
		WaveShader = SL->CreateProgram(WAVE_VERT_SHADER, WAVE_FRAG_SHADER);
		ModelShader = SL->CreateProgram(MODEL_VERT_SHADER, MODEL_FRAG_SHADER);
		ObjectShader = SL->CreateProgram(VERT_SHADER, FRAG_SHADER);
		CubeMapShader = SL->CreateProgram(CUBEMAP_VERT_SHADER, CUBEMAP_FRAG_SHADER);

		pu = std::make_shared<AutoAgent>(glm::vec3(1000.0f, 1000.0f, 0.0f), ModelShader);
		//EntityVect.push_back(std::make_shared<PickUp>(glm::vec3(0.0f, 0.0f, 0.0f), ObjectShader));

		EntityVect.push_back(std::make_shared<SmallEnemy>(glm::vec3(0.0f, 0.0f, 30.0f), ModelShader, pu));
		EntityVect.push_back(std::make_shared<Wave>(glm::vec3(0.0f, 0.0f, 0.0f), WaveShader));
		EntityVect.push_back(std::make_shared<PickUp>(glm::vec3(-375.0f, -375.0f, 60.0f), ObjectShader));
		
	}
	
	void Draw() {
		float DeltaTime = Clock::GetDeltaTime();
		CM.Render(CubeMapShader, Camera::GetMatrix());
		for (auto it : EntityVect) {
			it->Process(DeltaTime);
		}
		for (unsigned int i = 0; i < BulletVect.size(); ++i) {
			if (BulletVect[i]->GetPos().y >= 3300.0f || 
				BulletVect[i]->GetPos().y <= -3300.0f ||
				BulletVect[i]->GetPos().x >= 3300.0f ||
				BulletVect[i]->GetPos().x <= -3300.0f) {
				BulletVect.erase(BulletVect.begin(), BulletVect.begin() + i);
			}
			else BulletVect[i]->Process(DeltaTime);
		}
		pu->Process(DeltaTime);
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
};



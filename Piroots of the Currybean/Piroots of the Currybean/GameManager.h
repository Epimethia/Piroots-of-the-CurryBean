#pragma once
#include "Resource.h"
#include "ShaderLoader.h"
#include "Entity.h"
#include "Camera.h"
#include "InputManager.h"
#include "Mesh.h"

struct Level {
	CUBE bleh;
	PLANE planeboi;
	PYRAMID pyramid;
	Model Player;
	std::shared_ptr<ShaderLoader> SL0 = std::make_shared<ShaderLoader>();
	std::shared_ptr<ShaderLoader> SL1 = std::make_shared<ShaderLoader>();
	std::shared_ptr<ShaderLoader> SL2 = std::make_shared<ShaderLoader>();
	GLuint WaveShader;
	GLuint ModelShader;
	GLuint ObjectShader;


	void Init() {
		WaveShader = SL0->CreateProgram(WAVE_VERT_SHADER, WAVE_FRAG_SHADER);
		ModelShader = SL1->CreateProgram(MODEL_VERT_SHADER, MODEL_FRAG_SHADER);
		ObjectShader = SL2->CreateProgram(VERT_SHADER, FRAG_SHADER);
	}

	void Draw(glm::mat4 _VPMat) {

		bleh.Draw(ObjectShader, Camera::GetMatrix());
		pyramid.Draw(ObjectShader, Camera::GetMatrix());
		planeboi.Draw(WaveShader, Camera::GetMatrix());
		glFrontFace(GL_CCW);
		Player.Render(ModelShader);
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



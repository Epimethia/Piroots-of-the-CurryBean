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

	GLuint WaveShader;
	GLuint ModelShader;
	ShaderLoader sl;
	ShaderLoader sl2;
	Level() {
		WaveShader = sl.CreateProgram(WAVE_VERT_SHADER, WAVE_FRAG_SHADER);
		ModelShader = sl2.CreateProgram(MODEL_VERT_SHADER, MODEL_FRAG_SHADER);
	}

	void Draw(GLuint _Program, glm::mat4 _VPMat) {

		//BG.Process(_Program, _VPMat);
		/*box.Draw(_Program, Camera::GetMatrix());*/
		//PlayerObj.Process(_Program, _VPMat);
		bleh.Draw(_Program, Camera::GetMatrix());
		//pyramid.Draw(_Program, Camera::GetMatrix());
		planeboi.Draw(WaveShader, Camera::GetMatrix());
		glFrontFace(GL_CCW);
		Player.Render();
		glFrontFace(GL_CW);
		/*for (auto it : EntityVect) {
			it.Draw(_Program, _VPMat);
		}*/
		//Title.RenderText();

	}
	~Level() {

		/*for (int i = 0; i < static_cast<int>(EntityVect.size()); ++i) {
			EntityVect.pop_back();
		}
		EntityVect.clear();*/
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
	Level& GetCurrentLevel() { return LevelVect[CurrentLevel]; };

private:
	GameManager();
	static std::shared_ptr<GameManager> SceneManagerPtr;

	void GenerateLevels();
	std::vector<Level> LevelVect;
	ShaderLoader sl;
	ShaderLoader sl2;
	GLuint ObjectShaders;
	int CurrentLevel = 0;
};
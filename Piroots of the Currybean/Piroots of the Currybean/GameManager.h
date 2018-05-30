#pragma once
#include "Resource.h"
#include "ShaderLoader.h"
#include "Entity.h"
#include "Camera.h"
#include "InputManager.h"

struct Level {
	std::vector<Entity> EntityVect;
	/*CPlayer PlayerObj;
	CEntity BG;
	Cube box;*/
	CUBE bleh;
	//Pyramid pyramid;
	//Text Title = Text("Jack Sparrow", ARIAL, glm::vec2(10.0f, 700.0f));
	
	Level() {
		bleh = CUBE(
			PLAYER_SPRITE,
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.1f, 0.1f, 0.1f),
			glm::vec3(0.0f, 0.0f, 0.0f)
		);
	}

	void Draw(GLuint _Program, glm::mat4 _VPMat) {
		//BG.Process(_Program, _VPMat);
		/*box.Draw(_Program, Camera::GetMatrix());*/
		//PlayerObj.Process(_Program, _VPMat);
		bleh.Draw(_Program, Camera::GetMatrix());
		for (auto it : EntityVect) {
			it.Draw(_Program, _VPMat);
		}
		//Title.RenderText();
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
	static GLuint ObjectShaders;
	//CSoundMngr SoundManager;
	int CurrentLevel = 0;
};
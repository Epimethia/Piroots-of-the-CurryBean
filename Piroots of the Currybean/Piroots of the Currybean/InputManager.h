#pragma once
#include "Resource.h"
#include "AutonomousMove.h"
#include "GameManager.h"
#include "Menu.h"


enum KEYSTATE {
	KEY_FIRST_PRESS,
	KEY_RELEASED,
	KEY_HELD,
};

class InputManager {
public:
	InputManager();
	static void ProcessKeyInput(std::shared_ptr<Player> _Obj);
	static void ProcessKeyInput(std::shared_ptr<Menu> _Menu);
private:	
	static unsigned char KeyArray[255];
	static unsigned char KeySpecialArray[255];

	static void NormKeyDown(unsigned char key, int x, int y);
	static void NormKeyUp(unsigned char key, int x, int y);
	static void SpecialKeyDown(int key, int x, int y);
	static void SpecialKeyUp(int key, int x, int y);
};
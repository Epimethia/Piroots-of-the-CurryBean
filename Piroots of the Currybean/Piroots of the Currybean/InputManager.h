#pragma once
#include "Resource.h"
#include "Entity.h"

enum KeyState {
	KEY_FIRST_PRESS,
	KEY_RELEASED,
	KEY_HELD,
};

class InputManager {
public:
	InputManager();
	static void ProcessKeyInput(std::shared_ptr<Entity> _Obj);
private:	
	static unsigned char KeyArray[255];
	static unsigned char KeySpecialArray[255];

	static void NormKeyDown(unsigned char key, int x, int y);
	static void NormKeyUp(unsigned char key, int x, int y);
	static void SpecialKeyDown(int key, int x, int y);
	static void SpecialKeyUp(int key, int x, int y);

};
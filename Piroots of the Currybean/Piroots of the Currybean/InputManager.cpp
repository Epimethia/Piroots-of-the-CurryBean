#include "InputManager.h"

unsigned char InputManager::KeyArray[255];
unsigned char InputManager::KeySpecialArray[255];

InputManager::InputManager() {
	for (int i = 0; i < 255; ++i) {
		KeyArray[i] = KEY_RELEASED;
		KeySpecialArray[i] = KEY_RELEASED;
	}
}

void InputManager::ProcessKeyInput(std::shared_ptr<Player> _Obj) {
	glutSpecialFunc(InputManager::SpecialKeyDown);
	glutSpecialUpFunc(InputManager::SpecialKeyUp);
	glutKeyboardFunc(InputManager::NormKeyDown);
	glutKeyboardUpFunc(InputManager::NormKeyUp);

	glm::vec3 Target;
	//RIGHT KEY INPUT
	if (KeySpecialArray[GLUT_KEY_RIGHT] == KEY_HELD) {
		//Held
	}
	else if (KeySpecialArray[GLUT_KEY_RIGHT] == KEY_FIRST_PRESS) {
		_Obj->CreateBullet(glm::vec3(-1.0f, 0.0f, 0.0f));
		KeySpecialArray[GLUT_KEY_RIGHT] = KEY_HELD;
	}

	//LEFT KEY INPUT
	else if (KeySpecialArray[GLUT_KEY_LEFT] == KEY_HELD) {
		//Held
	}
	else if (KeySpecialArray[GLUT_KEY_LEFT] == KEY_FIRST_PRESS) {
		_Obj->CreateBullet(glm::vec3(1.0f, 0.0f, 0.0f));
		KeySpecialArray[GLUT_KEY_LEFT] = KEY_HELD;
	}

	//UP KEY INPUT
	else if (KeySpecialArray[GLUT_KEY_UP] == KEY_HELD) {
		//Held Stuff
	}
	else if (KeySpecialArray[GLUT_KEY_UP] == KEY_FIRST_PRESS) {
		_Obj->CreateBullet(glm::vec3(0.0f, -1.0f, 0.0f));
		
		KeySpecialArray[GLUT_KEY_UP] = KEY_HELD;
	}

	else if (KeySpecialArray[GLUT_KEY_DOWN] == KEY_HELD) {
		//Held Stuff
	}
	else if (KeySpecialArray[GLUT_KEY_DOWN] == KEY_FIRST_PRESS) {
		KeySpecialArray[GLUT_KEY_DOWN] = KEY_HELD;
		_Obj->bShoot = true;
		_Obj->CreateBullet(glm::vec3(0.0f, 1.0f, 0.0f));
	}

	else if (KeySpecialArray[GLUT_KEY_RIGHT] == KEY_RELEASED) _Obj->bShoot = false;
	else if (KeySpecialArray[GLUT_KEY_LEFT] == KEY_RELEASED) _Obj->bShoot = false;
	else if (KeySpecialArray[GLUT_KEY_UP] == KEY_RELEASED) _Obj->bShoot = false;
	else if (KeySpecialArray[GLUT_KEY_DOWN] == KEY_RELEASED) _Obj->bShoot = false;

	#pragma region WASD INPUT
	//RIGHT KEY INPUT
	if (KeyArray['d'] == KEY_HELD) {
		glm::vec3 Target = { _Obj->GetPos().x - 200.0f,_Obj->GetPos().y, _Obj->GetPos().z };
		_Obj->GetTarget() = Target;
	} else if (KeyArray['d'] == KEY_FIRST_PRESS) KeyArray['d'] = KEY_HELD;

	//LEFT KEY INPUT
	else if (KeyArray['a'] == KEY_HELD) {
		glm::vec3 Target = { _Obj->GetPos().x + 200.0f,_Obj->GetPos().y, _Obj->GetPos().z };
		_Obj->GetTarget() = Target;
	} else if (KeyArray['a'] == KEY_FIRST_PRESS) KeyArray['a'] = KEY_HELD;

	//UP KEY INPUT
	else if (KeyArray['w'] == KEY_HELD) {
		glm::vec3 Target = { _Obj->GetPos().x,_Obj->GetPos().y - 200.0f, _Obj->GetPos().z };
		_Obj->GetTarget() = Target;
	} else if (KeyArray['w'] == KEY_FIRST_PRESS) KeyArray['w'] = KEY_HELD;

	else if (KeyArray['s'] == KEY_HELD) {
		glm::vec3 Target = { _Obj->GetPos().x,_Obj->GetPos().y + 200.0f, _Obj->GetPos().z };
		_Obj->GetTarget() = Target;
	} else if (KeyArray['s'] == KEY_FIRST_PRESS) KeyArray['s'] = KEY_HELD;
	#pragma endregion

	if (KeyArray[27] == KEY_HELD) {
		//Do esc stuff
	}
	else if (KeyArray[27] == KEY_FIRST_PRESS) {
		KeyArray[27] = KEY_HELD;
		GameManager::DestroyInstance();
		glutLeaveMainLoop();
		return;
	}

	//Setting Camera Pos
	Camera::GetPos().x = _Obj->GetPos().x / 375.0f * -1.0f;
	Camera::GetPos().y = ((_Obj->GetPos().y + 375.0f) / 375.0f) * -1.0f;
	Camera::GetPos().z = _Obj->GetPos().z - 0.8f;
}

void InputManager::NormKeyDown(unsigned char key, int x, int y) {
	int num = key;
	KeyArray[key] = KEY_FIRST_PRESS;
	/*switch (key) {
		case 32: {
			KeyArray[32] = KEY_FIRST_PRESS;
			break;
		}
		case 27: {
			KeyArray[27] = KEY_FIRST_PRESS;
			break;
		}
	}*/
}

void InputManager::NormKeyUp(unsigned char key, int x, int y) {
	KeyArray[key] = KEY_RELEASED;
	/*switch (key) {
		case 32: {
			KeyArray[32] = KEY_RELEASED;
			break;
		}d
		case 27: {
			KeyArray[27] = KEY_RELEASED;
			break;
		}
		default:break;
	}*/
}

void InputManager::SpecialKeyDown(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_LEFT: {
			KeySpecialArray[GLUT_KEY_LEFT] = KEY_FIRST_PRESS;
			break;
		}
		case GLUT_KEY_RIGHT: {
			KeySpecialArray[GLUT_KEY_RIGHT] = KEY_FIRST_PRESS;
			break;
		}
		case GLUT_KEY_UP: {
			KeySpecialArray[GLUT_KEY_UP] = KEY_FIRST_PRESS;
			break;
		}
		case GLUT_KEY_DOWN: {
			KeySpecialArray[GLUT_KEY_DOWN] = KEY_FIRST_PRESS;
			break;
		}
		default:break;
	}
}

void InputManager::SpecialKeyUp(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_LEFT: {
			KeySpecialArray[GLUT_KEY_LEFT] = KEY_RELEASED;
			break;
		}
		case GLUT_KEY_RIGHT: {
			KeySpecialArray[GLUT_KEY_RIGHT] = KEY_RELEASED;
			break;
		}
		case GLUT_KEY_UP: {
			KeySpecialArray[GLUT_KEY_UP] = KEY_RELEASED;
			break;
		}
		case GLUT_KEY_DOWN: {
			KeySpecialArray[GLUT_KEY_DOWN] = KEY_RELEASED;
			break;
		}
		default:break;
	}
}

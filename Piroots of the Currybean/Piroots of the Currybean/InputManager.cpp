#include "InputManager.h"
#include "GameManager.h"

unsigned char InputManager::KeyArray[255];
unsigned char InputManager::KeySpecialArray[255];

void memes() {};

InputManager::InputManager() {
	for (int i = 0; i < 255; ++i) {
		KeyArray[i] = KEY_RELEASED;
		KeySpecialArray[i] = KEY_RELEASED;
	}
}

void InputManager::ProcessKeyInput(std::shared_ptr<Entity> _Obj) {
	glutSpecialFunc(InputManager::SpecialKeyDown);
	glutSpecialUpFunc(InputManager::SpecialKeyUp);
	glutKeyboardFunc(InputManager::NormKeyDown);
	glutKeyboardUpFunc(InputManager::NormKeyUp);

	//RIGHT KEY INPUT
	if (KeySpecialArray[GLUT_KEY_RIGHT] == KEY_HELD) {
		_Obj->GetPos().x += 20.0f;//5.0f;
	} else if (KeySpecialArray[GLUT_KEY_RIGHT] == KEY_FIRST_PRESS) KeySpecialArray[GLUT_KEY_RIGHT] = KEY_HELD;

	//LEFT KEY INPUT
	if (KeySpecialArray[GLUT_KEY_LEFT] == KEY_HELD) {
		_Obj->GetPos().x -= 20.0f;//5.0f;
	} else if (KeySpecialArray[GLUT_KEY_LEFT] == KEY_FIRST_PRESS) KeySpecialArray[GLUT_KEY_LEFT] = KEY_HELD;

	//UP KEY INPUT
	if (KeySpecialArray[GLUT_KEY_UP] == KEY_HELD) {
		_Obj->GetPos().y += 20.0f;//5.0f;

	} else if (KeySpecialArray[GLUT_KEY_UP] == KEY_FIRST_PRESS) KeySpecialArray[GLUT_KEY_UP] = KEY_HELD;

	if (KeySpecialArray[GLUT_KEY_DOWN] == KEY_HELD) {
		_Obj->GetPos().y -= 20.0f;//5.0f;

	} else if (KeySpecialArray[GLUT_KEY_DOWN] == KEY_FIRST_PRESS) KeySpecialArray[GLUT_KEY_DOWN] = KEY_HELD; 

	//SPACE BAR INPUT
	if (KeyArray[32] == KEY_FIRST_PRESS) {
		KeyArray[32] = KEY_HELD;
		GameManager::GetInstance()->Level1.EntityVect.push_back(std::make_shared<Entity>(
			PYRAMID_MESH,
			PLAYER_SPRITE,
			glm::vec3(_Obj->GetPos().x, _Obj->GetPos().y, _Obj->GetPos().z),
			glm::vec3(0.1f, 0.1f, 0.1f),
			glm::vec3(90.0f, 0.0f, 0.0f)
			)
		);
	}
	else {
		KeyArray[32] = KEY_HELD;
	}

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
	Camera::GetPos().y = (_Obj->GetPos().y / 375.0f) * -1.0f;
	Camera::GetPos().z = _Obj->GetPos().z + 0.2f;

}

void InputManager::NormKeyDown(unsigned char key, int x, int y) {
	int num = key;
	switch (key) {
		case 32: {
			KeyArray[32] = KEY_FIRST_PRESS;
			break;
		}
		case 27: {
			KeyArray[27] = KEY_FIRST_PRESS;
			break;
		}
	}
}

void InputManager::NormKeyUp(unsigned char key, int x, int y) {
	switch (key) {
		case 32: {
			KeyArray[32] = KEY_RELEASED;
			break;
		}
		case 27: {
			KeyArray[27] = KEY_RELEASED;
			break;
		}
		default:break;
	}
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

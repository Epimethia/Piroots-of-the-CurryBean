#include "InputManager.h"
#include "GameManager.h"

unsigned char InputManager::KeyArray[255];
unsigned char InputManager::KeySpecialArray[255];


InputManager::InputManager() {
	for (int i = 0; i < 255; ++i) {
		KeyArray[i] = KEY_RELEASED;
		KeySpecialArray[i] = KEY_RELEASED;
	}
}

void InputManager::ProcessKeyInput(Entity& _PlayerObj) {
	glutSpecialFunc(InputManager::SpecialKeyDown);
	glutSpecialUpFunc(InputManager::SpecialKeyUp);
	glutKeyboardFunc(InputManager::NormKeyDown);
	glutKeyboardUpFunc(InputManager::NormKeyUp);

	//RIGHT KEY INPUT
	if (KeySpecialArray[GLUT_KEY_RIGHT] == KEY_HELD) {
		_PlayerObj.GetPos().x += 2.0f;
		if (_PlayerObj.GetPos().x > 350.0f) _PlayerObj.GetPos().x = 350.0f;
	} else if (KeySpecialArray[GLUT_KEY_RIGHT] == KEY_FIRST_PRESS) KeySpecialArray[GLUT_KEY_RIGHT] = KEY_HELD;

	//LEFT KEY INPUT
	if (KeySpecialArray[GLUT_KEY_LEFT] == KEY_HELD) {
		_PlayerObj.GetPos().x -= 2.0f;
		if (_PlayerObj.GetPos().x < -350.0f) _PlayerObj.GetPos().x = -350.0f;
	} else if (KeySpecialArray[GLUT_KEY_LEFT] == KEY_FIRST_PRESS) KeySpecialArray[GLUT_KEY_LEFT] = KEY_HELD;

	//UP KEY INPUT
	if (KeySpecialArray[GLUT_KEY_UP] == KEY_HELD) {
		_PlayerObj.GetPos().y += 2.0f;
		if (_PlayerObj.GetPos().y > 350.0f) _PlayerObj.GetPos().y = 350.0f;

	} else if (KeySpecialArray[GLUT_KEY_UP] == KEY_FIRST_PRESS) KeySpecialArray[GLUT_KEY_UP] = KEY_HELD;

	if (KeySpecialArray[GLUT_KEY_DOWN] == KEY_HELD) {
		_PlayerObj.GetPos().y -= 2.0f;
		if (_PlayerObj.GetPos().y < -350.0f) _PlayerObj.GetPos().y = -350.0f;

	} else if (KeySpecialArray[GLUT_KEY_DOWN] == KEY_FIRST_PRESS) KeySpecialArray[GLUT_KEY_DOWN] = KEY_HELD; 

	//SPACE BAR INPUT
	if (KeyArray[32] == KEY_FIRST_PRESS) {
		//if (GameManager::GetInstance()->GetCurrentLevel().PlayerObj.CanShoot == true) {
		//	float x = _PlayerObj.ObjPos.x;
		//	float y = _PlayerObj.ObjPos.y;
		//	CBullet Bullet(x, y, 0.25f, 0.0f);
		//	GameManager::GetInstance()->GetCurrentLevel().BulletVect.push_back(Bullet);
		//	GameManager::GetInstance()->GetCurrentLevel().PlayerObj.CanShoot = false;
		std::cout << "Bullet Shot\n";
	}
	else {
		std::cout << "Bullet on cooldown\n";
		KeyArray[32] = KEY_HELD;
	}

	if (KeyArray[27] == KEY_HELD) {
		//Do esc stuff
	}
	else if (KeyArray[27] == KEY_FIRST_PRESS) {
		std::cout << "Esc Held\n";
		KeyArray[27] = KEY_HELD;
	}

	//Setting Camera Pos
	Camera::GetPos().x = (_PlayerObj.GetPos().x / 375.0f) * -1.0f;
	Camera::GetPos().y = -0.3f + (_PlayerObj.GetPos().y / 375.0f) * -1.0f;

}

void InputManager::NormKeyDown(unsigned char key, int x, int y) {
	int num = key;
	std::cout << num << std::endl;
	switch (key) {
		case 32: {
			KeyArray[32] = KEY_FIRST_PRESS;
			std::cout << "Space Down\n";
			break;
		}
		case 27: {
			KeyArray[27] = KEY_FIRST_PRESS;
			std::cout << "Esc Down\n";
			break;
		}
	}
}

void InputManager::NormKeyUp(unsigned char key, int x, int y) {
	switch (key) {
		case 32: {
			KeyArray[32] = KEY_RELEASED;
			std::cout << "Space Released\n";
			break;
		}
		case 27: {
			KeyArray[27] = KEY_RELEASED;
			std::cout << "Esc Released\n";
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

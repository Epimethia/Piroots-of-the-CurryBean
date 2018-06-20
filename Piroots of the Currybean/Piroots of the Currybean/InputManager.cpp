#include "InputManager.h"

unsigned char InputManager::KeyArray[255];
unsigned char InputManager::KeySpecialArray[255];

InputManager::InputManager() {
	for (int i = 0; i < 255; ++i) {
		KeyArray[i] = KEY_RELEASED;
		KeySpecialArray[i] = KEY_RELEASED;
	}
}

void InputManager::ProcessKeyInput(std::shared_ptr<Player> _Player) {
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
		_Player->CreateBullet(glm::vec3(-1.0f, 0.0f, 0.0f));
		KeySpecialArray[GLUT_KEY_RIGHT] = KEY_HELD;
	}

	//LEFT KEY INPUT
	else if (KeySpecialArray[GLUT_KEY_LEFT] == KEY_HELD) {
		//Held
	}
	else if (KeySpecialArray[GLUT_KEY_LEFT] == KEY_FIRST_PRESS) {
		_Player->CreateBullet(glm::vec3(1.0f, 0.0f, 0.0f));
		KeySpecialArray[GLUT_KEY_LEFT] = KEY_HELD;
	}

	//UP KEY INPUT
	else if (KeySpecialArray[GLUT_KEY_UP] == KEY_HELD) {
		//Held Stuff
	}
	else if (KeySpecialArray[GLUT_KEY_UP] == KEY_FIRST_PRESS) {
		_Player->CreateBullet(glm::vec3(0.0f, -1.0f, 0.0f));

		KeySpecialArray[GLUT_KEY_UP] = KEY_HELD;
	}

	//DOWN KEY INPUT
	else if (KeySpecialArray[GLUT_KEY_DOWN] == KEY_HELD) {
		//Held Stuff
	}
	else if (KeySpecialArray[GLUT_KEY_DOWN] == KEY_FIRST_PRESS) {
		KeySpecialArray[GLUT_KEY_DOWN] = KEY_HELD;
		_Player->bShoot = true;
		_Player->CreateBullet(glm::vec3(0.0f, 1.0f, 0.0f));
	}

	else if (KeySpecialArray[GLUT_KEY_RIGHT] == KEY_RELEASED) _Player->bShoot = false;
	else if (KeySpecialArray[GLUT_KEY_LEFT] == KEY_RELEASED) _Player->bShoot = false;
	else if (KeySpecialArray[GLUT_KEY_UP] == KEY_RELEASED) _Player->bShoot = false;
	else if (KeySpecialArray[GLUT_KEY_DOWN] == KEY_RELEASED) _Player->bShoot = false;

#pragma region WASD INPUT
//RIGHT KEY INPUT
	if (KeyArray['d'] == KEY_HELD) {
		glm::vec3 Target = { _Player->GetPos().x - 200.0f,_Player->GetPos().y, _Player->GetPos().z };
		_Player->GetTarget() = Target;
	}
	else if (KeyArray['d'] == KEY_FIRST_PRESS) KeyArray['d'] = KEY_HELD;

 //LEFT KEY INPUT
	else if (KeyArray['a'] == KEY_HELD) {
		glm::vec3 Target = { _Player->GetPos().x + 200.0f,_Player->GetPos().y, _Player->GetPos().z };
		_Player->GetTarget() = Target;
	}
	else if (KeyArray['a'] == KEY_FIRST_PRESS) KeyArray['a'] = KEY_HELD;

 //UP KEY INPUT
	else if (KeyArray['w'] == KEY_HELD) {
		glm::vec3 Target = { _Player->GetPos().x,_Player->GetPos().y - 200.0f, _Player->GetPos().z };
		_Player->GetTarget() = Target;
	}
	else if (KeyArray['w'] == KEY_FIRST_PRESS) KeyArray['w'] = KEY_HELD;

	else if (KeyArray['s'] == KEY_HELD) {
		glm::vec3 Target = { _Player->GetPos().x,_Player->GetPos().y + 200.0f, _Player->GetPos().z };
		_Player->GetTarget() = Target;
	}
	else if (KeyArray['s'] == KEY_FIRST_PRESS) KeyArray['s'] = KEY_HELD;
#pragma endregion

	if (KeyArray[27] == KEY_HELD) {
		//Do esc stuff
	}
	else if (KeyArray[27] == KEY_FIRST_PRESS) {
		KeyArray[27] = KEY_HELD;
		//glutLeaveMainLoop();
		return;
	}

	//Setting Camera Pos
	Camera::GetPos().x = _Player->GetPos().x / 375.0f * -1.0f;
	Camera::GetPos().y = ((_Player->GetPos().y + 375.0f) / 375.0f) * -1.0f;
	Camera::GetPos().z = _Player->GetPos().z - 0.8f;
}

//Menu input
void InputManager::ProcessKeyInput() {
	glutSpecialFunc(InputManager::SpecialKeyDown);
	glutSpecialUpFunc(InputManager::SpecialKeyUp);
	glutKeyboardFunc(InputManager::NormKeyDown);
	glutKeyboardUpFunc(InputManager::NormKeyUp);

	//UP KEY INPUT
	if (KeySpecialArray[GLUT_KEY_UP] == KEY_HELD) {
		//Held Stuff
	}
	else if (KeySpecialArray[GLUT_KEY_UP] == KEY_FIRST_PRESS) {
		KeySpecialArray[GLUT_KEY_UP] = KEY_HELD;
	}

	//DOWN KEY INPUT
	else if (KeySpecialArray[GLUT_KEY_DOWN] == KEY_HELD) {
		//Held Stuff
	}
	else if (KeySpecialArray[GLUT_KEY_DOWN] == KEY_FIRST_PRESS) {
		KeySpecialArray[GLUT_KEY_DOWN] = KEY_HELD;
	}

	//ENTER KEY INPUT
	if (KeyArray['\r'] == KEY_HELD) {

	}
	else if (KeyArray['\r'] == KEY_FIRST_PRESS) {
		KeyArray['\r'] = KEY_HELD;
	}

}

void InputManager::NormKeyDown(unsigned char key, int x, int y) {
	KeyArray[key] = KEY_FIRST_PRESS;
}

void InputManager::NormKeyUp(unsigned char key, int x, int y) {
	KeyArray[key] = KEY_RELEASED;
}

void InputManager::SpecialKeyDown(int key, int x, int y) {
	KeySpecialArray[key] = KEY_FIRST_PRESS;
}

void InputManager::SpecialKeyUp(int key, int x, int y) {
	KeySpecialArray[key] = KEY_RELEASED;
}

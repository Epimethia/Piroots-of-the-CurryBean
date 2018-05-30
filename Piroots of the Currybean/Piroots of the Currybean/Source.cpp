
#include "Resource.h"
#include "GameManager.h"
#include "Camera.h"
#include <cstring>

void init() {
	GameManager::GetInstance();
	GameManager::GetInstance()->Init();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

}

void render(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GameManager::GetInstance()->DrawScene(0);
	glutSwapBuffers();
}

void update(void) {
	GameManager::GetInstance()->GameLoop();
	glutPostRedisplay();
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(
		static_cast<int>(UTILS::WindowWidth),
		static_cast<int>(UTILS::WindowHeight)
	);
	glutCreateWindow("OpenGL First Window");
	glClearColor(0.0, 0.5, 0.5, 1.0);
	glewInit();
	init();
	glutDisplayFunc(render);
	glutIdleFunc(update);
	glutIgnoreKeyRepeat(1);
	glutMainLoop();
	glutExit();

	GameManager::DestroyInstance();
	return 0;
}
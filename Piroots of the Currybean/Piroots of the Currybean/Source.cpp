
#include "Resource.h"
#include "GameManager.h"
#include "Camera.h"
#include <cstring>

float g_DeltaTime = 0.0f;

void init() {
	glutSetCursor(GLUT_CURSOR_NONE);
	GameManager::GetInstance();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
}

void render(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GameManager::GetInstance()->DrawScene();
	glutSwapBuffers();
}

void update(void) {
	g_DeltaTime = Clock::GetDeltaTime();
	GameManager::GetInstance()->GameLoop(g_DeltaTime);
	glutPostRedisplay();
}

void Exit(void) {
	GameManager::DestroyInstance();
} 

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(static_cast<int>(UTILS::WindowWidth), static_cast<int>(UTILS::WindowHeight));
	glutCreateWindow("Piroots of the CurryBeans");
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glewInit();
	init();
	glutDisplayFunc(render);
	glutIdleFunc(update);
	glutIgnoreKeyRepeat(1);
	glutCloseFunc(Exit);
	glutMainLoop();
	return 0;
}
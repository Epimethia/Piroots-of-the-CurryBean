#include "Resource.h"

//Local Includes
//#include "Entity.h"
void init() {


}

void render(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glutSwapBuffers();
}

void update(void) {
	glutPostRedisplay();
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(25, 25);
	glutInitWindowSize(
		static_cast<int>(UTILS::WindowWidth), 
		static_cast<int>(UTILS::WindowHeight)
	);
	glutCreateWindow("Piroots of the Currybean");
	glClearColor(0.0, 0.5, 0.5, 1.0);

	glewInit();
	init();
	glutDisplayFunc(render);
	glutIdleFunc(update);
	glutIgnoreKeyRepeat(1);
	glutMainLoop();
	glutExit();

	return 0;
}
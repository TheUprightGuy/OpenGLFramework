#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "utils.h"

#include "SceneManager.h"
#include "MeshManager.h"

GLuint program;

void init();
void render();
void update();
void KeyDown(unsigned char key, int x, int y);
void KeyUp(unsigned char key, int x, int y);
void SpecialKeyUp(int key, int x, int y);
void SpecialKeyDown(int key, int x, int y);

void Mouse(int button, int glutState, int x, int y);
void PassiveMouse(int x, int y);

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	
	glutInitWindowPosition((GLfloat)glutGet(GLUT_WINDOW_WIDTH) / 2, (GLfloat)glutGet(GLUT_WINDOW_HEIGHT) / 2);
	glutInitWindowSize(1024, 576);
	glutCreateWindow("The Red Room");

	glutSetOption(GLUT_MULTISAMPLE, 12);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glewInit();
	init();

	glutDisplayFunc(render);

	glutIdleFunc(update);

	glutKeyboardFunc(KeyDown);
	glutKeyboardUpFunc(KeyUp);

	glutSpecialFunc(SpecialKeyDown);
	glutSpecialUpFunc(SpecialKeyUp);

	glutMouseFunc(Mouse); 
	glutPassiveMotionFunc(PassiveMouse);

	glutMainLoop();

	return(0);
}
void init()
{
	CSceneManager::GetInstance().Init(); //Init the scene manager
	CMeshManager::GetInstance(); //Init the mesh manager
}

void render()
{
	CSceneManager::GetInstance().Render();
}

void update()
{
	CSceneManager::GetInstance().Process();
	glutPostRedisplay();
}

void KeyDown(unsigned char key, int x, int y)
{
	CInput::GetInstance().KeyboardDown(key, x, y);
}

void KeyUp(unsigned char key, int x, int y)
{
	CInput::GetInstance().KeyboardUp(key, x, y);
}

void SpecialKeyUp(int key, int x, int y)
{
	CInput::GetInstance().SpecialKeyUp(key, x, y);
}

void SpecialKeyDown(int key, int x, int y)
{
	CInput::GetInstance().SpecialKeyDown(key, x, y);
}

void Mouse(int button, int glutState, int x, int y)
{
	CInput::GetInstance().MouseClick(button, glutState, x, y);
}

void PassiveMouse(int x, int y)
{
	CInput::GetInstance().MouseMove(x, y);
	CInput::GetInstance().UpdateMousePicking();
}


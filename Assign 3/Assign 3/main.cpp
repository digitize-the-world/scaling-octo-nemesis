
#include <stdio.h>
#include <stdlib.h>

#include "GL/glew.h"
#include "GL/freeglut.h"

#include "ProgLogic.h"

using namespace std;

#define STARTING_WIDTH 600
#define STARTING_HEIGHT 600

int windowWidth, windowHeight;

// perspective projection parameters
GLfloat fieldofview = 45.0;
GLfloat aspect      = 1.0;
GLfloat nearPlane   = 0.1;
GLfloat farPlane    = 100.0;

// lighting parameters
GLfloat AmbientLight[]		= { 0.3f, 0.3f, 0.3f, 1.0f };
GLfloat DiffuseLight[]		= { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat SpecularLight[]		= { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPos[]			= {0.0f, 5.0f , 5.0f,1.0f};

bool specialKeyBuffer[256];
bool standardKeyBuffer[256];

ProgLogic *progLogic;

void keyboard(unsigned char key, int x, int y )
{
	standardKeyBuffer[key] = true;

  switch( key ) {
    case 'q' : 
	case 'Q' :
      exit(0);	  		// exit the program
      break;
	case 'c' :
		progLogic->lightToggle = progLogic->lightToggle ^ 0x1;
		break;
	case 'r' :
	case 'R' :
		progLogic->InitDog();
  }
}

void keyboardUp(unsigned char key, int x, int y)
{
	standardKeyBuffer[key] = false;
}

void specialUp(int key, int x, int y)
{
	specialKeyBuffer[key] = false;
}

void special(int key, int x, int y)
{
	specialKeyBuffer[key] = true;
}

void menu(int value)
{
	switch (value)
	{
	case 1 :
		break;
	case 2 :
		// close the program
		exit(0);
		break;
	case 3 :
		glShadeModel(GL_SMOOTH);
		break;
	case 4 :
		glShadeModel(GL_FLAT);
		break;
	default:
		break;
	}
}

/* 
 * The rountine display() gets called each time the window is redrawn.
 */
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (!progLogic->lightToggle)
		glLightfv(GL_LIGHT0, GL_POSITION, LightPos);

	progLogic->UpdateLogic();

	if (progLogic->lightToggle)
		glLightfv(GL_LIGHT0, GL_POSITION, LightPos);

	progLogic->DrawObjects();

	glutSwapBuffers();
}

void windowReshape(int width, int height)
{
	windowWidth = width; 
	windowHeight = height;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, windowWidth, windowHeight);

	aspect = (float)windowWidth/(float)windowHeight;

	gluPerspective(fieldofview, aspect, nearPlane, farPlane);
	glMatrixMode(GL_MODELVIEW);

	glutPostRedisplay();
}

/* Initialize states -- called before */

void init()
{
	/* set background clear color to black */
	glClearColor(0.0, 0.0, 0.2, 1.0);
	/* set current color to white */
	glColor3f(1.0, 1.0, 1.0);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, SpecularLight);

	// smooth shading
	glShadeModel(GL_SMOOTH);

	/* identify the projection matrix that we would like to alter */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	aspect = (double)windowWidth/(double)windowHeight;
	farPlane = nearPlane+100.0f;
	gluPerspective(fieldofview, aspect, nearPlane, farPlane);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	progLogic = new ProgLogic();
	progLogic->InitLogic(standardKeyBuffer, specialKeyBuffer);

	int menuID = glutCreateMenu(menu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	int shadingMenuID = glutCreateMenu(menu);
	
	glutSetMenu(menuID);
	glutAddSubMenu("Shading", shadingMenuID);
	glutAddMenuEntry("Quit", 2);

	glutSetMenu(shadingMenuID);
	glutAddMenuEntry("Smooth", 3);
	glutAddMenuEntry("Flat", 4);


}

void idle()
{
	glutPostRedisplay();
}


/* The main program */

int main(int argc, char** argv)
{

	glewInit();
	/* create a window, position it, and name it */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(STARTING_WIDTH, STARTING_HEIGHT);
	glutInitWindowPosition(200, 200);
	glutCreateWindow("Assignment #3");

	glutReshapeFunc(windowReshape);

	/* create a callback routine for (re-)display */
	glutDisplayFunc(display);

	// idle function for being idle
	glutIdleFunc(idle);

	// callback for keypress
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);
	glutSpecialFunc(special);
	glutSpecialUpFunc(specialUp);

	/* init some states */
	init();

	/* entering the event loop */
	glutMainLoop();

	/* code here will not be executed */
}


//---------------------------------------------------------------------------
//
// This software is provided 'as-is' for assignment of COMP308
// in ECS, Victoria University of Wellington,
// without any express or implied warranty.
// In no event will the authors be held liable for any
// damages arising from the use of this software.
//
// The contents of this file may not be copied or duplicated in any form
// without the prior permission of its owner.
//
// Copyright (c) 2012 by Taehyun Rhee
//
// Edited by Roma Klapaukh, Daniel Atkins, and Taehyun Rhee
//
//---------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <string.h>
#include "define.h"
#include "G308_Skeleton.h"

GLuint g_mainWnd;
GLuint g_nWinWidth = G308_WIN_WIDTH;
GLuint g_nWinHeight = G308_WIN_HEIGHT;

void G308_keyboardListener(unsigned char, int, int);
void G308_Reshape(int w, int h);
void G308_display();
void G308_init();
void G308_SetCamera();
void G308_SetLight();

Skeleton* skeleton;

/*  Various global state */
/*  Toggles */
int toggleAxes = 0;   /* toggle axes on and off */
int toggleFloor = 0; /* toggle floor on and off */

/*  Display view */
int th = 0;  /* azimuth of view angle */
int command=A_STOP; //Command animation

// Pop up menu identifiers
int mainMenu;

// menu status
int menuFlag = 0;

void processMenuStatus(int status, int x, int y) {

	if (status == GLUT_MENU_IN_USE)
		menuFlag = 1;
	else
		menuFlag = 0;
}

void processMainMenu(int option)
{
	if ((option>=A_REWIND)&&(option<=A_FFORWARD))
	{
		command = option;
	}
	else
	{
		switch (option)
		{
		case A_AXIS: toggleAxes = 1-toggleAxes; break;
		case A_FLOOR: toggleFloor = 1-toggleFloor; break;
		case A_ROTL: th = -5; break;
		case A_ROTR: th = 5; break;
		case A_NPOSE: skeleton->nextFrame(); break;
		case A_SEPARATOR: break;
		default: break;
		}
	}
	glutPostRedisplay();
}


void createPopupMenus(int args) {


	mainMenu = glutCreateMenu(processMainMenu);

	//if amc is not loaded, animation controls don't appear
	if (args>2)
	{
		if (skeleton->getNumFrames()>3)
		{
		glutAddMenuEntry("Rewind (z)", A_REWIND);
		glutAddMenuEntry("Play (x)", A_PLAY);
		glutAddMenuEntry("Pause (v)",A_PAUSE);
		glutAddMenuEntry("Stop (c)",A_STOP);
		glutAddMenuEntry("Fast Forward (b)", A_FFORWARD);
		}
		else {
			glutAddMenuEntry("Next Pose (p)", A_NPOSE);
		}
		glutAddMenuEntry("------------", A_SEPARATOR);
	}
	glutAddMenuEntry("Toggle Axis (a)", A_AXIS);
	glutAddMenuEntry("Toggle Floor (f)", A_FLOOR);
	glutAddMenuEntry("------------", A_SEPARATOR);
	glutAddMenuEntry("Rotate Left (q)", A_ROTL);
	glutAddMenuEntry("Rotate Right (e)", A_ROTR);
	// attach the menu to the right button
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// this will allow us to know if the menu is active
	glutMenuStatusFunc(processMenuStatus);
}


/*
 * drawAxes()
 * ------
 * Draw the axes
 */
void drawAxes()
{
  if (toggleAxes) {
    /*  Length of axes */
    double len = 10000.0;
    glColor3f(1.0,0.0,0.0);
    glBegin(GL_LINES);
    glVertex3d(-len,0,0);
    glVertex3d(len,0,0);
    glEnd();
    glColor3f(0.0,1.0,0.0);
    glBegin(GL_LINES);
    glVertex3d(0,-len,0);
    glVertex3d(0,len,0);
    glEnd();
    glColor3f(0.0,0.0,1.0);
    glBegin(GL_LINES);
    glVertex3d(0,0,-len);
    glVertex3d(0,0,len);
    glEnd();
  }
}

/*
 *  drawValues()
 *  ------
 *  Draw the values in the lower left corner
 */
void drawFloor()
{
	if (toggleFloor)
	{
		glPushMatrix();
		glColor3d(0,0.2,0.2);
		glTranslated(0, -0.05, 0);
		glScalef(10, 0.1, 10);
		glutSolidCube(1);
		glPopMatrix();
	}
}

int main(int argc, char** argv) {
	if (argc < 2 || argc > 3) {
		//Usage instructions for core and challenge
		printf("Usage\n");
		printf("./Ass2 priman.asf [priman.amc]\n");
		exit(EXIT_FAILURE);
	}
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(g_nWinWidth, g_nWinHeight);
	g_mainWnd = glutCreateWindow("COMP308 Assignment2");

	glutKeyboardFunc(G308_keyboardListener);


	glutDisplayFunc(G308_display);
	glutReshapeFunc(G308_Reshape);

	G308_init();
	// [Assignment2] : Read ASF file

	if (argc < 3)
		skeleton = new Skeleton(argv[1], NULL);
	else
		skeleton = new Skeleton(argv[1], argv[2]);

	createPopupMenus(argc);

	glutMainLoop();

	return EXIT_SUCCESS;
}


// Init Light and Camera
void G308_init() {

	G308_SetLight();
	G308_SetCamera();
}

// Display call back
void G308_display() {

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glShadeModel(GL_SMOOTH);

	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		printf("%s\n", gluErrorString(err));
	}

	  /*  Set View Angle */
	  glRotated(th,0,1,0);
	  th = 0;

	  /*  Draw  */
	  drawAxes();
	  drawFloor();

	// [Assignmet2] : render skeleton
	if (skeleton != NULL) {
		skeleton->display(command);
	}


	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);

	glutPostRedisplay();
	glutSwapBuffers();
}


void G308_keyboardListener(unsigned char key, int x, int y) {
	  /*  Exit on ESC */
	  //printf("key:  %u \n", key);
	  switch(key) {
	  case 'a': toggleAxes = 1-toggleAxes; break;
	  case 'q': th = -5; break;
	  case 'e': th = 5; break;
	  case 'r': th = 5; break;
	  case 'z': command = A_REWIND; break;
	  case 'x': command = A_PLAY; break;
	  case 'c': command = A_STOP; break;
	  case 'v': command = A_PAUSE; break;
	  case 'b': command = A_FFORWARD; break;
	  case 'f': toggleFloor = 1-toggleFloor; break;
	  case 'p': skeleton->nextFrame(); break;
	  default: break;
	  }
	  glutPostRedisplay();
}

// Reshape function
void G308_Reshape(int w, int h) {
	if (h == 0)
		h = 1;

	g_nWinWidth = w;
	g_nWinHeight = h;

	glViewport(0, 0, g_nWinWidth, g_nWinHeight);
}

// Set Camera Position
void G308_SetCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(G308_FOVY, (double) g_nWinWidth / (double) g_nWinHeight, G308_ZNEAR_3D, G308_ZFAR_3D);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(3, 1.5, 7.0,  0.,   0.6,   0.,  0.0,  1.0,   0.0);
}

// Set View Position
void G308_SetLight() {
	float direction[] = { 1.0f, 1.0f, 1.0f, 0.0f };
	float diffintensity[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	float ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, direction);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffintensity);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	glEnable(GL_LIGHT0);
}


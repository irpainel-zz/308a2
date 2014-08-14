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
int toggleValues = 1; /* toggle values on and off */

/*  Display view */
int th = 0;  /* azimuth of view angle */
int ph = 0;  /* elevation of view angle */
int frame = 0;


void printAt(float x, float y, float z, std::string message)
{
  glRasterPos3f(x, y, z);
  int len, i;
  len = message.size();
  for (i = 0; i < len; i++) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, message[i]);
  }
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
void drawValues()
{
  std::string message;
  message = "Angle (th, ph) = , )";
  if (toggleValues) {
    glColor3f(1.0,1.0,1.0);
    printAt(5, 5, 0, message);
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
	cout << argc << "  args \n";
	// [Assignment2] : Read ASF file
	if (argc < 3)
	{
		skeleton = new Skeleton(argv[1], NULL);
	}
	else
		skeleton = new Skeleton(argv[1], argv[2]);

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
	  //glRotated(ph,1,0,0);
	  glRotated(th,0,1,0);
	  th = 0;

	  /*  Draw  */
	  drawAxes();
	  drawValues();

	// [Assignmet2] : render skeleton
	if (skeleton != NULL) {
		skeleton->display();
	}


	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);

	glutSwapBuffers();
}

void G308_keyboardListener(unsigned char key, int x, int y) {
	  /*  Exit on ESC */
	  //printf("key:  %u \n", key);
	  switch(key) {
	  case 'a': toggleAxes = 1-toggleAxes; break;
	  case 'q': th = 5; break;
	  case 'e': th = -5; break;
	  default: skeleton->controlSkeleton(key); break;
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


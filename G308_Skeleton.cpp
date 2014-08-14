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

#include <string.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include "G308_Skeleton.h"
#include "define.h"
#include "Frame.h"

Skeleton::Skeleton(char* filename, char * amcfilename) {

	numBones = 1;
	buffSize = 200;
	maxBones = 60;
	angle = 0;
	actualFrame = 0;
	numFrames = 0;
	root = (bone*) malloc(sizeof(bone) * maxBones);

	for (int i = 0; i < 60; i++) {
		root[i].id = 0;
		root[i].numChildren = 0;
		root[i].dirx = 0;
		root[i].diry = 0;
		root[i].dirz = 0;
		root[i].rotx = 0;
		root[i].roty = 0;
		root[i].rotz = 0;
		root[i].dof = DOF_NONE;
		root[i].length = 0;
		root[i].name = NULL;
		root[i].children = (bone**) malloc(sizeof(bone*) * 5);

		//Challenge stuff
		root[i].currentTranslatex = 0;
		root[i].currentTranslatey = 0;
		root[i].currentTranslatez = 0;
		root[i].currentRotationx = 0;
		root[i].currentRotationy = 0;
		root[i].currentRotationz = 0;

	}
	char*name = (char*) malloc(sizeof(char) * 5);
	name[0] = 'r';
	name[1] = name[2] = 'o';
	name[3] = 't';
	name[4] = '\0';
	root[0].name = name;
	root[0].dof = DOF_ROOT;
	root[0].id = 0;
	readASF(filename);
	readAMC(amcfilename);
}

Skeleton::~Skeleton() {
	deleteBones(root);
}

void Skeleton::deleteBones(bone* root) {
	for (int i = 0; i < maxBones; i++) {
		if (root[i].name != NULL) {
			free(root[i].name);
		}
		if (root[i].children != NULL) {
			free(root[i].children);
		}
	}
	free(root);
}

void Skeleton::controlSkeleton(unsigned char key)
{
	switch (key)
	{
	 case 'p':
		 if (numFrames>0)
		 {
			 actualFrame++;
			 actualFrame = actualFrame % numFrames;
			 cout << actualFrame << " frame\n";
		 }
		 break;
	}

}

void Skeleton::drawBone(bone r, GLUquadric* q)
{
	double angle;
	float axisDiam=0.1;
	int axisLen = 2;
	G308_Point boneDirTemp, rotationAxis, t, initialPosition;
	double matrixRotation[4][4], matrixRotationT[4][4], matrixPose[4][4];
	double rotationVector[16];

	glColor3f(0, 1, 1);
	gluSphere(q, 0.5, 10, 10);


	rotationInXYZ(r.rotx, r.roty, r.rotz, matrixRotation);
	castMatrix44to16(matrixRotation, rotationVector);
	//cout << r.name;
	//printMatrix(matrixRotation);
	glMultMatrixd(rotationVector);

	float * poseRot = frames[actualFrame].bonesPose[r.id].rot;
		rotationInXYZ(poseRot[0], poseRot[1], poseRot[2], matrixPose);
		castMatrix44to16(matrixPose, rotationVector);
		glMultMatrixd(rotationVector);

	//axis
	//X-Axis
	glPushMatrix();
	glRotated(90, 0, 1, 0);
	glColor3f(1, 0, 0); //red
	gluCylinder(q, axisDiam, axisDiam, axisLen, 10, 10);
	glTranslatef(0, 0, axisLen);
	glutSolidCone(axisDiam+0.08, 0.5 ,10 ,10);
	glPopMatrix();
	//Y-Axis
	glPushMatrix();
	glRotated(-90, 1, 0, 0);
	glColor3f(0, 1, 0); //green
	gluCylinder(q, axisDiam, axisDiam, axisLen, 10, 10);
	glTranslatef(0, 0, axisLen);
	glutSolidCone(axisDiam+0.08, 0.5 ,10 ,10);
	glPopMatrix();
	//Z-Axis
	glPushMatrix();
	glColor3f(0, 0, 1);//blue
	gluCylinder(q, axisDiam, axisDiam, axisLen, 10, 10);
	glTranslatef(0, 0, axisLen);
	glutSolidCone(axisDiam+0.08, 0.5 ,10 ,10);
	glPopMatrix();

	matrixTranspose(matrixRotation, matrixRotationT);
	castMatrix44to16(matrixRotationT, rotationVector);
	glMultMatrixd(rotationVector);

	glPushMatrix();

	boneDirTemp.x = r.dirx;
	boneDirTemp.y = r.diry;
	boneDirTemp.z = r.dirz;

	initialPosition.x = 0;
	initialPosition.y = 0;
	initialPosition.z = 0;

	boneDirTemp = diffVec(boneDirTemp, initialPosition);

	rotationAxis.x = 0;
	rotationAxis.y = 0;
	rotationAxis.z = 1;

	t = crossP(rotationAxis, boneDirTemp);
	angle = calcAngle(rotationAxis, boneDirTemp);
	//rotate bone in t axis
	glRotated(angle,t.x,t.y,t.z);
		//bone
		gluQuadricOrientation(q, GLU_OUTSIDE);
		glColor3f(0.5, 0.5, 0.5);
		gluCylinder(q, 0.3, 0.4, r.length, 10, 10);
	glPopMatrix();
}

void Skeleton::readChildBones(bone *r, GLUquadric* q)
{
	int i;

	glPushMatrix();
	drawBone(*r, q);
	if (r->numChildren > 0)
	{
		//cout << r->dof << " dof \n";

		glTranslatef(r->dirx*r->length, r->diry*r->length, r->dirz*r->length);
			for (i=0; i < r->numChildren; i++) {
				readChildBones(r->children[i], q);
			}
	}
	glPopMatrix();
}

// [Assignment2] you may need to revise this function
void Skeleton::display() {
	if (root == NULL) {
		return;
	}
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glScalef(0.05, 0.05, 0.05);

	GLUquadric* quad = gluNewQuadric(); //Create a new quadric to allow you to draw cylinders
	if (quad == 0) {
		printf("Not enough memory to allocate space to draw\n");
		exit(EXIT_FAILURE);
	}
	//Actually draw the skeleton
	display(root, quad);

	gluDeleteQuadric(quad);
	glPopMatrix();
}


// [Assignment2] you need to fill this function
void Skeleton::display(bone* root, GLUquadric* q) {
	if (root == NULL) {
		return;
	}
	float * rootTrans = frames[actualFrame].bonesPose[0].trans;
	glTranslated(rootTrans[0], rootTrans[1], rootTrans[2]);

	readChildBones(root, q);
}


void Skeleton::readAMC(char* filename) {
	tempFrame = NULL;
	if (filename == NULL)
	{
		tempFrame = new Frame(numBones, numFrames);
		frames.push_back(*tempFrame);
	}
	else {
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		printf("Failed to open file %s\n", filename);
		exit(EXIT_FAILURE);
	}

	char* buff = new char[buffSize];
	char *p;

	printf("Starting reading AMC\n");
	while ((p = fgets(buff, buffSize, file)) != NULL) {
		//Reading actually happened!

		char* start = buff;
		trim(&start);

		//Check if it is a comment or just empty
		if (buff[0] == '#' || buff[0] == '\0' || buff[0] == ':') {
			continue;
		}
		if (start != NULL) {
			if (isdigit(buff[0])){

				if (tempFrame != NULL) {
					//push last frame to array
					frames.push_back(*tempFrame);
				}

				//cout << "Creating a new frame \n";
				tempFrame = new Frame(numBones, numFrames);
				numFrames ++;
			}
			else {
				readTransfBoneAMC(start);
			}
		}

	}

	//reaches the document EOF, pushes last frame read
	frames.push_back(*tempFrame);

	delete[] buff;
	fclose(file);
	printf("Completed reading AMC file\n");
	}
}

void Skeleton::readTransfBoneAMC(char * buff){
	Posebone * tempTransfBone;
	char * temp = buff;

	int boneId, i;
	char t[40];

	//read bone name
	sscanf(temp, "%s", t);
	temp += strlen(t);
	trim(&temp);


	//look for bone id
	boneId = getBoneId(t);

	if (boneId != -1)
	{
		tempTransfBone = new Posebone();
		tempTransfBone->boneid = boneId;

		//read angles
		if ((root[boneId].dof & DOF_ROOT) == DOF_ROOT)
		{
			for (i = 0; sscanf(temp, "%s", t) != 0 && i < 3; i++) {
				tempTransfBone->trans[i] = atof(t);
				temp += strlen(t);
				trim(&temp);

			}

			for (i = 0; sscanf(temp, "%s", t) != 0 && i < 3; i++) {
				tempTransfBone->rot[i] = atof(t);
				temp += strlen(t);
				trim(&temp);
			}
		}
		else
		{
			if ((root[boneId].dof & DOF_RX) == DOF_RX && sscanf(temp, "%s", t) != 0)
			{
				tempTransfBone->rot[0] = atof(t);
				temp += strlen(t);
				trim(&temp);
			}
			if ((root[boneId].dof & DOF_RY) == DOF_RY && sscanf(temp, "%s", t) != 0)
			{
				tempTransfBone->rot[1] = atof(t);
				temp += strlen(t);
				trim(&temp);
						}
			if ((root[boneId].dof & DOF_RZ) == DOF_RZ && sscanf(temp, "%s", t) != 0)
			{
				tempTransfBone->rot[2] = atof(t);
				temp += strlen(t);
				trim(&temp);
			}
		}
	}
	tempFrame->setBoneRotation(boneId, tempTransfBone);
	//cout << tempFrame->bonesPose[2].boneid << "  frame number \n";
}


int Skeleton::getBoneId(char * name)
{
	int i = 0;
	for (i = 0; i < numBones; i++)
	{
		if (strcmp(root[i].name, name) == 0){
			return root[i].id;
		}
	}
	return -1;
}


bool Skeleton::readASF(char* filename) {
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		printf("Failed to open file %s\n", filename);
		exit(EXIT_FAILURE);
	}
	char* buff = new char[buffSize];
	char *p;

	printf("Starting reading skeleton file\n");
	while ((p = fgets(buff, buffSize, file)) != NULL) {
		//Reading actually happened!

		char* start = buff;
		trim(&start);

		//Check if it is a comment or just empty
		if (buff[0] == '#' || buff[0] == '\0') {
			continue;
		}

		start = strchr(buff, ':');
		if (start != NULL) {
			//This actually is a heading
			//Reading may actually consume the next heading
			//so headings need to be a recursive construct?
			readHeading(buff, file);
		}
	}

	delete[] buff;
	fclose(file);
	printf("Completed reading skeleton file\n");
	return true;
}

/**
 * Trim the current string, by adding a null character into where the comments start
 */
void Skeleton::decomment(char * buff) {
	char* comStart = strchr(buff, '#');
	if (comStart != NULL) {
		//remove irrelevant part of string
		*comStart = '\0';
	}
}

void Skeleton::readHeading(char* buff, FILE* file) {
	char* temp = buff;
	decomment(buff);
	trim(&temp);

	char head[50];
	char rest[200];
	int num = sscanf(temp, ":%s %s", head, rest);
	if (num == 0) {
		printf("Could not get heading name from %s, all is lost\n", temp);
		exit(EXIT_FAILURE);
	}
	if (strcmp(head, "version") == 0) {
		//version string - must be 1.10
		char* version = rest;
		if (num != 2) {
			char *p = { '\0' };
			while (strlen(p) == 0) {
				char* p = fgets(buff, buffSize, file);
				decomment(p);
				trim(&p);
				version = p;
			}
		}
		if (strcmp(version, "1.10") != 0) {
			printf("Invalid version: %s, must be 1.10\n", version);
			exit(EXIT_FAILURE);
		}
		//Finished reading version so read the next thing?
	} else if (strcmp(head, "name") == 0) {
		//This allows the skeleton to be called something
		//other than the file name
		//We don't actually care what the name is, so can
		//ignore this whole section

	} else if (strcmp(head, "documentation") == 0) {
		//Documentation section has no meaningful information
		//only of use if you want to copy the file. So we skip it
	} else if (strcmp(head, "units") == 0) {
		//Has factors for the units
		//To be able to model the real person,
		//these must be parsed correctly
		//Only really need to check if deg or rad, but even
		//that is probably not needed for the core or extension
	} else if (strcmp(head, "root") == 0) {
		//Read in information about root
		//Or be lazy and just assume it is going to be the normal CMU thing!
	} else if (strcmp(head, "bonedata") == 0) {
		//Description of each bone
		//This does need to actually be read :(
		char *p;
		while ((p = fgets(buff, buffSize, file)) != NULL) {
			decomment(p);
			trim(&p);
			if (strlen(p) > 0) {
				if (p[0] == ':') {
					return readHeading(buff, file);
				} else if (strcmp(p, "begin") != 0) {
					printf("Expected begin for bone data %d, found \"%s\"", numBones, p);
					exit(EXIT_FAILURE);
				} else {
					readBone(buff, file);
					numBones++;
				}

			}
		}
	} else if (strcmp(head, "hierarchy") == 0) {
		//Description of how the bones fit together
		char *p;
		while ((p = fgets(buff, buffSize, file)) != NULL) {
			trim(&p);
			decomment(p);
			if (strlen(p) > 0) {
				if (p[0] == ':') {
					return readHeading(buff, file);
				} else if (strcmp(p, "begin") != 0) {
					printf("Expected begin in hierarchy, found %s", p);
					exit(EXIT_FAILURE);
				} else {
					readHierarchy(buff, file);
				}

			}
		}
	} else {
		printf("Unknown heading %s\n", head);
	}

}

void Skeleton::readHierarchy(char* buff, FILE* file) {
	char *p;
	char t1[200];
	while ((p = fgets(buff, buffSize, file)) != NULL) {
		trim(&p);
		decomment(p);
		if (strlen(p) > 0) {
			if (strcmp(p, "end") == 0) {
				//end of hierarchy
				return;
			} else {
				//Read the root node
				sscanf(p, "%s ", t1);
				bone* rootBone = NULL;
				for (int i = 0; i < numBones; i++) {
					if (strcmp(root[i].name, t1) == 0) {
						rootBone = root + i;
						break;
					}
				}
				//Read the connections
				p += strlen(t1);
				bone* other = NULL;
				while (*p != '\0') {
					sscanf(p, "%s ", t1);

					for (int i = 0; i < numBones; i++) {
						if (strcmp(root[i].name, t1) == 0) {
							other = root + i;
							break;
						}
					}
					if (other == NULL) {
						printf("Unknown bone %s found in hierarchy. Failure", t1);
						exit(EXIT_FAILURE);
					}
					rootBone->children[rootBone->numChildren] = other;
					rootBone->numChildren++;
					p += strlen(t1) + 1;

				}
			}
		}

	}
}

void Skeleton::readBone(char* buff, FILE* file) {
	char *p;
	char t1[50];
	while ((p = fgets(buff, buffSize, file)) != NULL) {
		trim(&p);
		decomment(p);
		if (strlen(p) > 0) {
			if (strcmp(p, "end") == 0) {
				//end of this bone
				return;
			} else {
				sscanf(p, "%s ", t1);
				if (strcmp(t1, "id") == 0) {
					//Read the id and actually remember it
					int id;
					sscanf(p, "id %d", &id);
					//printf("id: %d \n", id);
					root[numBones].id = id;
				} else if (strcmp(t1, "name") == 0) {
					//Read the name and actually remember it
					char* name = (char*) malloc(sizeof(char) * 10);
					sscanf(p, "name %s", name);
					//printf("name: %s \n", name);
					root[numBones].name = name;
				} else if (strcmp(t1, "direction") == 0) {
					//Also actually important
					float x, y, z;
					sscanf(p, "direction %f %f %f", &x, &y, &z);
					root[numBones].dirx = x;
					root[numBones].diry = y;
					root[numBones].dirz = z;
				} else if (strcmp(t1, "length") == 0) {
					//Also actually important
					float len;
					sscanf(p, "length %f", &len);
					root[numBones].length = len;
				} else if (strcmp(t1, "dof") == 0) {
					//Read the degrees of freedom!
					char d1[5];
					char d2[5];
					char d3[5];
					int num = sscanf(p, "dof %s %s %s", d1, d2, d3);
					switch (num) {
					DOF dof;
				case 3:
					dof = dofFromString(d3);
					root[numBones].dof = root[numBones].dof | dof;
					//fallthrough!!
					/* no break */
				case 2:
					dof = dofFromString(d2);
					root[numBones].dof = root[numBones].dof | dof;
					//fallthrough!!
					/* no break */
				case 1:
					dof = dofFromString(d1);
					root[numBones].dof = root[numBones].dof | dof;
					break;
					}
				} else if (strcmp(t1, "axis") == 0) {
					//Read the rotation axis
					float x, y, z;
					int num = sscanf(p, "axis %f %f %f XYZ", &x, &y, &z);
					if (num != 3) {
						printf("axis format doesn't match expected\n");
						printf("Expected: axis %%f %%f %%f XYZ\n");
						printf("Got: %s", p);
						exit(EXIT_FAILURE);
					}
					root[numBones].rotx = x;
					root[numBones].roty = y;
					root[numBones].rotz = z;
				}
				//There are more things but they are not needed for the core
			}

		}
	}
}

/**
 * Helper function to turn a DOF from the AMC file into the correct DOF value
 */
DOF Skeleton::dofFromString(char* s) {
	if (strcmp(s, "rx") == 0)
		return DOF_RX;
	if (strcmp(s, "ry") == 0)
		return DOF_RY;
	if (strcmp(s, "rz") == 0)
		return DOF_RZ;
	printf("Unknown DOF found: %s", s);
	return DOF_NONE;
}

/*
 * Remove leading and trailing whitespace. Increments the
 * pointer until it points to a non whitespace char
 * and then adds nulls to the end until it has no
 * whitespace on the end
 */
void trim(char **p) {
	if (p == NULL) {
		printf("File terminated without version number!\n");
		exit(EXIT_FAILURE);
	}

	//Remove leading whitespace
	while (**p == ' ' || **p == '\t') {
		(*p)++;
	}

	int len = strlen(*p);
	while (len > 0) {
		char last = (*p)[len - 1];
		if (last == '\r' || last == '\n' || last == ' ' || last == '\t') {
			(*p)[--len] = '\0';
		} else {
			return;
		}
	}
}


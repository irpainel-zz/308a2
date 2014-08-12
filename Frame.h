/*
 * Frame.h
 *
 *  Created on: Aug 11, 2014
 *      Author: iury
 */

#ifndef FRAME_H_
#define FRAME_H_

#include <string.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include "define.h"
#include "Posebone.h"
#include <vector>

using namespace std;

class Frame {

public:
	vector<Posebone> bonesPose;
	int frameNumber;

	Frame(int, int n);
	void setBoneRotation(int boneid, Posebone * p);

};


#endif /* FRAME_H_ */

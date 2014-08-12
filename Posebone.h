/*
 * pose.h
 *
 *  Created on: Aug 11, 2014
 *      Author: iury
 */

#ifndef POSE_H_
#define POSE_H_

#include <string.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include "define.h"


class Posebone {

public:
	int boneid;
	float rot[3];
	float trans[3];

private:

};


#endif /* POSE_H_ */

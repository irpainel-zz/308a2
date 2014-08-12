/*
 * Frame.cpp
 *
 *  Created on: Aug 11, 2014
 *      Author: iury
 */

#include <stdio.h>
#include<iostream>
#include "Frame.h"

Frame::Frame(int numBones, int n)
{
	Posebone * bonePose;
	int i;
	bonesPose.reserve(numBones);
	for (i =0; i < numBones; i++){
		bonePose = new Posebone();
		bonePose->boneid=-1;
		bonePose->rot[0] = 0;
		bonePose->rot[1] = 0;
		bonePose->rot[2] = 0;
		bonePose->trans[0] = 0;
		bonePose->trans[1] = 0;
		bonePose->trans[2] = 0;
		bonesPose.push_back(*bonePose);

	}
	frameNumber = n;
}

void Frame::setBoneRotation(int boneid, Posebone * p){
	bonesPose[boneid] = *p;
}



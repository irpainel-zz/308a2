#include "define.h"

#include <math.h>
#include <stdio.h>
#include<iostream>

using namespace std;

G308_Point crossP(G308_Point u, G308_Point v);

double dotP(G308_Point u, G308_Point v);

G308_Point diffVec(G308_Point a, G308_Point b);

float calcMagnitude(G308_Point v);

double calcAngle(G308_Point rotationAxis, G308_Point directionV);

void rotateInZ(double angle, double matrix[4][4]);

void rotateInY(double angle, double matrix[4][4]);

void rotateInX(double angle, double matrix[4][4]);

void multMatrix(double a[4][4], double b[4][4], double result[4][4]);

void castMatrix44to16(double a[4][4], double result[16]);

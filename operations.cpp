/*
 * math.cpp
 *
 *  Created on: Aug 11, 2014
 *      Author: iury
 */
#include "operations.h"


G308_Point crossP(G308_Point u, G308_Point v){
	G308_Point result;

	//(u y v z - u z v y )i + (u z v x - u x v z )j + (u x v y - u y v x )k
	result.x = (u.y*v.z-u.z*v.y);
	result.y = (u.z*v.x-u.x*v.z);
	result.z = (u.x*v.y-u.y*v.x);

	return result;
}

double dotP(G308_Point u, G308_Point v)
{
	return u.x*v.x + u.y*v.y + u.z*v.z;
}

G308_Point diffVec(G308_Point a, G308_Point b)
{
	G308_Point result;

	//vec A - vec B

	result.x = a.x - b.x;
	result.y = a.y - b.y;
	result.z = a.z - b.z;

	return result;
}

float calcMagnitude(G308_Point v)
{
	return sqrt((v.x*v.x)+(v.y*v.y)+(v.z*v.z));
}

double calcAngle(G308_Point rotationAxis, G308_Point directionV)
{

	double magnitudeU, result;

	result = acos(dotP(rotationAxis, directionV));
	magnitudeU = calcMagnitude(directionV);

	result = result / magnitudeU;

	//radians to degrees
	result = result*(180/PI);

	return result;
}



void rotateInZ(double angle, double matrix[4][4])
{
	angle = angle*(M_PI/180);
  matrix[0][0]=cos(angle); matrix[0][1]=-sin(angle); matrix[0][2]=0; matrix[0][3]=0;
  matrix[1][0]=sin(angle); matrix[1][1]= cos(angle); matrix[1][2]=0; matrix[1][3]=0;
  matrix[2][0]=0;      	   matrix[2][1]=0;           matrix[2][2]=1; matrix[2][3]=0;
  matrix[3][0]=0;          matrix[3][1]=0;           matrix[3][2]=0; matrix[3][3]=1;
}

void rotateInY(double angle, double matrix[4][4])
{
	angle = angle*(M_PI/180);
  matrix[0][0]=cos(angle);  matrix[0][1]=0;       matrix[0][2]=sin(angle);   matrix[0][3]=0;
  matrix[1][0]=0;           matrix[1][1]=1;       matrix[1][2]=0;            matrix[1][3]=0;
  matrix[2][0]=-sin(angle); matrix[2][1]=0;       matrix[2][2]=cos(angle);   matrix[2][3]=0;
  matrix[3][0]=0;           matrix[3][1]=0;       matrix[3][2]=0;            matrix[3][3]=1;
}

void rotateInX(double angle, double matrix[4][4])
{
	angle = angle*(M_PI/180);
  matrix[0][0]=1;       matrix[0][1]=0;           matrix[0][2]=0;           matrix[0][3]=0;
  matrix[1][0]=0;       matrix[1][1]=cos(angle);  matrix[1][2]=-sin(angle); matrix[1][3]=0;
  matrix[2][0]=0;       matrix[2][1]=sin(angle);  matrix[2][2]=cos(angle);  matrix[2][3]=0;
  matrix[3][0]=0;       matrix[3][1]=0;           matrix[3][2]=0;           matrix[3][3]=1;
}

void multMatrix(double a[4][4], double b[4][4], double result[4][4])
{
  int i, j, k;
  for(i=0;i<4;i++)
    for(j=0;j<4;j++)
    {
      result[i][j]=0;
      for(k=0;k<4;k++)
        result[i][j]+=a[i][k]*b[k][j];
    }
}

void rotationInXYZ(double ax, double ay, double az, double result[4][4])
{
	double RoZ[4][4];
	double RoY[4][4];
	double RoX[4][4];
	double MatrixA[4][4];
	rotateInZ(az, RoZ);
	rotateInY(ay, RoY);
	rotateInX(ax, RoX);
	multMatrix(RoZ, RoY, MatrixA);
	multMatrix(MatrixA, RoX, result);
}
void castMatrix44to16(double a[4][4],double result[16])
{
	int k=0;
	//cout << "===========\n";
	for (int i=0; i<4; i++)
	{
		for (int j=0; j<4; j++)
		{
			result[k] = a[j][i];
			//cout << result[k]<< " | ";
			k++;
		}
	}
	//cout << "\n";
}
void printMatrix(double a[4][4])
{
	cout << "===========\n";
	for (int i=0; i<4; i++)
	{
		for (int j=0; j<4; j++)
		{
			cout << " v: "<< a[i][j] << " ";
		}
		 cout << "\n";
	}
	cout << "===========\n";
}

/* transpose matrix*/
void matrixTranspose(double input[4][4], double result[4][4])
{
  int i, j;

  for (i=0; i<4; i++)
    for (j=0; j<4; j++)
      result[i][j] = input[j][i];
}


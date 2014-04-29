#include "Camera.h"

#include <iostream>
using namespace std;

Camera::Camera(void)
{
	/*
	gluLookAt(0.0f, 0.0f, -5.0f,
			  0.0f, 0.0f, 0.0f,
			  0.0f, 1.0f, 0.0f);
	*/
	camera.fields[0] = 0.0f;
	camera.fields[1] = 0.0f;
	camera.fields[2] = -10.0f;
	camera.fields[3] = 0.0f;
	camera.fields[4] = 0.0f;
	camera.fields[5] = 0.0f;
	camera.fields[6] = 0.0f;
	camera.fields[7] = 1.0f;
	camera.fields[8] = 0.0f;

	rot = 0.0f;

	posx = 0.0f;
	posy = 0.0f;
	posz = -10.0f;

	vectorx = 0.0f;
	vectory = 0.0f;
	vectorz = -1.0f;

	gluLookAt(camera.base.eyex, camera.base.eyey, camera.base.eyez,
			  camera.base.atx, camera.base.aty, camera.base.atz,
			  camera.base.upx, camera.base.upy, camera.base.upz);
}


Camera::~Camera(void)
{
}

void Camera::RotX()
{
	/*
		z' = z*cos q - x*sin q
		x' = z*sin q + x*cos q
		y' = y
	*/

	float tempz = camera.base.eyez * cos(rot) - camera.base.eyex * sin(rot);
	float tempx = camera.base.eyez * sin(rot) + camera.base.eyex * cos(rot);
	
	camera.base.eyez = tempz;
	camera.base.eyex = tempx;

	tempz = camera.base.atz * cos(rot) - camera.base.atx * sin(rot);
	tempx = camera.base.atz * sin(rot) + camera.base.atx * cos(rot);

	camera.base.atz = tempz;
	camera.base.atx = tempx;

	tempz = vectorz * cos(rot) - vectorx * sin(rot);
	tempx = vectorz * sin(rot) + vectorx * cos(rot);

	vectorz = tempz;
	vectorx = tempx;

	//cout << "Eye: " << camera.base.eyex << ", " << camera.base.eyey << ", " << camera.base.eyez << endl;
	//cout << "At: " << camera.base.atx << ", " << camera.base.aty << ", " << camera.base.atz << endl;
	//cout << vectorx << ", " << vectory << ", " << vectorz << endl; 
}

void Camera::IncRot(float angle)
{
	rot += angle;
}

void Camera::MoveY(float distance)
{
	posy += distance;
}

void Camera::Set()
{
	camera.fields[0] = 0.0f;
	camera.fields[1] = 0.0f;
	camera.fields[2] = 0.0f;
	camera.fields[3] = 0.0f;
	camera.fields[4] = 0.0f;
	camera.fields[5] = 10.0f;
	camera.fields[6] = 0.0f;
	camera.fields[7] = 1.0f;
	camera.fields[8] = 0.0f;

	vectorx = vectory = 0.0f;
	vectorz = -1.0f;


	// rotate then translate back to correct position
	RotX();	

	camera.base.eyex += posx;
	camera.base.eyey += posy;
	camera.base.eyez += posz;
	camera.base.atx += posx;
	camera.base.aty += posy;
	camera.base.atz += posz;

	gluLookAt(camera.base.eyex, camera.base.eyey, camera.base.eyez,
			  camera.base.atx, camera.base.aty, camera.base.atz,
			  camera.base.upx, camera.base.upy, camera.base.upz);
}

void Camera::MoveForward(float mod)
{
	posx -= vectorx * mod;
	posz -= vectorz * mod;
}

void Camera::MoveBackwards(float mod)
{
    posx += vectorx * mod;
	posz += vectorz * mod;
}

void Camera::MovePos(float x, float y, float z)
{
	camera.base.eyex += x;
	camera.base.eyey += y;
	camera.base.eyez += z;
	camera.base.atx += x;
	camera.base.aty += y;
	camera.base.atz += z;
}

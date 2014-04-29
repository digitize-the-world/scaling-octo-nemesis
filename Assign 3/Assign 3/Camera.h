#pragma once

#include "GL/glew.h"
#include "GL/freeglut.h"

class Camera
{
public:
	Camera(void);
	~Camera(void);

	void Set();
	void MoveForward(float mod);
	void MoveBackwards(float mod);
	void MovePos(float x, float y, float z);
	void RotX();
	void IncRot(float angle);
	void MoveY(float distance);
private:
	struct cam
	{
		float eyex, eyey, eyez;
		float atx, aty, atz;
		float upx, upy, upz;
	};

	union camVect
	{
		float fields[9];
		cam base;
	};

	camVect camera;

	float posx, posy, posz;
	float vectorx, vectory, vectorz;
	float rot;
	float matrix[16];
};


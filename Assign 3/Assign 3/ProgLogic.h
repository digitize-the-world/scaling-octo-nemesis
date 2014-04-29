#pragma once

#include "Camera.h"
#include "Mesh.h"
#include "HModel.h"
#include "Point.h"
#include "glsl.h"

#define LIGHT_FOLLOW	0x0
#define LIGHT_FIXED		0x1

#define PI (3.141592653589793)

struct DogActor
{
	Point3f position;
	Point3f dirVector;
	float speed;
	float rot;
	float startTime;
};

class ProgLogic
{
public:
	ProgLogic(void);
	~ProgLogic(void);

	void InitLogic(bool *, bool *);
	void UpdateLogic();
	void DrawObjects();

	float time, adjustTime, elapsed;
	char lightToggle;

	void InitDog();
	void UpdateDog();

private:
	Camera *cam;
	bool *stdInput, *specialInput;
	Mesh groundPlane;
	GLUquadric *quadratic;

	float updateSpeed;

	DogActor dogActor;
	HModel *dog;
	void CreateDog();
	static void DrawSphere();

	HModel *dogHead;
	HModel *dogLeg1;
	HModel *dogLeg2;
	HModel *dogLeg3;
	HModel *dogLeg4;
	HModel *dogEye1;
	HModel *dogEye2;
	HModel *dogEar1;
	HModel *dogEar2;
	HModel *dogSnout;
	HModel *dogNose;

	Mesh *tankMesh;

	int sceneState;

	cwc::glShaderManager SM;
	cwc::glShader *shader;

};


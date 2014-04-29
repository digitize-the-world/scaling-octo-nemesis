#pragma once
#include "GL\glew.h"
#include <vector>
#include "Point.h"


using namespace std;

class EMeshNamedObject
{
public:
	EMeshNamedObject();
	~EMeshNamedObject(void);
	void CreateBufferObjects();

	EMeshNamedObject *next;

	void Draw();

	vector<Point3f> vertices;
	vector<int> indices;
	vector<Point3f> normals;
	vector<Point2f> tex;
	char *name;
	short *numVerts, *numIndices;

	float* bVerts;
	unsigned short* bIndices;
	float* bNorms;
	float* bTex;

private:

};

class Mesh
{
public:
	Mesh(void);
	~Mesh(void);

	void Draw();
	void DrawBuffered();
	void LoadEMeshFromFile(char *filename);
	void CreateGroundPlane(float sizemod);
	void AddTexture(char *filename, int width, int height, int bitDepth);
	void DrawGround(GLint shaderId);

		GLuint tex;

private:
	EMeshNamedObject *head;
};


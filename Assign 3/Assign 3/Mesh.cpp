
#include "Mesh.h"
#include "GL\glew.h"
#include "GL\freeglut.h"
#include <fstream>
#include "glsl.h"

typedef unsigned char BYTE;

extern "C" unsigned char *stbi_load(char const *filename, int *x, int *y, int *comp, int req_comp);

using namespace std;


Mesh::Mesh(void)
{
	head = NULL;
}


Mesh::~Mesh(void)
{
}

void Mesh::DrawGround(GLint shaderId)
{
	GLfloat ambient[4] = {0.2f, 0.2f, 0.2f, 1.0f};
	GLfloat diffuse[4] = {0.8f, 0.8f, 0.8f, 1.0f};
	GLfloat specular[4] = {0.8f, 0.8f, 0.8f, 1.0f};
	GLfloat shine = 90.0f;
	GLfloat emission[4] = {0.0f, 0.0f, 0.0f, 1.0f};

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shine);
	glMaterialfv(GL_FRONT, GL_EMISSION, emission);

	EMeshNamedObject *curPos = head;

	// set to ground texture
	GLint flag = glGetUniformLocation(shaderId, "procFlag");
	glUniform1i(flag, 2);

	glBegin(GL_TRIANGLES);
	for (int i = 0; i < curPos->indices.size(); i++)
	{
		glNormal3f(curPos->normals[curPos->indices[i]].x, curPos->normals[curPos->indices[i]].y, curPos->normals[curPos->indices[i]].z);
		glTexCoord2f(curPos->tex[curPos->indices[i]].x, curPos->tex[curPos->indices[i]].y);
		glVertex3f(curPos->vertices[curPos->indices[i]].x, curPos->vertices[curPos->indices[i]].y, curPos->vertices[curPos->indices[i]].z);
	}
	glEnd();
}

void Mesh::Draw()
{
	EMeshNamedObject *curPos = head;

	glBegin(GL_TRIANGLES);
	for (int i = 0; i < curPos->indices.size(); i++)
	{
		glNormal3f(curPos->normals[curPos->indices[i]].x, curPos->normals[curPos->indices[i]].y, curPos->normals[curPos->indices[i]].z);
		glVertex3f(curPos->vertices[curPos->indices[i]].x, curPos->vertices[curPos->indices[i]].y, curPos->vertices[curPos->indices[i]].z);
	}
	glEnd();

	while ((curPos = curPos->next) != NULL)
	{
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < curPos->indices.size(); i++)
		{
			glNormal3f(curPos->normals[curPos->indices[i]].x, curPos->normals[curPos->indices[i]].y, curPos->normals[curPos->indices[i]].z);
			glVertex3f(curPos->vertices[curPos->indices[i]].x, curPos->vertices[curPos->indices[i]].y, curPos->vertices[curPos->indices[i]].z);
		}
		glEnd();
	}
}

EMeshNamedObject::EMeshNamedObject()
{
	numVerts = new short;
	numIndices = new short;
	next = NULL;
}

EMeshNamedObject::~EMeshNamedObject()
{
}


void Mesh::CreateGroundPlane(float sizemod)
{
	head = new EMeshNamedObject();
	head->vertices.push_back(Point3f(-1.0f * sizemod, 0.0f, -1.0f * sizemod));
	head->vertices.push_back(Point3f(-1.0f * sizemod, 0.0f, 1.0f * sizemod));
	head->vertices.push_back(Point3f(1.0f * sizemod, 0.0f, 1.0f * sizemod));
	head->vertices.push_back(Point3f(1.0f * sizemod, 0.0f, -1.0f * sizemod));

	head->indices.push_back(0);
	head->indices.push_back(1);
	head->indices.push_back(2);
	head->indices.push_back(0);
	head->indices.push_back(2);
	head->indices.push_back(3);
	

	head->normals.push_back(Point3f(0.0f, 1.0f, 0.0f));
	head->normals.push_back(Point3f(0.0f, 1.0f, 0.0f));
	head->normals.push_back(Point3f(0.0f, 1.0f, 0.0f));
	head->normals.push_back(Point3f(0.0f, 1.0f, 0.0f));

	head->tex.push_back(Point2f(0.0f, 0.0f));
	head->tex.push_back(Point2f(0.0f, 1.0f));
	head->tex.push_back(Point2f(1.0f, 1.0f));
	head->tex.push_back(Point2f(1.0f, 0.0f));
}

void Mesh::LoadEMeshFromFile(char *filename)
{
	// open file, if failed return
	ifstream instream(filename, ios::in | ios::binary);
	if (!instream.is_open())
		return;


	while (instream.peek() != EOF)
	{
		char* buffer = new char[50];
		ZeroMemory(buffer, 50);

		EMeshNamedObject *namedObject = new EMeshNamedObject();

		// read object name
		int i = 0;
		do
		{
			instream.read(buffer+i, 1);
			i++;
		}
		while (buffer[i-1] != NULL);

		// Create new named object
		namedObject->name = buffer;

		// Read the number of vertexes
		instream.read((char*)(namedObject->numVerts), sizeof(short));

		// Read the entire vertex data into memory
		for (int i = 0; i < *namedObject->numVerts; i++)
		{
			// create a new Point3f
			Point3f singlePoint3f, normal;
			Point2f texCoord;
			instream.read((char*)&(singlePoint3f), sizeof(Point3f));
			instream.read((char*)&(normal), sizeof(Point3f));
			instream.read((char*)&(texCoord), sizeof(Point2f));
			namedObject->vertices.push_back(singlePoint3f);
			namedObject->normals.push_back(normal);
			namedObject->tex.push_back(texCoord);
		}

		// Read the number of indices
		instream.read((char*)(namedObject->numIndices), sizeof(short));

		// Read the entire index data into memory
		for (int i = 0; i < *namedObject->numIndices * 3; i++)
		{
			short nextIndex;
			instream.read((char*)&(nextIndex), sizeof(short));
			namedObject->indices.push_back(nextIndex);
		}

		namedObject->CreateBufferObjects();

		if (head == NULL)
			head = namedObject;
		else
		{
			EMeshNamedObject *curPos = head;
			// put the object at the end of the linked list
			while (curPos->next != NULL)
				curPos = curPos->next;
			curPos->next = namedObject;
		}
	}

	instream.close();
}

void EMeshNamedObject::CreateBufferObjects()
{
	bVerts = new float[3 * *numVerts];
	bNorms = new float[3 * *numVerts];
	bTex = new float[2 * *numVerts];

	for (int i = 0; i < *numVerts; i++)
	{
		int offset = i * 3;
		bVerts[offset] = vertices[i].x;
		bVerts[offset + 1] = vertices[i].y;
		bVerts[offset + 2] = vertices[i].z;

		bNorms[offset] = normals[i].x;
		bNorms[offset + 1] = normals[i].y;
		bNorms[offset + 2] = normals[i].z;

		offset = i * 2;
		bTex[offset] = tex[i].x;
		bTex[offset + 1] = tex[i].y;
	}

	bIndices = new unsigned short[*numIndices * 3];
	for (int i = 0; i < *numIndices * 3; i++)
		bIndices[i] = indices[i];
}

void Mesh::DrawBuffered()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_INDEX_ARRAY);

	
	EMeshNamedObject *curPos = head;

	glVertexPointer(3, GL_FLOAT, 0, curPos->bVerts);
	glNormalPointer(GL_FLOAT, 0, curPos->bNorms);
	glTexCoordPointer(2, GL_FLOAT, 0, curPos->bTex);
	glIndexPointer(GL_FLOAT, 0, curPos->bIndices);

	int num = *curPos->numIndices * 3;
	glDrawElements(GL_TRIANGLES, num,  GL_UNSIGNED_SHORT, curPos->bIndices);
	
	while ((curPos = curPos->next) != NULL)
	{
		glVertexPointer(3, GL_FLOAT, 0, curPos->bVerts);
		glNormalPointer(GL_FLOAT, 0, curPos->bNorms);
		glTexCoordPointer(2, GL_FLOAT, 0, curPos->bTex);
		glIndexPointer(GL_FLOAT, 0, curPos->bIndices);

		glDrawElements(GL_TRIANGLES, (*curPos->numIndices) * 3,  GL_UNSIGNED_SHORT, curPos->bIndices);
	}
}

void Mesh::AddTexture(char *filename, int width, int height, int bitDepth)
{
	// load a texture using code supplied by the professor
	int loadCorrectly = 0;

	BYTE *image = stbi_load(filename, &width, &height, &bitDepth, 0);

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // not needed

	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glEnable (GL_TEXTURE_2D);
}


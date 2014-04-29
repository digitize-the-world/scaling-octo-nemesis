#pragma once
#include "GL/glew.h"
#include "GL/freeglut.h"

struct treenode
{
	GLfloat m[16];
	void (*draw)();
	treenode *sibling;
	treenode *child;
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat shine;
	GLfloat emission[4];
	float rot;
	GLuint tex;
	GLint procFlag;
};

class HModel
{
public:
	HModel(void);
	~HModel(void);

	treenode *modelNode;

	static void traverse(treenode *root, GLint shaderId);
	void AddTexture(char *filename, int width, int height, int bitDepth, GLuint *tex);
	GLint shaderId;
private:
};


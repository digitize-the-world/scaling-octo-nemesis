#include "HModel.h"

extern "C" unsigned char *stbi_load(char const *filename, int *x, int *y, int *comp, int req_comp);

HModel::HModel(void)
{
	modelNode = new treenode;
	modelNode->child = NULL;
	modelNode->sibling = NULL;

	modelNode->ambient[0] = 0.2f;
	modelNode->ambient[1] = 0.2f;
	modelNode->ambient[2] = 0.2f;
	modelNode->ambient[3] = 1.0f;
	modelNode->diffuse[0] = 0.8f;
	modelNode->diffuse[1] = 0.8f;
	modelNode->diffuse[2] = 0.8f;
	modelNode->diffuse[3] = 1.0f;
	modelNode->specular[0] = 0.0f;
	modelNode->specular[1] = 0.0f;
	modelNode->specular[2] = 0.0f;
	modelNode->specular[3] = 1.0f;
	modelNode->shine = 100.0f;
	modelNode->emission[0] = 0.0f;
	modelNode->emission[1] = 0.0f;
	modelNode->emission[2] = 0.0f;
	modelNode->emission[3] = 1.0f;

	modelNode->rot = 0.0f;
	modelNode->tex = NULL;
	modelNode->procFlag = 0;
}


HModel::~HModel(void)
{
}

void HModel::traverse(treenode *root, GLint shaderId)
{
	if (root == NULL)
		return;
	glPushMatrix();
	glMultMatrixf(root->m);

	glMaterialfv(GL_FRONT, GL_AMBIENT, root->ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, root->diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, root->specular);
	glMaterialf(GL_FRONT, GL_SHININESS, root->shine);
	glMaterialfv(GL_FRONT, GL_EMISSION, root->emission);

	GLint flag = glGetUniformLocation(shaderId, "procFlag");
	glUniform1i(flag, root->procFlag);
	glBindTexture(GL_TEXTURE_2D, root->tex);
	root->draw();
	if (root->child != NULL)
		traverse(root->child, shaderId);
	glPopMatrix();
	if (root->sibling != NULL)
		traverse(root->sibling, shaderId);
}

void HModel::AddTexture(char *filename, int width, int height, int bitDepth, GLuint *tex)
{
	// load a texture using code supplied by the professor
	int loadCorrectly = 0;

	BYTE *image = stbi_load(filename, &width, &height, &bitDepth, 0);

	glGenTextures(1, tex);
	glBindTexture(GL_TEXTURE_2D, *tex);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // not needed

	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glEnable (GL_TEXTURE_2D);
}

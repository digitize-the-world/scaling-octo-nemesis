#include "ProgLogic.h"

#include <iostream>
using namespace std;

ProgLogic::ProgLogic(void)
{
}


ProgLogic::~ProgLogic(void)
{
}

void ProgLogic::InitLogic(bool *std, bool *special)
{
	stdInput = std;
	specialInput = special;
	cam = new Camera();

	adjustTime = glutGet(GLUT_ELAPSED_TIME);
	time = glutGet(GLUT_ELAPSED_TIME) - adjustTime;

	lightToggle = LIGHT_FOLLOW;

	quadratic = gluNewQuadric(); // Create A Pointer To The Quadric Object 
	gluQuadricNormals(quadratic, GLU_SMOOTH); // Create Smooth Normals 

	groundPlane.CreateGroundPlane(10.0f);

	InitDog();
	updateSpeed = 1.0f;
	CreateDog();

	shader = SM.loadfromFile("vertexshader.txt", "fragmentshader.txt");
	if (shader == 0)
		cout << "Error loading, compiling or linking shader\n";

	tankMesh = new Mesh();
	tankMesh->LoadEMeshFromFile("building1.enigma");
	tankMesh->AddTexture("metal.bmp", 1024, 1024, 3);

	sceneState = 0;
}

void ProgLogic::UpdateLogic()
{
	elapsed = (GLfloat)glutGet(GLUT_ELAPSED_TIME) - time - adjustTime;
	time = (GLfloat)glutGet(GLUT_ELAPSED_TIME) - adjustTime;

	if (specialInput[GLUT_KEY_UP] == true)
		cam->MoveForward(0.015f * elapsed);
	if (specialInput[GLUT_KEY_DOWN] == true)
		cam->MoveBackwards(0.015f * elapsed);
	if (specialInput[GLUT_KEY_LEFT] == true)
		cam->IncRot(0.0015f * elapsed);
	if (specialInput[GLUT_KEY_RIGHT] == true)
		cam->IncRot(-0.0015f * elapsed);
	if (stdInput['a'] == true)
		cam->MoveY(0.01f * elapsed);
	if (stdInput['s'] == true)
		cam->MoveY(-0.01f * elapsed);
	if (stdInput['l'] == true)
		dogActor.rot -= 0.06f * elapsed;
	if (stdInput['k'] == true)
		dogActor.rot += 0.06f * elapsed;
	if (stdInput['f'] == true)
		updateSpeed += 0.002f * elapsed;
	if (stdInput['g'] == true)
	{
		updateSpeed -= 0.002f * elapsed;
		if (updateSpeed < 0.0f)
			updateSpeed = 0.0f;
	}


	if (stdInput['1'] == true)
	{
		sceneState = 0;
		glDisable(GL_BLEND);
	}
	if (stdInput['2'] == true)
	{
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		sceneState = 1;
	}

	cam->Set();
}

void ProgLogic::InitDog()
{
	dogActor.position = Point3f(0.0f, 0.0f, 0.0f);
	dogActor.dirVector = Point3f(-1.0f, 0.0f, 0.0f);
	dogActor.speed = 0.002f;
	dogActor.rot = 0.0f;
	dogActor.startTime = glutGet(GLUT_ELAPSED_TIME);
}

void ProgLogic::DrawObjects()
{
	GLfloat ambient[] = {0.0215f, 0.1745f, 0.0215f, 0.55f};
	GLfloat diffuse[] = {0.07568f, 0.61424f, 0.07568f, 0.55f};
	GLfloat specular[] = {0.633f, 0.727811f, 0.633f, 0.55f};
	GLfloat shine = 76.8f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shine);


	switch (sceneState)
	{
	case 0:
		glPushMatrix();
		glTranslatef(0.0f, -1.6f, 0.0f);
		if (shader)
		{
			shader->begin();
			groundPlane.DrawGround(shader->GetProgramObject());
		}
		if (shader) shader->end();
		glPopMatrix();

		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
	
		glMatrixMode(GL_MODELVIEW);

		glColor3f(0.0f, 0.0f, 0.0f);
		glTranslatef(dogActor.position.x, dogActor.position.y, dogActor.position.z);
		glRotatef(dogActor.rot, 0.0f, 1.0f, 0.0f);
		if (shader)
		{
			shader->begin();
			HModel::traverse(dog->modelNode, shader->GetProgramObject());
		}
		if (shader) shader->end();
		break;
	case 1:
		glPushMatrix();
		glTranslatef(0.0f, -1.6f, 0.0f);
		if (shader)
		{
			shader->begin();
			groundPlane.DrawGround(shader->GetProgramObject());
		}
		if (shader) shader->end();
		glPopMatrix();

		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
	
		glMatrixMode(GL_MODELVIEW);
		
		glPushMatrix();
		glTranslatef(0.0f, 0.0f, 3.0f);
		if (shader)
		{
			shader->begin();
			GLint flag = glGetUniformLocation(shader->GetProgramObject(), "procFlag");
			glUniform1i(flag, 1);
    
			//glutSolidTorus(0.5f, 1.0f, 10, 20);
		}
		if (shader) shader->end();
		glPopMatrix();

		

		glTranslatef(dogActor.position.x, dogActor.position.y, dogActor.position.z);
		glRotatef(dogActor.rot, 0.0f, 1.0f, 0.0f);

		//glDepthMask(GL_FALSE);
		
		glPushMatrix();

		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.0f, 0.0f, -1.5f);

		if (shader)
		{
			shader->begin();
			GLint flag = glGetUniformLocation(shader->GetProgramObject(), "procFlag");
			glUniform1i(flag, 3);

			glBindTexture(GL_TEXTURE_2D, tankMesh->tex);

			tankMesh->DrawBuffered();
		}
		if (shader) shader->end();

		glPopMatrix();

		glDepthMask(GL_TRUE);
		break;
	}
}

void ProgLogic::UpdateDog()
{
	float rotRads = dogActor.rot * (PI / 180);

	dogActor.dirVector.z = 1.0f * sin(rotRads);
	dogActor.dirVector.x = -1.0f * cos(rotRads);

	// update dog
	dogActor.position.x += dogActor.dirVector.x * dogActor.speed * elapsed * updateSpeed;
	dogActor.position.y += dogActor.dirVector.y * dogActor.speed * elapsed * updateSpeed;
	dogActor.position.z += dogActor.dirVector.z * dogActor.speed * elapsed * updateSpeed;

	static float rotdir1 = 0.02f;
	static float rotdir2 = -0.02f;
	static float rotdir3 = 0.02f;
	static float rotdir4 = -0.02f;

	if (dogLeg1->modelNode->rot > 40.0f)
	{
		dogLeg1->modelNode->rot = 40.0f;
		rotdir1 = -rotdir1;
	}
	else if (dogLeg1->modelNode->rot < -15.0f)
	{
		dogLeg1->modelNode->rot = -15.0f;
		rotdir1 = -rotdir1;
	}
	dogLeg1->modelNode->rot += rotdir1 * updateSpeed * elapsed;

	if (dogLeg2->modelNode->rot > 40.0f)
	{
		dogLeg2->modelNode->rot = 40.0f;
		rotdir2 = -rotdir2;
	}
	else if (dogLeg2->modelNode->rot < -15.0f)
	{
		dogLeg2->modelNode->rot = -15.0f;
		rotdir2 = -rotdir2;
	}
	dogLeg2->modelNode->rot += rotdir2 * updateSpeed * elapsed;

	if (dogLeg3->modelNode->rot > 15.0f)
	{
		dogLeg3->modelNode->rot = 15.0f;
		rotdir3 = -rotdir3;
	}
	else if (dogLeg3->modelNode->rot < -30.0f)
	{
		dogLeg3->modelNode->rot = -30.0f;
		rotdir3 = -rotdir3;
	}
	dogLeg3->modelNode->rot += rotdir3 * updateSpeed * elapsed;

	if (dogLeg4->modelNode->rot > 15.0f)
	{
		dogLeg4->modelNode->rot = 15.0f;
		rotdir4 = -rotdir4;
	}
	else if (dogLeg4->modelNode->rot < -30.0f)
	{
		dogLeg4->modelNode->rot = -30.0f;
		rotdir4 = -rotdir4;
	}
	dogLeg4->modelNode->rot += rotdir4 * updateSpeed * elapsed;

	glPushMatrix();
	glLoadIdentity();
	glRotatef(dogLeg1->modelNode->rot, 0.0f, 0.0f, 1.0f);
	glScalef(0.4f, 0.8f, 0.4f);
	glTranslatef(-2.0f, -1.0f, -1.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, dogLeg1->modelNode->m);
	glPopMatrix();

	glPushMatrix();
	glLoadIdentity();
	glRotatef(dogLeg2->modelNode->rot, 0.0f, 0.0f, 1.0f);
	glScalef(0.4f, 0.8f, 0.4f);
	glTranslatef(-2.0f, -1.0f, 1.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, dogLeg2->modelNode->m);
	glPopMatrix();

	glPushMatrix();
	glLoadIdentity();
	glRotatef(dogLeg3->modelNode->rot, 0.0f, 0.0f, 1.0f);
	glScalef(0.4f, 0.8f, 0.4f);
	glTranslatef(3.0f, -1.0f, -1.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, dogLeg3->modelNode->m);
	glPopMatrix();

	glPushMatrix();
	glLoadIdentity();
	glRotatef(dogLeg4->modelNode->rot, 0.0f, 0.0f, 1.0f);
	glScalef(0.4f, 0.8f, 0.4f);
	glTranslatef(3.0f, -1.0f, 1.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, dogLeg4->modelNode->m);
	glPopMatrix();

}

void ProgLogic::CreateDog()
{
	dog = new HModel();

	// create body
	glLoadIdentity();
	glScalef(2.0f, 1.0f, 1.0f);
	glRotatef(90, 0.0f, 1.0f, 0.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, dog->modelNode->m);
	dog->AddTexture("smiley.bmp", 1024, 1024, 3, &dog->modelNode->tex);
	dog->modelNode->ambient[0] = 0.25f;
	dog->modelNode->ambient[1] = 0.25f;
	dog->modelNode->ambient[2] = 0.25f;
	dog->modelNode->ambient[3] = 1.0f;
	dog->modelNode->diffuse[0] = 0.4f;
	dog->modelNode->diffuse[1] = 0.4f;
	dog->modelNode->diffuse[2] = 0.4f;
	dog->modelNode->diffuse[3] = 1.0f;
	dog->modelNode->specular[0] = 0.774597f;
	dog->modelNode->specular[1] = 0.774597f;
	dog->modelNode->specular[2] = 0.774597f;
	dog->modelNode->specular[3] = 1.0f;
	dog->modelNode->shine = 76.8f;
	dog->modelNode->emission[0] = 0.0f;
	dog->modelNode->emission[1] = 0.0f;
	dog->modelNode->emission[2] = 0.0f;
	dog->modelNode->emission[3] = 1.0f;
	dog->modelNode->draw = DrawSphere;

	// create head
	dogHead = new HModel();
	glLoadIdentity();
	glScalef(0.75f, 0.75f, 0.75f);
	glTranslatef(-2.5f, 0.7f, 0.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, dogHead->modelNode->m);
	dogHead->modelNode->procFlag = 4;
	dogHead->modelNode->draw = DrawSphere;
	dog->modelNode->sibling = dogHead->modelNode;
	dogHead->modelNode->ambient[0] = 0.25f;
	dogHead->modelNode->ambient[1] = 0.20725f;
	dogHead->modelNode->ambient[2] = 0.20725f;
	dogHead->modelNode->ambient[3] = 0.922f;
	dogHead->modelNode->diffuse[0] = 1.0f;
	dogHead->modelNode->diffuse[1] = 0.829f;
	dogHead->modelNode->diffuse[2] = 0.829f;
	dogHead->modelNode->diffuse[3] = 0.922f;
	dogHead->modelNode->specular[0] = 0.296648f;
	dogHead->modelNode->specular[1] = 0.296648f;
	dogHead->modelNode->specular[2] = 0.296648f;
	dogHead->modelNode->specular[3] = 0.922f;
	dogHead->modelNode->shine = 11.264f;
	dogHead->modelNode->emission[0] = 0.0f;
	dogHead->modelNode->emission[1] = 0.0f;
	dogHead->modelNode->emission[2] = 0.0f;
	dogHead->modelNode->emission[3] = 1.0f;

	// create leg1
	dogLeg1 = new HModel();
	glLoadIdentity();
	glScalef(0.4f, 0.8f, 0.4f);
	glTranslatef(-2.0f, -1.0f, -1.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, dogLeg1->modelNode->m);
	dogLeg1->modelNode->procFlag = 1;
	dogLeg1->modelNode->draw = DrawSphere;
	dogLeg1->modelNode->ambient[0] = 0.25f;
	dogLeg1->modelNode->ambient[1] = 0.20725f;
	dogLeg1->modelNode->ambient[2] = 0.20725f;
	dogLeg1->modelNode->ambient[3] = 0.922f;
	dogLeg1->modelNode->diffuse[0] = 1.0f;
	dogLeg1->modelNode->diffuse[1] = 0.829f;
	dogLeg1->modelNode->diffuse[2] = 0.829f;
	dogLeg1->modelNode->diffuse[3] = 0.922f;
	dogLeg1->modelNode->specular[0] = 0.296648f;
	dogLeg1->modelNode->specular[1] = 0.296648f;
	dogLeg1->modelNode->specular[2] = 0.296648f;
	dogLeg1->modelNode->specular[3] = 0.922f;
	dogLeg1->modelNode->shine = 11.264f;
	dogLeg1->modelNode->emission[0] = 0.0f;
	dogLeg1->modelNode->emission[1] = 0.0f;
	dogLeg1->modelNode->emission[2] = 0.0f;
	dogLeg1->modelNode->emission[3] = 1.0f;

	// create leg2
	dogLeg2 = new HModel();
	glLoadIdentity();
	glScalef(0.4f, 0.8f, 0.4f);
	glTranslatef(-2.0f, -1.0f, 1.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, dogLeg2->modelNode->m);
	dogLeg2->modelNode->procFlag = 1;
	dogLeg2->modelNode->draw = DrawSphere;
	dogLeg1->modelNode->sibling = dogLeg2->modelNode;
	dogLeg2->modelNode->ambient[0] = 0.25f;
	dogLeg2->modelNode->ambient[1] = 0.20725f;
	dogLeg2->modelNode->ambient[2] = 0.20725f;
	dogLeg2->modelNode->ambient[3] = 0.922f;
	dogLeg2->modelNode->diffuse[0] = 1.0f;
	dogLeg2->modelNode->diffuse[1] = 0.829f;
	dogLeg2->modelNode->diffuse[2] = 0.829f;
	dogLeg2->modelNode->diffuse[3] = 0.922f;
	dogLeg2->modelNode->specular[0] = 0.296648f;
	dogLeg2->modelNode->specular[1] = 0.296648f;
	dogLeg2->modelNode->specular[2] = 0.296648f;
	dogLeg2->modelNode->specular[3] = 0.922f;
	dogLeg2->modelNode->shine = 11.264f;
	dogLeg2->modelNode->emission[0] = 0.0f;
	dogLeg2->modelNode->emission[1] = 0.0f;
	dogLeg2->modelNode->emission[2] = 0.0f;
	dogLeg2->modelNode->emission[3] = 1.0f;

	// create leg3
	dogLeg3 = new HModel();
	glLoadIdentity();
	glScalef(0.4f, 0.8f, 0.4f);
	glTranslatef(3.0f, -1.0f, -1.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, dogLeg3->modelNode->m);
	dogLeg3->modelNode->procFlag = 1;
	dogLeg3->modelNode->draw = DrawSphere;
	dogLeg2->modelNode->sibling = dogLeg3->modelNode;
	dogLeg3->modelNode->ambient[0] = 0.25f;
	dogLeg3->modelNode->ambient[1] = 0.20725f;
	dogLeg3->modelNode->ambient[2] = 0.20725f;
	dogLeg3->modelNode->ambient[3] = 0.922f;
	dogLeg3->modelNode->diffuse[0] = 1.0f;
	dogLeg3->modelNode->diffuse[1] = 0.829f;
	dogLeg3->modelNode->diffuse[2] = 0.829f;
	dogLeg3->modelNode->diffuse[3] = 0.922f;
	dogLeg3->modelNode->specular[0] = 0.296648f;
	dogLeg3->modelNode->specular[1] = 0.296648f;
	dogLeg3->modelNode->specular[2] = 0.296648f;
	dogLeg3->modelNode->specular[3] = 0.922f;
	dogLeg3->modelNode->shine = 11.264f;
	dogLeg3->modelNode->emission[0] = 0.0f;
	dogLeg3->modelNode->emission[1] = 0.0f;
	dogLeg3->modelNode->emission[2] = 0.0f;
	dogLeg3->modelNode->emission[3] = 1.0f;

	// create leg4
	dogLeg4 = new HModel();
	glLoadIdentity();
	glScalef(0.4f, 0.8f, 0.4f);
	glTranslatef(3.0f, -1.0f, 1.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, dogLeg4->modelNode->m);
	dogLeg4->modelNode->procFlag = 1;
	dogLeg4->modelNode->draw = DrawSphere;
	dogLeg3->modelNode->sibling = dogLeg4->modelNode;
	dogLeg4->modelNode->ambient[0] = 0.25f;
	dogLeg4->modelNode->ambient[1] = 0.20725f;
	dogLeg4->modelNode->ambient[2] = 0.20725f;
	dogLeg4->modelNode->ambient[3] = 0.922f;
	dogLeg4->modelNode->diffuse[0] = 1.0f;
	dogLeg4->modelNode->diffuse[1] = 0.829f;
	dogLeg4->modelNode->diffuse[2] = 0.829f;
	dogLeg4->modelNode->diffuse[3] = 0.922f;
	dogLeg4->modelNode->specular[0] = 0.296648f;
	dogLeg4->modelNode->specular[1] = 0.296648f;
	dogLeg4->modelNode->specular[2] = 0.296648f;
	dogLeg4->modelNode->specular[3] = 0.922f;
	dogLeg4->modelNode->shine = 11.264f;
	dogLeg4->modelNode->emission[0] = 0.0f;
	dogLeg4->modelNode->emission[1] = 0.0f;
	dogLeg4->modelNode->emission[2] = 0.0f;
	dogLeg4->modelNode->emission[3] = 1.0f;

	// create eye1
	dogEye1 = new HModel();
	glLoadIdentity();
	glScalef(0.1f, 0.1f, 0.1f);
	glTranslatef(-24.0f, 9.0f, -3.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, dogEye1->modelNode->m);
	dogEye1->modelNode->procFlag = 4;
	dogEye1->modelNode->draw = DrawSphere;
	dogHead->modelNode->sibling = dogEye1->modelNode;
	dogEye1->modelNode->ambient[0] = 0.2f;
	dogEye1->modelNode->ambient[1] = 0.0f;
	dogEye1->modelNode->ambient[2] = 0.0f;
	dogEye1->modelNode->ambient[3] = 1.0f;
	dogEye1->modelNode->diffuse[0] = 0.8f;
	dogEye1->modelNode->diffuse[1] = 0.0f;
	dogEye1->modelNode->diffuse[2] = 0.0f;
	dogEye1->modelNode->diffuse[3] = 1.0f;
	dogEye1->modelNode->specular[0] = 1.0f;
	dogEye1->modelNode->specular[1] = 0.0f;
	dogEye1->modelNode->specular[2] = 0.0f;
	dogEye1->modelNode->specular[3] = 1.0f;
	dogEye1->modelNode->shine = 25.0f;
	dogEye1->modelNode->emission[0] = 0.0f;
	dogEye1->modelNode->emission[1] = 0.3f;
	dogEye1->modelNode->emission[2] = 0.3f;
	dogEye1->modelNode->emission[3] = 1.0f;


	// create eye2
	dogEye2 = new HModel();
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, 6.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, dogEye2->modelNode->m);
	dogEye2->modelNode->procFlag = 4;
	dogEye2->modelNode->draw = DrawSphere;
	dogEye1->modelNode->child = dogEye2->modelNode;
	dogEye2->modelNode->ambient[0] = 0.2f;
	dogEye2->modelNode->ambient[1] = 0.0f;
	dogEye2->modelNode->ambient[2] = 0.0f;
	dogEye2->modelNode->ambient[3] = 1.0f;
	dogEye2->modelNode->diffuse[0] = 0.8f;
	dogEye2->modelNode->diffuse[1] = 0.0f;
	dogEye2->modelNode->diffuse[2] = 0.0f;
	dogEye2->modelNode->diffuse[3] = 1.0f;
	dogEye2->modelNode->specular[0] = 1.0f;
	dogEye2->modelNode->specular[1] = 0.0f;
	dogEye2->modelNode->specular[2] = 0.0f;
	dogEye2->modelNode->specular[3] = 1.0f;
	dogEye2->modelNode->shine = 25.0f;
	dogEye2->modelNode->emission[0] = 0.0f;
	dogEye2->modelNode->emission[1] = 0.3f;
	dogEye2->modelNode->emission[2] = 0.3f;
	dogEye2->modelNode->emission[3] = 1.0f;

	// create ear1
	dogEar1 = new HModel();
	glLoadIdentity();
	glRotatef(30.0f, 1.0f, 0.0f, 0.0f);
	glScalef(0.3f, 0.6f, 0.3f);
	glTranslatef(-6.0f, -0.2f, -3.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, dogEar1->modelNode->m);
	dogEar1->modelNode->procFlag = 4;
	dogEar1->modelNode->draw = DrawSphere;
	dogEye1->modelNode->sibling = dogEar1->modelNode;
	dogEar1->modelNode->ambient[0] = 0.2f;
	dogEar1->modelNode->ambient[1] = 0.0f;
	dogEar1->modelNode->ambient[2] = 0.0f;
	dogEar1->modelNode->ambient[3] = 1.0f;
	dogEar1->modelNode->diffuse[0] = 0.8f;
	dogEar1->modelNode->diffuse[1] = 0.0f;
	dogEar1->modelNode->diffuse[2] = 0.0f;
	dogEar1->modelNode->diffuse[3] = 1.0f;
	dogEar1->modelNode->specular[0] = 1.0f;
	dogEar1->modelNode->specular[1] = 0.0f;
	dogEar1->modelNode->specular[2] = 0.0f;
	dogEar1->modelNode->specular[3] = 1.0f;
	dogEar1->modelNode->shine = 25.0f;
	dogEar1->modelNode->emission[0] = 0.0f;
	dogEar1->modelNode->emission[1] = 0.3f;
	dogEar1->modelNode->emission[2] = 0.3f;
	dogEar1->modelNode->emission[3] = 1.0f;

	// create ear2
	dogEar2 = new HModel();
	glLoadIdentity();
	glRotatef(-30.0f, 1.0f, 0.0f, 0.0f);
	glScalef(0.3f, 0.6f, 0.3f);
	glTranslatef(-6.0f, -0.2f, 3.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, dogEar2->modelNode->m);
	dogEar2->modelNode->procFlag = 4;
	dogEar2->modelNode->draw = DrawSphere;
	dogEar1->modelNode->sibling = dogEar2->modelNode;
	dogEar2->modelNode->sibling = dogLeg1->modelNode;
	dogEar2->modelNode->ambient[0] = 0.2f;
	dogEar2->modelNode->ambient[1] = 0.0f;
	dogEar2->modelNode->ambient[2] = 0.0f;
	dogEar2->modelNode->ambient[3] = 1.0f;
	dogEar2->modelNode->diffuse[0] = 0.8f;
	dogEar2->modelNode->diffuse[1] = 0.0f;
	dogEar2->modelNode->diffuse[2] = 0.0f;
	dogEar2->modelNode->diffuse[3] = 1.0f;
	dogEar2->modelNode->specular[0] = 1.0f;
	dogEar2->modelNode->specular[1] = 0.0f;
	dogEar2->modelNode->specular[2] = 0.0f;
	dogEar2->modelNode->specular[3] = 1.0f;
	dogEar2->modelNode->shine = 25.0f;
	dogEar2->modelNode->emission[0] = 0.0f;
	dogEar2->modelNode->emission[1] = 0.3f;
	dogEar2->modelNode->emission[2] = 0.3f;
	dogEar2->modelNode->emission[3] = 1.0f;


	// create snout
	dogSnout = new HModel();
	glLoadIdentity();
	glScalef(0.5f, 0.5f, 0.5f);
	glScalef(1.5f, 1.0f, 1.0f);
	glTranslatef(-1.5f, -0.25f, 0.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, dogSnout->modelNode->m);
	dogSnout->modelNode->procFlag = 4;
	dogSnout->modelNode->draw = DrawSphere;
	dogHead->modelNode->child = dogSnout->modelNode;
	dogSnout->modelNode->ambient[0] = 0.25f;
	dogSnout->modelNode->ambient[1] = 0.20725f;
	dogSnout->modelNode->ambient[2] = 0.20725f;
	dogSnout->modelNode->ambient[3] = 0.922f;
	dogSnout->modelNode->diffuse[0] = 1.0f;
	dogSnout->modelNode->diffuse[1] = 0.829f;
	dogSnout->modelNode->diffuse[2] = 0.829f;
	dogSnout->modelNode->diffuse[3] = 0.922f;
	dogSnout->modelNode->specular[0] = 0.296648f;
	dogSnout->modelNode->specular[1] = 0.296648f;
	dogSnout->modelNode->specular[2] = 0.296648f;
	dogSnout->modelNode->specular[3] = 0.922f;
	dogSnout->modelNode->shine = 11.264f;
	dogSnout->modelNode->emission[0] = 0.0f;
	dogSnout->modelNode->emission[1] = 0.0f;
	dogSnout->modelNode->emission[2] = 0.0f;
	dogSnout->modelNode->emission[3] = 1.0f;

	// create nose
	dogNose = new HModel();
	glLoadIdentity();
	glScalef(0.15f, 0.15f, 0.15f);
	glTranslatef(-12.0f, 0.0f, 0.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, dogNose->modelNode->m);
	dogNose->modelNode->procFlag = 4;
	dogNose->modelNode->draw = DrawSphere;
	dogSnout->modelNode->sibling = dogNose->modelNode;
	dogNose->modelNode->ambient[0] = 0.2f;
	dogNose->modelNode->ambient[1] = 0.0f;
	dogNose->modelNode->ambient[2] = 0.0f;
	dogNose->modelNode->ambient[3] = 1.0f;
	dogNose->modelNode->diffuse[0] = 0.8f;
	dogNose->modelNode->diffuse[1] = 0.0f;
	dogNose->modelNode->diffuse[2] = 0.0f;
	dogNose->modelNode->diffuse[3] = 1.0f;
	dogNose->modelNode->specular[0] = 1.0f;
	dogNose->modelNode->specular[1] = 0.0f;
	dogNose->modelNode->specular[2] = 0.0f;
	dogNose->modelNode->specular[3] = 1.0f;
	dogNose->modelNode->shine = 25.0f;
	dogNose->modelNode->emission[0] = 0.0f;
	dogNose->modelNode->emission[1] = 0.3f;
	dogNose->modelNode->emission[2] = 0.3f;
	dogNose->modelNode->emission[3] = 1.0f;

}

void ProgLogic::DrawSphere()
{
	GLUquadric *quadratic = gluNewQuadric(); // Create A Pointer To The Quadric Object 
	gluQuadricNormals(quadratic, GLU_SMOOTH); // Create Smooth Normals 
	gluQuadricTexture(quadratic, TRUE);
	gluSphere(quadratic, 1.0, 20, 20);
}
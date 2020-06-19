// ConsoleApplication5.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#define NEW_APPLE
#ifdef NEW_APPLE

#include <windows.h>
#include <stdio.h>
#include <gl\glut.h>
#include <GL/freeglut.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <math.h>

#include "GL/bmp.h"   // replacing the obsolate file : <gl/glaux.h>


#define MAX_NO_TEXTURES 2

GLuint texture[MAX_NO_TEXTURES];

static float x = 0.0f, y = 0.0f, z = 3.0f;
static float lx = 0.0f, ly = 0.0f, lz = -1.0f;

AUX_RGBImageRec* auxDIBImageLoad(const char* FileName);


AUX_RGBImageRec* LoadBMP(char* Filename)    // Loads A Bitmap Image
{
	FILE* File = NULL;         // File Handle

	if (!Filename)          // Make Sure A Filename Was Given
	{
		return NULL;         // If Not Return NULL
	}

	File = fopen(Filename, "r");       // Check To See If The File Exists

	if (File)           // Does The File Exist?
	{
		fclose(File);         // Close The Handle
		return auxDIBImageLoad((const char *)Filename);    // Load The Bitmap And Return A Pointer
	}

	return NULL;          // If Load Failed Return NULL
}
void init(void)
{
	glShadeModel(GL_SMOOTH);       // Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);    // Black Background
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glEnable(GL_TEXTURE_2D);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(MAX_NO_TEXTURES, texture);

	AUX_RGBImageRec* TextureImage[MAX_NO_TEXTURES];     // Create Storage Space For The Texture

	memset(TextureImage, 0, sizeof(void*) * MAX_NO_TEXTURES);            // Set The Pointer To NULL



																		 // if ((TextureImage[0] = LoadBMP("삼성 4-1 SLIDE(2)_00001.bmp")) &&
																		 //  (TextureImage[1] = LoadBMP("삼성 4-1 SLIDE(2)_00002.bmp")))   //이미지 로딩
	TextureImage[0] = LoadBMP((char *)"./apple.bmp");
	TextureImage[1] = LoadBMP((char*)"./apple.bmp");


	{
		glGenTextures(MAX_NO_TEXTURES, texture);     //텍스쳐 생성

													 //텍스쳐에 이미지 1 넣기
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		//텍스쳐에 이미지 2 넣기
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		// glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[1]->sizeX, TextureImage[1]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[1]->data);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[1]->sizeX, TextureImage[1]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[1]->data);

	}
	//////////////////////////
	glEnable(GL_CULL_FACE);
	// glEnable(GL_DEPTH_TEST);
}
void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


	//텍스쳐 출력
	// glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	glBegin(GL_QUADS);
	// Front Face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 0.0f, 0.0f);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.2f, -0.2f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -0.2f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.2f, 1.0f, 0.0f);
	glEnd();
	//////////////////////////

	// glFlush();
	glDisable(GL_TEXTURE_2D);
	glutSwapBuffers();
}
void changeSize(int w, int h)
{
	//창이 아주 작을 때, 0 으로 나누는 것을 예방합니다.
	if (h == 0)
		h = 1;
	float ratio = 1.0 * w / h;

	//좌표계를 수정하기 전에 초기화합니다.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//뷰포트를 창의 전체 크기로 설정합니다.
	glViewport(0, 0, w, h);

	//투시값을 설정합니다.
	gluPerspective(45, ratio, 0, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);

}

void moveMeFlat(int direction)
{
	// x = x + direction*(lx)*0.01;
	z = z + direction * (lz) * 0.03;
	if (z < 0.0f) {
		z = 0.0f;
	}
	glLoadIdentity();
	gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
	glutPostRedisplay();
}
void moveMeFlatx(int direction)
{
	x = x + direction * 0.03;
	glLoadIdentity();
	gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
	glutPostRedisplay();
}
void moveMeFlaty(int direction)
{
	y = y + direction * 0.03;
	glLoadIdentity();
	gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
	glutPostRedisplay();
}
void inputKey(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_RIGHT:
		moveMeFlatx(-1);
		break;
	case GLUT_KEY_LEFT:
		moveMeFlatx(1);
		break;
	case GLUT_KEY_UP:
		moveMeFlaty(-1);
		break;
	case GLUT_KEY_DOWN:
		moveMeFlaty(1);
		break;
	case GLUT_KEY_F1:
		moveMeFlat(1);
		break;
	case GLUT_KEY_F2:
		moveMeFlat(-1);
		break;
	}
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(800, 600);
	glutCreateWindow("3D Tech- GLUT Tutorial");

	init();
	glutSpecialFunc(inputKey);
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

	glutMainLoop();
}
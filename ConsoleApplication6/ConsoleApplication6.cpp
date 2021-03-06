#include "stdafx.h"
#pragma warning(disable:4996)
#include <Windows.h>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <glut.h>
#include <bmp.h>

#define PI 3.141592
#define MAX_NO_TEXTURES 5

//texture
GLuint texture[MAX_NO_TEXTURES];
AUX_RGBImageRec* auxDIBImageLoad(const char* FileName);



float x = 0, y = 1.0, z = 25;//관찰자 위치지정
float xview = 0, yview = 0, zview = -1;//관찰자의 초점
float alpha = 0;//회전 각도
float ground_x = 20, ground_z = 30;//땅 크기
float moving = 0.1f;// 앞, 뒤 움직일 시 필요

					//각 건물들의 위치 지정
float places[5][3] = { { 15.5, 0, 20.5 },//기숙사 건물 위치
{ -14, 0,  12.5 },//자연대 건물 위치
{ -7, 0, -8 },//공학관 건물 
{ 14.7, 0, -12.5 } };//도서관 건물 위치






					 ////////////////////////////////// texture 쪽 선언 ////////////////////////////////////////////////
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
		return auxDIBImageLoad((const char*)Filename);    // Load The Bitmap And Return A Pointer
	}

	return NULL;          // If Load Failed Return NULL
}

void texture_init(void)
{
	glShadeModel(GL_SMOOTH);       // Enable Smooth Shading
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glEnable(GL_TEXTURE_2D);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(MAX_NO_TEXTURES, texture);

	AUX_RGBImageRec* TextureImage[MAX_NO_TEXTURES];     // Create Storage Space For The Texture

	memset(TextureImage, 0, sizeof(void*) * MAX_NO_TEXTURES);            // Set The Pointer To NULL

	TextureImage[0] = LoadBMP((char*)"./s.bmp"); //기숙사
	TextureImage[1] = LoadBMP((char*)"./j.bmp"); //자연대
	TextureImage[2] = LoadBMP((char*)"./g.bmp"); //공대
	TextureImage[3] = LoadBMP((char*)"./d.bmp"); //도서관
	TextureImage[4] = LoadBMP((char*)"./m.bmp"); //명수당


	{
		glGenTextures(MAX_NO_TEXTURES, texture);     //텍스쳐 생성

													 //텍스쳐에 기숙사 이미지 넣기
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		//텍스쳐에 자연대 이미지 넣기
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[1]->sizeX, TextureImage[1]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[1]->data);
		//텍스쳐에 공대 이미지 넣기
		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[2]->sizeX, TextureImage[2]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[2]->data);
		//텍스쳐에 도서관 이미지 넣기
		glBindTexture(GL_TEXTURE_2D, texture[3]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[3]->sizeX, TextureImage[3]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[3]->data);
		//텍스쳐에 명수당 이미지 넣기
		glBindTexture(GL_TEXTURE_2D, texture[4]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[4]->sizeX, TextureImage[4]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[4]->data);

	}
	//////////////////////////
	glEnable(GL_CULL_FACE);
	// glEnable(GL_DEPTH_TEST);
}



////////////////////////////////////////// 지도 그리는 쪽 ///////////////////////////////////////////////////////
//건물 바닥면 그리기
void drawsGroundBox() {
	glBegin(GL_QUADS); {
		glColor3f(0.9, 0.9, 0.9);
		glNormal3f(0.0, 1.0, 0.0); //법선벡터
		glVertex3f(2.3, 0.015, 2.3);
		glVertex3f(2.3, 0.015, -2.3);
		glVertex3f(-2.3, 0.015, -2.3);
		glVertex3f(-2.3, 0.015, 2.3);
	} glEnd();
}

//정육면체 건물들 그리기
void drawCube() {

	glBegin(GL_QUADS); {
		glColor3f(0.07058, 0.46274, 0.50588);

		// 윗면 (높이 4, 가로 세로 1.3)  - 반시계
		glNormal3f(0.0, 1.0, 0.0); // 법선벡터
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1.3, 4.0, -1.3);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.3, 4.0, -1.3);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.3, 4.0, 1.3);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1.3, 4.0, 1.3);

		// 바닥 (높이0, 가로 세로 1.3) - 시계
		glNormal3f(0.0, -1.0, 0.0); // 법선벡터
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1.3, 0.0, 1.3);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.3, 0.0, 1.3);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.3, 0.0, -1.3);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1.3, 0.0, -1.3);

		// 앞면 (높이 4, 가로 세로 1.3) - 반시계
		glNormal3f(0.0, 0.0, 1.0); // 법선벡터
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1.3, 4.0, 1.3);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.3, 4.0, 1.3);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.3, 0.0, 1.3);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1.3, 0.0, 1.3);

		// 뒷면 (높이 4, 가로 세로 1.3) - 시계
		glNormal3f(0.0, 0.0, -1.0); // 법선벡터
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1.3, 0.0, -1.3);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.3, 0.0, -1.3);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.3, 4.0, -1.3);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1.3, 4.0, -1.3);

		// 왼쪽면 (높이4, 가로 세로 1.3) - 반시계
		glNormal3f(-1.0, 0.0, 0.0); // 법선벡터
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.3, 4.0, 1.3);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.3, 4.0, -1.3);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.3, 0.0, -1.3);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.3, 0.0, 1.3);

		// 오른쪽 면(높이4, 가로 세로 1.3) - 시계
		glNormal3f(1.0, 0.0, .0); // 법선벡터
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1.3, 4.0, -1.3);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1.3, 4.0, 1.3);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1.3, 0.0, 1.3);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1.3, 0.0, -1.3);
	} glEnd();
}


//땅, 도로 둘다 그리기
void drawGround() {
	glColor3f(0.6, 0.4, 0.2);

	glBegin(GL_QUADS);
	// 땅 그리기
	glNormal3f(0.0, 1.0, 0.0); // 법선벡터
	glVertex3f(-ground_x, 0, -ground_z);
	glVertex3f(-ground_x, 0, ground_z);
	glVertex3f(ground_x, 0, ground_z);
	glVertex3f(ground_x, 0, -ground_z);
	glEnd();


	//도로 그리기 (평행 사변형은 삼각형 두개 합친거)
	glColor3f(0.6, 0.6, 0.6);
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(-7, 0.01, 30);
	glVertex3f(3, 0.01, 30);
	glVertex3f(-3, 0.1, -30);
	glVertex3f(7, 0.1, -30);
	glEnd();

	//제일 첫번째 substreet는 길게
	glBegin(GL_QUADS);
	glVertex3f(2.5, 0.01, 23);
	glVertex3f(14.5, 0.01, 23);
	glVertex3f(14.5, 0.01, 18);
	glVertex3f(2.5, 0.01, 18);
	glEnd();

	//두번째 substreet
	glBegin(GL_QUADS);
	glVertex3f(-12.5, 0.01, 15);
	glVertex3f(-2.5, 0.01, 15);
	glVertex3f(-2.5, 0.01, 10);
	glVertex3f(-12.5, 0.01, 10);
	glEnd();

	//세번째 substreet
	glBegin(GL_QUADS);
	glVertex3f(2.5, 0.01, -10);
	glVertex3f(12.5, 0.01, -10);
	glVertex3f(12.5, 0.01, -15);
	glVertex3f(2.5, 0.01, -15);
	glEnd();

	//명쑤당 그리기
	glBegin(GL_QUADS);
	glColor3f(0.0, 0.0, 1);
	glVertex3f(-18, 0.01, -15);
	glVertex3f(-5.5, 0.01, -15);
	glVertex3f(-5.5, 0.01, -25);
	glVertex3f(-18, 0.01, -25);
	glEnd();
}

//윈도우 사이즈 변경 처리
void changeSize(int w, int h) {
	h = (h > 0 ? h : 1);
	float ratio = (w * 1.0) / h;

	// 투영 방법 설정
	glMatrixMode(GL_PROJECTION);

	// 좌표변환행령 초기화
	glLoadIdentity();

	// 전체 윈도우의 viewport 설정(표시 범위 설정)
	glViewport(0, 0, w, h);

	// 관점 설정
	gluPerspective(45.0, ratio, 0.1, 100.0);

	glMatrixMode(GL_MODELVIEW);
}

//조명 설정하기
void init_light() {
	GLfloat ambientLight[] = { 0.4f, 0.4f, 0.4f, 1.0f }; //전역 주변반사
	GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat lightPos[] = { 0.0f, 100.0f, 100.0f, 0.0f };
	GLfloat specular[] = { 0.6f, 0.6f, 0.6f, 0.6f };
	GLfloat specref[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glShadeModel(GL_SMOOTH); // Enable Smooth Shading
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE); // 뒷면에 예외
	glFrontFace(GL_CCW); // 반시계방향이 앞
	glEnable(GL_LIGHTING);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glEnable(GL_LIGHT0); // 0번빛을 사용

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
	glMateriali(GL_FRONT, GL_SHININESS, 10);

	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

}

void MyDisplay() {
	//화면과 디스플레이버퍼 제거
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// 하늘 그리기
	glClearColor(0, 0.8, 1, 0);

	glLoadIdentity();

	//관찰자 위치(x, y, z) 및 초점(x + xview, y + yview, z + zview) 및 카메라의 상향 설정
	// gluLookAt(posx,posy,posz,atx,aty,atz, upx,upy,upz); 참고
	gluLookAt(x, y, z, x + xview, y + yview, z + zview, 0.0, y, 0.0); // up

																	  //조명 설정하기
	init_light();

	//땅,도로 그리기
	drawGround();

	// texture enable
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//각 위치에 건물 그리기
	for (int i = 0; i < 4; i++) {
		glPushMatrix(); {
			glTranslatef(places[i][0], places[i][1], places[i][2]);
			drawsGroundBox();
			glBindTexture(GL_TEXTURE_2D, texture[i]);
			drawCube();
		} glPopMatrix();
	}
	glDisable(GL_TEXTURE_2D);
	glutSwapBuffers();
}


// 키보드 입력 콜백함수
void MyKeyboard(unsigned char KeyPressed, int X, int Y) {
	switch (KeyPressed) {
	case '1': //Front에서 바라본 시점
		x = 0, y = 1.0, z = 5;
		xview = 0, yview = 0, zview = -1;
		alpha = 0;

		z = 70; break;
	case '2': //위에서 바라본 시점
		x = 0, y = 0, z = 16.5;
		xview = 0, yview = 0, zview = -1;
		alpha = 0;

		y = 70; yview = -5; break;
	case '3': // 1인칭시점
		x = 0, y = 1.0, z = 35;
		xview = 0, yview = 0, zview = -1;
		alpha = 0;
		break;

	case 'w': //앞으로 전진
		x += xview * moving;
		z += zview * moving;
		break;
	case 's': //뒤로 후진
		x -= xview * moving;
		z -= zview * moving;
		break;
	case 'a': //5도 시계 방향
		alpha -= .02;
		xview = sin(alpha);
		zview = -cos(alpha);
		break;
	case 'd': //5도 반 시계 방향
		alpha += .02;
		xview = sin(alpha);
		zview = -cos(alpha);
		break;
	case 'q': //고개 들기
		yview += .02;
		break;
	case 'e': //고개 내리기
		yview -= .02;
		break;
	case 27: //'esc' 키의 아스키 코드 값
		exit(0); break;
	}
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1200, 600);
	glutCreateWindow("OpenGL Navigation system");
	texture_init();
	glutDisplayFunc(MyDisplay);
	glutReshapeFunc(changeSize);
	glutIdleFunc(MyDisplay);
	glutKeyboardFunc(MyKeyboard);

	glEnable(GL_DEPTH_TEST);
	glutMainLoop();

	return 0;
}
// ConsoleApplication2.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include <iostream>
#include <gl/glut.h>;
#include <gl/gl.h>;
#include <gl/glu.h>;
#include <math.h>

#define PI 3.141592

GLfloat alpha = -60.0f;
GLfloat beta = 0.0f;

// 몸을 그리는 함수
void Draw_Body() {
	glBegin(GL_POLYGON);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(5.0, 5.0, 0.0);
	glVertex3f(5.0, -5.0, 0.0);
	glVertex3f(-5.0, -5.0, 0.0);
	glVertex3f(-5.0, 5.0, 0.0);
	glEnd();
}

// 위쪽 팔을 그리는 함수
void Draw_UpperArm() {
	glBegin(GL_POLYGON);
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 3.0, 0.0);
	glVertex3f(7.0, 3.0, 0.0);
	glVertex3f(7.0, 0.0, 0.0);
	glEnd();
}

// 아래쪽 팔을 그리는 함수
void Draw_LowerArm() {
	glBegin(GL_POLYGON);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 3.0, 0.0);
	glVertex3f(7.0, 3.0, 0.0);
	glVertex3f(7.0, 0.0, 0.0);
	glEnd();
}

// 손을 그리는 함수
void Draw_Hand()
{
	float cx = 0.0f;
	float cy = 0.0f;

	// 오각형 그리기
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(1.0f, 0.92f, 0.79f);	// 살색
	glVertex3f(cx, cy, 0.0f);
	for (float angle = 0.0001f; angle <= 2 * PI + 0.2; angle += (2 * PI / 5))
	{
		cx = cx + sin(angle) * 3;
		cy = cy + cos(angle) * 3;
		glVertex3f(cx, cy, 0);
	}
	glEnd();
}

// 어깨 좌표로 모델좌표계 이동
void GoToShoulderCoordinates() {
	glTranslatef(5.0, 5.0, 0.0);
	glRotatef(alpha, 0.0, 0.0, 1.0);
}

// 팔꿈치 좌표로 모델좌표계 이동
void GoToElbowCoordinates()
{
	glTranslatef(7.0, 0.0, 0.0);
	glRotatef(beta, 0.0, 0.0, 1.0);
}

// 손목 좌표로 모델좌표계 이동
void GoToWristCoordinates()
{
	glTranslatef(7.0f, 0.0f, 0.0f);
}

// 오른팔 그리는 함수
void Right_Arm_Draw()
{
	glPushMatrix();						//전역 좌표계 저장
	GoToShoulderCoordinates();		//어깨 기준 모델 좌표계
	Draw_UpperArm();				//위 팔 그리기
	glPushMatrix();					//어깨 기준 모델 좌표계 저장
	GoToElbowCoordinates();		//팔꿈치 기준 모델 좌표계
	Draw_LowerArm();			//아래팔 그리기
	glPushMatrix();				//팔꿈치 기준 모델 좌표계 저장
	GoToWristCoordinates();	//손목 기준 모델 좌표계
	Draw_Hand();			//손 그리기
	glPopMatrix();				//팔꿈치 좌표계 복원
	glPopMatrix();					//어깨 좌표계 복원
	glPopMatrix();						//몸체 좌표계 복원
}

// 왼팔 그리는 함수
void Left_Arm_Draw()
{
	glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
	glPushMatrix();						//전역 좌표계 저장
	GoToShoulderCoordinates();		//어깨 기준 모델 좌표계
	Draw_UpperArm();				//위 팔 그리기
	glPushMatrix();					//어깨 기준 모델 좌표계 저장
	GoToElbowCoordinates();		//팔꿈치 기준 모델 좌표계
	Draw_LowerArm();			//아래팔 그리기
	glPushMatrix();				//팔꿈치 기준 모델 좌표계 저장
	GoToWristCoordinates();	//손목 기준 모델 좌표계
	Draw_Hand();			//손 그리기
	glPopMatrix();				//팔꿈치 좌표계 복원
	glPopMatrix();					//어깨 좌표계 복원
	glPopMatrix();						//몸체 좌표계 복원
	glRotatef(-180.0f, 0.0f, 1.0f, 0.0f);
}

// 몸과 팔을 그리는 함수
void DrawArms() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();			//전역 좌표계 = 모델 좌표계
	Draw_Body();				//몸체 그리기
	Right_Arm_Draw();
	Left_Arm_Draw();
}

// 머리는 그리는 함수
void DrawHead()
{
	float x, y, x2, y2;
	double radius = 3.0f;

	x = 0.0f, y = 8.0f;
	glColor3f(1.0f, 0.92f, 0.79f);	// 살색

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y);

	for (float angle = 0.01f; angle <= 2 * PI + 0.2; angle += 0.2)
	{
		x2 = x - sin(angle) * radius;
		y2 = y + cos(angle) * radius;
		glVertex2f(x2, y2);
	}

	glEnd();
}

// 키보드 입력 콜백함수
void MyKeyboard(unsigned char KeyPressed, int X, int Y) {
	switch (KeyPressed) {
	case 'q':
		alpha += 5; break;
	case 'a':
		alpha -= 5;	break;
	case 'p':
		beta += 5; break;
	case 'l':
		beta -= 5; break;
	case 27:                //'esc' 키의 아스키 코드 값
		exit(0); break;
	}
}

void MyDisplay() {

	glClear(GL_COLOR_BUFFER_BIT);

	DrawArms();
	DrawHead();

	glutSwapBuffers();
}

void MyTimer(int Value) {
	glutPostRedisplay();
	glutTimerFunc(40, MyTimer, 1); //msec
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 0);
	glutCreateWindow("OpenGL Robot Arm Moving");
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-30.0, 30.0, -30.0, 30.0, 30.0, -30.0);
	glutDisplayFunc(MyDisplay);
	glutKeyboardFunc(MyKeyboard);
	glutTimerFunc(40, MyTimer, 1);
	glutMainLoop();
	return 0;
}
// HW.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include <GL/glut.h>					
#include <GL/gl.h>						
#include <GL/glu.h>

#include <stdlib.h>
#include <math.h>

GLfloat theta = -60; //어깨쪽
GLfloat theta2 = 0; //팔꿈치쪽
GLfloat mexTheta = 60;
GLfloat minTheta = -60;
GLint arm; //1이면 전체 올리기, 2이면 전체 내리기, 3이면 아래만 올리기, 4이면 아래만내리기

//키조작
void MyKeyboard(unsigned char KeyPressed, int X, int Y) {
	switch (KeyPressed) {
	case 'q':
		arm=1; break;
	case 'a':
		arm=2; break;
	case 'p':
		arm = 3; break;
	case 'l':
		arm=4; break;
	case 27:       			//'esc' 키의 아스키 코드 값
		exit(0); break;
	}
}

void arm_move() {
	if (theta <= 60 && arm == 1) {
		theta += 2;
		if (theta > 60) arm = 0;
	}
	else if (theta >= -80 && arm == 2) {
		theta -= 2;
		if (theta < -80) arm = 0;
	}
	else if (theta2 <= 80 && arm == 3) {
		theta2 += 2;
		if (theta2 > 80) arm = 0;
	}
	else if (theta2 >= -60 && arm == 4) {
		theta2 -= 2;
		if (theta2 < -60) arm = 0;
	}
}

//얼굴 그리기
void Draw_Head() {
	float x1, y1, x2, y2;
	float angle;
	double radius = 0.3;

	x1 = 0.0, y1 = 1.1;
	glColor3f(0.984f, 0.808f, 0.694f);

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(x1, y1,0);

	for (angle = 1.0f; angle < 361.0f; angle += 0.2)
	{
		x2 = x1 + sin(angle)*radius;
		y2 = y1 + cos(angle)*radius;
		glVertex3f(x2, y2,0);
	}
	glEnd();
}

//몸체로 가기
void GoToBody() {
	glTranslatef(0.0, 0.3, 0.0);
}

//몸그리기
void Draw_Body() {
	glBegin(GL_POLYGON);
	glColor3f(0.529412f, 0.807843f, 0.980392f);
	glVertex3f(0.5, 0.5, 0.0);
	glVertex3f(0.5, -0.5, 0.0);
	glVertex3f(-0.5, -0.5, 0.0);
	glVertex3f(-0.5, 0.5, 0.0);
	glEnd();
}

//어깨로 가기
void GoToShoulderCoordinates() {
	glTranslatef(0.5, 0.5, 0.0);
	glRotatef(theta, 0.0, 0.0, 1.0);
}

//어깨(윗팔)그리기
void Draw_UpperArm() {
	glBegin(GL_POLYGON);
	glColor3f(0.0f, 0.74902f, 1.0f);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.3, 0.0);
	glVertex3f(0.7, 0.3, 0.0);
	glVertex3f(0.7, 0.0, 0.0);
	glEnd();
}


//팔꿈치(아랫팔) 가기
void GoToElbowCoordinates()
{
	glTranslatef(0.7, 0.0, 0.0);
	glRotatef(theta2, 0.0, 0.0, 1.0);
}

//팔꿈치(아랫팔)그리기
void Draw_LowerArm() {
	glBegin(GL_POLYGON);
	glColor3f(0.27451f, 0.509804f, 0.705882);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.3, 0.0);
	glVertex3f(0.7, 0.3, 0.0);
	glVertex3f(0.7, 0.0, 0.0);
	glEnd();
}

//손목으로 가기
void GoToWristCoordinates() {
	glTranslatef(0.7, 0.0, 0.0);
}

//손그리기
void Draw_Hand() {
	glBegin(GL_POLYGON);
	glColor3f(0.984f, 0.808f, 0.694f);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.3, 0.0);
	glVertex3f(0.2, 0.38, 0.0);
	glVertex3f(0.4, 0.175, 0);
	glVertex3f(0.2, -0.05, 0.0);
	glEnd();
}


void test() {
	glRotatef(180.0f, 0.0f, 1.0f, 0.0f); //x축 기준으로 180도 돌아야 좌우반전
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); //전역 좌표계 = 모델 좌표계
	Draw_Head();//머리 그리기
	GoToBody();//몸체로 이동
	Draw_Body(); //몸체 그리기
	glPushMatrix(); //전역 좌표계 저장
	GoToShoulderCoordinates();	//어깨 기준 모델 좌표계
	Draw_UpperArm(); //위 팔 그리기
}


void right_drawArm() {
	glPushMatrix(); //전역 좌표계 저장
	GoToShoulderCoordinates();	//어깨 기준 모델 좌표계
	Draw_UpperArm(); //위 팔 그리기
	glPushMatrix();//어깨 기준 모델 좌표계 저장
	GoToElbowCoordinates();//팔꿈치 기준 모델 좌표계
	Draw_LowerArm();//아래팔 그리기
	glPushMatrix();//팔꿈치 기준 모델 좌표계 저장
	GoToWristCoordinates();	//손목 기준 모델 좌표계
	Draw_Hand();//손 그리기
	glPopMatrix();//팔꿈치 좌표계 복원
	glPopMatrix();//어깨 좌표계 복원
	glPopMatrix();//몸체 좌표계 복원
}

void left_drawArm() {
	glRotatef(180.0f, 0.0f, 1.0f, 0.0f); //x축 기준으로 180도 돌아야 좌우반전
	glPushMatrix(); //전역 좌표계 저장
	GoToShoulderCoordinates();	//어깨 기준 모델 좌표계
	Draw_UpperArm(); //위 팔 그리기
	glPushMatrix();//어깨 기준 모델 좌표계 저장
	GoToElbowCoordinates();//팔꿈치 기준 모델 좌표계
	Draw_LowerArm();//아래팔 그리기
	glPushMatrix();//팔꿈치 기준 모델 좌표계 저장
	GoToWristCoordinates();	//손목 기준 모델 좌표계
	Draw_Hand();//손 그리기
	glPopMatrix();//팔꿈치 좌표계 복원
	glPopMatrix();//어깨 좌표계 복원
	glPopMatrix();//몸체 좌표계 복원
}

void complete_arm() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); //전역 좌표계 = 모델 좌표계
	Draw_Head();//머리 그리기
	GoToBody();//몸체로 이동
	Draw_Body(); //몸체 그리기
	right_drawArm();
	left_drawArm();
}


//화면 display
void MyDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);
	arm_move();
	complete_arm();
	glEnd();
	glFlush();
}

void MyTimer(int Value) {

	glutPostRedisplay();
	glutTimerFunc(10, MyTimer, 1);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(100, 0);
	glutCreateWindow("OpenGL Drawing 박주영");
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2.5, 2.5, -2.5, 2.5, 1.0, -1.0);
	glutDisplayFunc(MyDisplay);
	glutTimerFunc(40, MyTimer, 1);
	glutKeyboardFunc(MyKeyboard);
	glutMainLoop();
	return 0;
}



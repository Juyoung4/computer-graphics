#include "stdafx.h"
/*
#include "stdafx.h"
#include <iostream>
#include <glut.h>;
#include <gl/gl.h>;
#include <gl/glu.h>;
#include <math.h>

#define PI 3.141592

GLfloat alpha = -60.0f;
GLfloat beta = 0.0f;

//���� �׸���
void drawstreet() {
	glBegin(GL_QUADS); {
	glColor3f(GREEN);
	glVertex3f(2.0, 0.015, 2.0);
	glVertex3f(-2.0, 0.015, 2.0);
	glVertex3f(-2.0, 0.015, -2.0);
	glVertex3f(2.0, 0.015, -2.0);

	glColor3f(WHITE);
	glVertex3f(2.9, 0.01, 2.9);
	glVertex3f(-2.9, 0.01, 2.9);
	glVertex3f(-2.9, 0.01, -2.9);
	glVertex3f(2.9, 0.01, -2.9);
	} glEnd();
}


//�� �׸��� ��
void drawlake() {

}


//cube�׸��� ��
void drawPlace() {

}

// Ű���� �Է� �ݹ��Լ�
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
	case 27:                //'esc' Ű�� �ƽ�Ű �ڵ� ��
	exit(0); break;
	}
}

void MyDisplay() {
	// clear color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//���� �׸���
	drawstreet();
	//��ҵ� �׸���
	drawPlace();
	//����� �׸���
	drawlake();

	glutSwapBuffers();
}

void MyTimer(int Value) {
	glutPostRedisplay();
	glutTimerFunc(40, MyTimer, 1); //msec
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(900, 900);
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

*/
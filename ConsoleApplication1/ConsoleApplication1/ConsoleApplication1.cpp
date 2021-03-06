// ConsoleApplication1.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"


#include <time.h>
#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

#define PIE	3.141592653589793238462643383279 
#define WIDTH 600
#define HEIGHT 300
#define BALL_NUM 2 //수구,적구
#define R 0.2 //반지름의 길이

GLfloat Delta = 0.015;// 원의 이동 속도
GLint count = 0; //수구와 적구가 처음, 부딪혔는지 두번째 부딪혔는지 확인 필요
time_t start = time(NULL); //초반에 일정시간이 흐른뒤 애니메이션 시작 된다

						   //수구,적구 class
class ball {
public:
	float ballColor[3]; //공의 색 표현 -> 수구 노랑, 적구 빨강
	double ballDir[3]; // 공의 위치 
	double ballAngle; //공 움직일 때 각도
	double ballSpd; //공의 속도=0.01->Delta
	double ballFric; //공이 멈추기 위한 값
	int w_count; //수구와 적구가 벽에 부딪히는 횟수[쿠션]

public: //수구,적구 생성자
	ball(float *color, double *dir, double angle) {
		for (int i = 0; i < 3; i++) {
			ballColor[i] = color[i];
			ballDir[i] = dir[i];
		}
		ballAngle = angle;
		ballSpd = Delta;
		ballFric = 0.001;
		w_count = 0;
	}

	//원 그리는 함수
	void DrawCircle()//float cx, float cy, float r, float c1, float c2, float c3
	{
		float x1 = ballDir[0], y1 = ballDir[1], x2, y2;
		float angle;
		double radius = ballDir[2];

		glColor3f(ballColor[0], ballColor[1], ballColor[2]);

		glBegin(GL_TRIANGLE_FAN);
		glVertex3f(x1, y1, 0.0f);

		for (angle = 0.01f; angle <= 2 * 3.14f + 0.2; angle += 0.2)
		{
			x2 = x1 - sin(angle)*radius;
			y2 = y1 + cos(angle)*radius;
			glVertex3f(x2, y2, 0.0f);
		}

		glEnd();
	}

	//원 움직이기
	void move() {
		if (ballSpd > ballFric) {
			ballDir[0] += cos((ballAngle*PIE) / 180.0)*ballSpd;
			ballDir[1] += sin((ballAngle*PIE) / 180.0)*ballSpd;
		}
		else {
			ballSpd = 0;
		}
	}

	//공이 벽에 충돌했는지 체크
	void collisionWall() {
		//다음에 있을 위치 파악
		double move1 = ballDir[0] + cos((ballAngle*PIE) / 180.0)*ballSpd;
		double move2 = ballDir[1] + sin((ballAngle*PIE) / 180.0)*ballSpd;
		if ((move1 < -1.8) || (move1 > 1.8)) {// 화면이 600*600 이기때문에 y의 범위는 -2~2 이다
			ballAngle = 180 - ballAngle; //입사각 반사각 -> 벽에 부딪히면 각도 변형
			if (ballAngle<0)
				ballAngle += 360;
			w_count += 1; //벽에 부딪힌 횟수 필요
		}
		if ((move2 < -0.8) || (move2 > 0.8)) { // 화면이 600*600 이기때문에 y의 범위는 -1~1 이다.
			ballAngle = 360 - ballAngle; //입사각 반사각 -> 벽에 부딪히면 각도 변형
			if (ballAngle < 0)
				ballAngle += 360;
			w_count += 1; //벽에 부딪힌 횟수 필요
		}
	}


	//수구와 적구가 부딪히는지 체크
	bool b_check(const ball& b) const {
		//미리 다음 위치로 이동시켜본 다음에 충돌하는지 확인
		double move1 = (ballDir[0] + cos((ballAngle*PIE) / 180.0)*ballSpd) - (b.ballDir[0] + cos((b.ballAngle*PIE) / 180.0)*b.ballSpd); //(x1-x2)
		double move2 = (ballDir[1] + sin((ballAngle*PIE) / 180.0)*ballSpd) - (b.ballDir[1] + sin((b.ballAngle*PIE) / 180.0)*b.ballSpd); //(y1-y2)

																																		//두 점사이의 거리가 반지름보다 같거나 작으면 두 공은 부딪힘
		if (pow(move1, 2) + pow(move2, 2) <= pow(R * 2, 2)) { //move1^2 + move2^2 <= (2R)^2
			count += 1;
			return true;
		}
		else
			return false;
	}

	//수구와 적구 충돌체크 한 후 계산(분리각 90도 법칙)
	void collision(ball& b) {

		//벡터계산을 위해 공과 공사이의 각도를 찾아줌
		double rect = atan2(this->ballDir[1] - b.ballDir[1], this->ballDir[0] - b.ballDir[0]);
		//atan2는 radian각도이기 때문에 도로 표현해줘야함
		rect *= 180;
		rect /= PIE;


		double bx1, by1, bx2, by2;//벡터용

								  // 탄성 충돌의 속도 계산 공식을 이용하기 위해 먼저 벡터 계싼
		bx1 = cos(((ballAngle - rect)*PIE) / 180.0)*ballSpd;
		by1 = sin(((ballAngle - rect)*PIE) / 180.0)*ballSpd;
		bx2 = cos(((b.ballAngle - rect)*PIE) / 180.0)*b.ballSpd;
		by2 = sin(((b.ballAngle - rect)*PIE) / 180.0)*b.ballSpd;

		//벡터 계산 한뒤, 수구와 적구의 x벡터를 서로 교환해준다음 속도 찾기
		ballSpd = sqrt(pow(bx2, 2) + pow(by1, 2));
		b.ballSpd = sqrt(pow(bx1, 2) + pow(by2, 2));
		//각도 다시 구함
		ballAngle = rect + (atan2(by1, bx2) * 180 / PIE);
		b.ballAngle = rect + (atan2(by2, bx1) * 180 / PIE);
	}

};

ball *ballList[BALL_NUM]; //ballList[0]=수구, ballList[1]=적구


						  //벽에 충돌, 수구 적구 충돌
void check() {
	//수구,적구가 2번째로 부딪히면 stop
	if (count == 2) {
		ballList[0]->ballSpd = 0.0001;
		ballList[1]->ballSpd = 0.0001;
	}



	//수구 = 2쿠션, 적구 = 3쿠션되면 "서서히" 정지

	if (ballList[1]->w_count >= 3) {
		ballList[1]->ballSpd -= 0.00015;
	}
	if (ballList[0]->w_count >= 2) {
		ballList[0]->ballSpd -= 0.00015;
	}


	//공이 벽에 충돌했는지 체크한다음 벽에 충돌했으면 각도 바꿔준다
	ballList[0]->collisionWall();
	ballList[1]->collisionWall();

	//수구 적구 충돌 확인 후, 각각 진행방향 정해주기
	for (int i = 0; i<BALL_NUM - 1; i++) {
		for (int j = i + 1; j<BALL_NUM; j++) {
			if (ballList[i]->b_check(*ballList[j]))
				ballList[i]->collision(*ballList[j]);
		}
	}

}

int find_angle() {
	// 수구,적구의 두접선의 교차점
	float idx = (ballList[0]->ballDir[0] + ballList[1]->ballDir[0]) / 2;
	float idy = (ballList[0]->ballDir[1] + ballList[1]->ballDir[1]) / 2;

	//수구에서 교차점까지의 거리=z, 직각삼각형에서 밑변 길=w
	float z = sqrt(pow(idx - ballList[0]->ballDir[0], 2) + pow(idy - ballList[0]->ballDir[1], 2));
	float w = sqrt(pow(z, 2) - pow(R, 2));
	//이 삼각형에서 사이 각도는
	float angle1 = atan2(R, w);
	angle1 *= 180;
	angle1 /= PIE;

	//수구의 x축에 교차점이 직각으로 선을 내릴때, 그 사이의 각도
	float h = idy - ballList[0]->ballDir[1]; // 높이
	float b = idx - ballList[0]->ballDir[0]; // 밑변
	float angle2 = atan2(h, b);
	angle2 *= 180;
	angle2 /= PIE;

	//이때 수구가 적구로 향하는 각도의 범위는 angle2-angle1 <각도< angle2+angle1에서 랜덤으로 가짐
	srand(time(NULL));

	int b_angle = rand() % int(2 * angle1 + 1) + (angle2 - angle1);

	return b_angle;
}


//수구,적구 초기 설정들
void init() {
	//수구,적구 위치->랜덤(랜덤은 양수만 뽑음)
	//x축은 -1.8 ~ 1.8
	srand(time(NULL));
	float start1 = rand() % 540 + 1;
	start1 /= 300;
	float start2 = rand() % 540 + 1;
	start2 /= 300;
	int a = rand() % 2 + 1;
	int b = rand() % 2 + 1;
	if (a == 1) start1 = -start1; //음수 값도 얻기위해서 1,2 중에 1인 음수, 2는 양수로 사용
	if (b == 1) start2 = -start2;
	float end1 = rand() % 180 + 60;
	float end2 = rand() % 180 + 60;

	double ball1[3] = { start1, -(end1 / 300),R }; //수구 -> 아래 위치(y축이 -0.8 ~ 0) 
	double ball2[3] = { start2,end2 / 300,R }; //적구 -> 위에 위치(y축이 0 ~ 0.8)

											   //수구, 적구 구별을 위한 색
	float cball1[3] = { 1.0f, 1.0f, 0.0f }; // 수구-노란색
	float cball2[3] = { 1.0f, 0.0f, 0.0f }; // 적구-빨간색

											//수구, 적구 만들기
	ballList[0] = new ball(cball1, ball1, 0.0f); //수구
	ballList[1] = new ball(cball2, ball2, 0.0f); // 적구
}


void MyDisplay() {
	//당구대 바닥색
	glClearColor(0.2, 0.58, 0.2, 1.0); //당구대 색 입히기
	glClear(GL_COLOR_BUFFER_BIT); // 전에 있던 수구,적구의 흔적을 지운다

								  //수구,적구 그리기 함수 필요
								  //먼저 수구는 움직임
	ballList[0]->DrawCircle();
	ballList[1]->DrawCircle();

	glutSwapBuffers(); //백버퍼에 그림 다 그리면 전면버퍼와 통째로 교체

}

//glut는 반복적인 작업을 처리할 때 timer event를 제공한다
void MyTimer(int Value) {
	//: start - end 가 5 지나면 애니메이션 시작
	time_t end = time(NULL);
	double result = (double)(end - start);
	if (result > 4) {
		//처음 시작에는 수구만 움직임
		if (count == 0) {
			ballList[0]->move();
		}
		//수구와 적구가 처음으로 부딪히면 적구 움직임
		else {
			ballList[0]->move();
			ballList[1]->move();
		}

		check();
	}
	else {
		ballList[0]->ballAngle = find_angle();
	}

	glutPostRedisplay(); //윈도우를 재생하도록 요구하는 것
	glutTimerFunc(3, MyTimer, 1); //msec, 일반 타이머 함수와 달르게 딱 한번만 호출됨->주기적으로 계속 호출하기 위해 콜백함수에서 계속 호출필요
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WIDTH, HEIGHT); //당구대 size 정의
	glutInitWindowPosition(300, 300); //어디서 당구대 커지는가
	glutCreateWindow("OpenGL 당구 Animation");

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2.0, 2.0, -1.0, 1.0, 1.0, -1.0); //화면의 비율이 2:1비율이기 떄문에 left,right 값을 두배로 늘려주기

	init();
	glutDisplayFunc(MyDisplay); //수구, 적구 그리기

	glutTimerFunc(3, MyTimer, 1);
	glutMainLoop();
	return 0;
}

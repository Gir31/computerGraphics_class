#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

#define MAX_X 800.0
#define MAX_Y 600.0

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

void Mouse(int button, int state, int x, int y);
void draw_rectangle(GLclampf* color, float* xy);
void rectangle_color_rand(GLclampf* color);
void background_color_rand();
void TimerFunction(int value);
void increase_rectangle(float* rectangle);
void decrease_rectangle(float* rectangle);

float conversion_x(float x);
float conversion_y(float y);
int select_rectangle(float x, float y);
int select_quadrant(float x, float y);

bool in_rectangle(float* rectangle, float x, float y);
bool out_rectangle(float* rectangle, float x, float y);

// 배경 초기 색
GLclampf background_color[3] = { 0.0f, 0.0f, 0.0f };
// 사각형 초기 색
GLclampf rectangle_color[4][3] = { {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 1.0f},
								{0.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f} };
// 초기 사각형 위치
float rectangle_xy[4][4] = { {-1.0, 1.0, 0, 0},{0, 1.0, 1.0, 0},
						{-1.0, 0, 0, -1.0},{0,0, 1.0, -1.0}};
// 사분면
float quadrant[4][4] = { {(MAX_X / 2 * -1), MAX_Y / 2, 0, 0},{0, MAX_Y / 2, MAX_X / 2, 0},
					{(MAX_X / 2 * -1), 0, 0, (MAX_Y / 2 * -1)},{0, 0, MAX_X / 2, (MAX_Y / 2 * -1)} };

bool start = FALSE;
int status;
int rec;

void main(int argc, char** argv)
{	
	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); 
	glutInitWindowPosition(100, 100); 
	glutInitWindowSize(MAX_X, MAX_Y);
	glutCreateWindow("GL_Q2 : 사각형 다루기 1"); 

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutMainLoop();
}

GLvoid drawScene() 
{
	glClearColor(background_color[0], background_color[1], background_color[2], 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < 4; i++) {
		draw_rectangle(rectangle_color[i], rectangle_xy[i]);
	}
	
	glutSwapBuffers(); 
}

GLvoid Reshape(int w, int h) 
{
	glViewport(0, 0, w, h);
}

void Mouse(int button, int state, int x, int y)
{
	int mouse_locate = select_rectangle(conversion_x(x), conversion_y(y));

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (mouse_locate < 4) {
			rectangle_color_rand(rectangle_color[mouse_locate]);

		}
		else {
			background_color_rand();
		}

	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		if (mouse_locate < 4) {
			start = TRUE;
			status = 0;
			rec = mouse_locate;
			glutTimerFunc(10, TimerFunction, 1);
		}
		else {
			start = TRUE;
			status = 1;
			rec = mouse_locate - 4;
			glutTimerFunc(10, TimerFunction, 1);
		}
	}

}

void draw_rectangle(GLclampf* color, float* xy) {
	glColor3f(color[0], color[1], color[2]);
	glRectf(xy[0], xy[1], xy[2], xy[3]);
}

void rectangle_color_rand(GLclampf* color) {
	for (int i = 0; i < 3; i++)
		color[i] = ((GLclampf)rand() / (GLclampf)rand());
}

void background_color_rand() {
	for (int i = 0; i < 3; i++)
		background_color[i] = ((GLclampf)rand() / (GLclampf)rand());
}

float conversion_x(float x) { return (x - 400) / (MAX_X / 2); }

float conversion_y(float y) { return (y - 300) / (MAX_Y / -2); }

int select_rectangle(float x, float y) {
	if (in_rectangle(rectangle_xy[0], x, y)) return 0;
	else if (in_rectangle(rectangle_xy[1], x, y)) return 1;
	else if (in_rectangle(rectangle_xy[2], x, y)) return 2;
	else if (in_rectangle(rectangle_xy[3], x, y)) return 3;
	else return select_quadrant(x, y);

}

int select_quadrant(float x, float y){
	if (out_rectangle(quadrant[0], x, y)) return 4;
	else if (out_rectangle(quadrant[1], x, y)) return 5;
	else if (out_rectangle(quadrant[2], x, y)) return 6;
	else if (out_rectangle(quadrant[3], x, y)) return 7;
}

void increase_rectangle(float* rectangle) {
	rectangle[0] -= (float)0.01;
	rectangle[1] += (float)0.01;
	rectangle[2] += (float)0.01;
	rectangle[3] -= (float)0.01;
}

void decrease_rectangle(float* rectangle) {
	if (rectangle[2] - rectangle[0] < 0.3) start = FALSE;
	else {
		rectangle[0] += (float)0.01;
		rectangle[1] -= (float)0.01;
		rectangle[2] -= (float)0.01;
		rectangle[3] += (float)0.01;
	}

}

void TimerFunction(int value){
	if (start) {

		switch (status) {
		case 0:
			decrease_rectangle(rectangle_xy[rec]);
			break;
		case 1:
			increase_rectangle(rectangle_xy[rec]);
			break;
		}

		glutPostRedisplay();
		glutTimerFunc(10, TimerFunction, 1);
	}
}

bool in_rectangle(float* rectangle, float x, float y) {
	if (rectangle[0] <= x && x <= rectangle[2] && rectangle[3] <= y && y <= rectangle[1]) return TRUE;
	else return FALSE;
}

bool out_rectangle(float* rectangle, float x, float y) {
	if ((rectangle[0] <= x && x <= rectangle[2]) && (rectangle[3] <= y && y <= rectangle[1])) return TRUE;
	else return FALSE;
}
#include <iostream>
#include <random>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

#define MAX_X 1200.0
#define MAX_Y 1200.0
#define MAX_WIDTH 0.1
#define MAX_HEIGHT 0.1

GLvoid drawScene();
GLvoid Reshape(int w, int h);

void random_rectangle(float* rectangle);
void random_color(GLclampf* color);

void draw_rectangle(GLclampf* color, float* xy);
void rectangle_color_rand(GLclampf* color);
void new_rectangle(float* rectangle1, float* rectangle2, GLclampf* color);
void choose_point1(float* rectangle1, float* rectangle2);
void choose_point2(float* rectangle1, float* rectangle2);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
void TimerFunction(int value);

float conversion_x(int x);
float conversion_y(int y);

bool overlap_rectangle(float* rectangle1, float* rectangle2);
bool in_rectangle(float* rectangle, float x, float y);

int number;
float width, height;
float rectangle_xy[10][4];
GLclampf rectangle_color[10][3];

bool left_button;
bool time_control = FALSE;

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(MAX_X, MAX_Y);
	glutCreateWindow("GL_Q3 : 사각형 다루기 2");

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";


	for (int i = 0; i < 10; i++) {
		random_rectangle(rectangle_xy[i]);
		random_color(rectangle_color[i]);
	}
	

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutMainLoop();
}

GLvoid drawScene()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < 10; i++)
		draw_rectangle(rectangle_color[i], rectangle_xy[i]);

	glutSwapBuffers(); 
}

GLvoid Reshape(int w, int h) 
{
	glViewport(0, 0, w, h);
}

void random_rectangle(float* rectangle) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(100, 1100);

	rectangle[0] = conversion_x(dis(gen));
	rectangle[1] = conversion_y(dis(gen));
	rectangle[2] = rectangle[0] + MAX_WIDTH;
	rectangle[3] = rectangle[1] - MAX_HEIGHT;
}

void random_color(GLclampf* color) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 1200);

	for (int i = 0; i < 3; i++)
		color[i] = ((GLclampf)dis(gen) / (GLclampf)dis(gen));
}

void draw_rectangle(GLclampf* color, float* xy) {
	glColor3f(color[0], color[1], color[2]);
	glRectf(xy[0], xy[1], xy[2], xy[3]);
}

void rectangle_color_rand(GLclampf* color) {
	for (int i = 0; i < 3; i++)
		color[i] = ((GLclampf)rand() / (GLclampf)rand());
}

void new_rectangle(float* rectangle1, float* rectangle2, GLclampf* color) {
	choose_point1(&rectangle1[0], &rectangle2[0]);
	choose_point2(&rectangle1[1], &rectangle2[1]);
	choose_point2(&rectangle1[2], &rectangle2[2]);
	choose_point1(&rectangle1[3], &rectangle2[3]);

	random_color(color);
}

void choose_point1(float* rectangle1, float* rectangle2) {
	if (*rectangle1 < *rectangle2) {
		*rectangle2 = 5;
	}
	else {
		*rectangle1 = *rectangle2;
		*rectangle2 = 5;
	}
}

void choose_point2(float* rectangle1, float* rectangle2) {
	if (*rectangle1 > *rectangle2) {
		*rectangle2 = 5;
	}
	else {
		*rectangle1 = *rectangle2;
		*rectangle2 = 5;
	}
}

void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		for (int i = 0; i < 10; i++) {
			if (in_rectangle(rectangle_xy[i], conversion_x(x), conversion_y(y))) {
				number = i;
				width = rectangle_xy[number][2] - rectangle_xy[number][0];
				height = rectangle_xy[number][1] - rectangle_xy[number][3];

				left_button = TRUE;
				time_control = TRUE;
				break;
			}
		}
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		left_button = FALSE;
		time_control = FALSE;


		for (int i = 0; i < 10; i++) {
			if (number != i && (overlap_rectangle(rectangle_xy[number], rectangle_xy[i]))) {
				std::cout << "겹쳐짐!!" << std::endl;
				new_rectangle(rectangle_xy[number], rectangle_xy[i], rectangle_color[number]);
			}
		}
	}
}

void Motion(int x, int y) {
	if (left_button == TRUE) {
		rectangle_xy[number][0] = conversion_x(x) - (width / 2);
		rectangle_xy[number][1] = conversion_y(y) + (height / 2);
		rectangle_xy[number][2] = conversion_x(x) + (width / 2);
		rectangle_xy[number][3] = conversion_y(y) - (height / 2);

		glutTimerFunc(1, TimerFunction, 1);
	}
}

void TimerFunction(int value) {
	glutPostRedisplay();
	if(time_control) glutTimerFunc(1, TimerFunction, 1);
}

float conversion_x(int x) { return (x - 600) / (MAX_X / 2); }

float conversion_y(int y) { return (y - 600) / (MAX_Y / -2); }

bool in_rectangle(float* rectangle, float x, float y) {
	if (rectangle[0] <= x && x <= rectangle[2] && rectangle[3] <= y && y <= rectangle[1]) return TRUE;
	else return FALSE;
}

// rectangle1이 움직이고 있는 사각형
bool overlap_rectangle(float* rectangle1, float* rectangle2) {
	bool result = FALSE;

	if (rectangle1[0] <= rectangle2[0] && rectangle1[2] >= rectangle2[0]) {
		if (rectangle1[1] >= rectangle2[1] && rectangle1[3] <= rectangle2[1]) {
			result = TRUE;
		}else if (rectangle1[1] >= rectangle2[3] && rectangle1[3] <= rectangle2[3]) {
			result = TRUE;
		}
	}
	else if (rectangle1[0] <= rectangle2[2] && rectangle1[2] >= rectangle2[2]) {
		if (rectangle1[1] >= rectangle2[1] && rectangle1[3] <= rectangle2[1]) {
			result = TRUE;
		}
		else if (rectangle1[1] >= rectangle2[3] && rectangle1[3] <= rectangle2[3]) {
			result = TRUE;
		}
	}else if (rectangle2[0] <= rectangle1[0] && rectangle2[2] >= rectangle1[0]) {
		if (rectangle2[1] >= rectangle1[1] && rectangle2[3] <= rectangle1[1]) {
			result = TRUE;
		}
		else if (rectangle2[1] >= rectangle1[3] && rectangle2[3] <= rectangle1[3]) {
			result = TRUE;
		}
	}
	else if (rectangle2[0] <= rectangle1[2] && rectangle2[2] >= rectangle1[2]) {
		if (rectangle2[1] >= rectangle1[1] && rectangle2[3] <= rectangle1[1]) {
			result = TRUE;
		}
		else if (rectangle2[1] >= rectangle1[3] && rectangle2[3] <= rectangle1[3]) {
			result = TRUE;
		}
	}

	return result;
}
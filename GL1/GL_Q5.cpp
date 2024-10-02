#include <iostream>
#include <random>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

#define LENGTH 800.0
#define MAX_WIDTH 0.05
#define MAX_HEIGHT 0.05

typedef struct rectangle {
	GLclampf rectangle_color[3];
	float point[4];
	bool show;
}REC;

REC rec[35];
REC eraser;

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);

void random_rectangle(float* rectangle);
void random_color(GLclampf* color);

void draw_rectangle(GLclampf* color, float* xy);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
void TimerFunction(int value);

void move_eraser(float* eraser, int x, int y);

float conversion_x(int x);
float conversion_y(int y);

bool overlap_rectangle(float* rectangle1, float* rectangle2);

float width, height;
float plus = 0;

bool left_button = FALSE;
bool time_control = FALSE;

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(LENGTH, LENGTH);
	glutCreateWindow("GL_Q5 : 사각형 다루기 4");

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	eraser.show = FALSE;

	for (int i = 0; i < 35; i++) {
		random_rectangle(rec[i].point);
		random_color(rec[i].rectangle_color);
		rec[i].show = TRUE;
	}


	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutMainLoop();
}

GLvoid drawScene()
{
	glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < 35; i++)
		if (rec[i].show) draw_rectangle(rec[i].rectangle_color, rec[i].point);

	if (eraser.show) draw_rectangle(eraser.rectangle_color, eraser.point);

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

void random_rectangle(float* rectangle) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(100, LENGTH - 100);

	rectangle[0] = conversion_x(dis(gen));
	rectangle[1] = conversion_y(dis(gen));
	rectangle[2] = rectangle[0] + MAX_WIDTH;
	rectangle[3] = rectangle[1] - MAX_HEIGHT;
}

void random_color(GLclampf* color) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(1, 1200);

	for (int i = 0; i < 3; i++)
		color[i] = ((GLclampf)dis(gen) / (GLclampf)dis(gen));
}

void draw_rectangle(GLclampf* color, float* xy) {
	glColor3f(color[0], color[1], color[2]);
	glRectf(xy[0], xy[1], xy[2], xy[3]);
}

void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		eraser.show = TRUE;
		move_eraser(eraser.point, x, y);
		left_button = TRUE;
		time_control = TRUE;
		glutTimerFunc(0.1, TimerFunction, 1);
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		eraser.show = FALSE;
		left_button = FALSE;
		time_control = FALSE;
		for (int i = 0; i < 3; i++) eraser.rectangle_color[i] = 0.0f;
		plus = 0;
	}
}

void Motion(int x, int y) {
	if (left_button == TRUE) {
		move_eraser(eraser.point, x, y);

		for (int i = 0; i < 35; i++) {
			if (rec[i].show) {
				if (overlap_rectangle(eraser.point, rec[i].point)) {
					rec[i].show = FALSE;
					eraser.rectangle_color[0] = rec[i].rectangle_color[0];
					eraser.rectangle_color[1] = rec[i].rectangle_color[1];
					eraser.rectangle_color[2] = rec[i].rectangle_color[2];
					plus += 0.01;
				}
			}
		}

		glutTimerFunc(0.1, TimerFunction, 1);
	}
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'r': 
		for (int i = 0; i < 35; i++) {
			random_rectangle(rec[i].point);
			random_color(rec[i].rectangle_color);
			rec[i].show = TRUE;
		}
		break;
	case 'q': glutLeaveMainLoop();
	}
	glutPostRedisplay();
}

void TimerFunction(int value) {
	glutPostRedisplay();
	if (time_control) glutTimerFunc(1, TimerFunction, 1);
}

void move_eraser(float* eraser, int x, int y) {
	eraser[0] = conversion_x(x) - MAX_WIDTH - plus;
	eraser[1] = conversion_y(y) + MAX_HEIGHT + plus;
	eraser[2] = conversion_x(x) + MAX_WIDTH + plus;
	eraser[3] = conversion_y(y) - MAX_HEIGHT - plus;
}

float conversion_x(int x) { return ((x - (LENGTH / 2)) / (LENGTH / 2)); }

float conversion_y(int y) { return ((y - (LENGTH / 2)) / -(LENGTH / 2)); }

// rectangle1이 움직이고 있는 사각형
bool overlap_rectangle(float* rectangle1, float* rectangle2) {
	bool result = FALSE;

	if (rectangle1[0] <= rectangle2[0] && rectangle1[2] >= rectangle2[0]) {
		if (rectangle1[1] >= rectangle2[1] && rectangle1[3] <= rectangle2[1]) {
			result = TRUE;
		}
		else if (rectangle1[1] >= rectangle2[3] && rectangle1[3] <= rectangle2[3]) {
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
	}
	else if (rectangle2[0] <= rectangle1[0] && rectangle2[2] >= rectangle1[0]) {
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
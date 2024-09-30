#include <iostream>
#include <random>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

#define MAX_DISPLAY 800.0
#define LENGTH 0.1
#define MAX_LENGTH 1
#define MIN_LENGTH 0.05

typedef struct rectangle {
	GLclampf color[3];
	float locate[4];
	int move_hor, move_ver;
	int size_x, size_y;
}RECTANGLE;

RECTANGLE rectangle[5], origin_rectangle[5];
int number = 0;
int zigzag_count[5] = { 0 };

bool move1_command = FALSE;
bool move2_command = FALSE;
bool color_command = FALSE;
bool size_command = FALSE;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);

void Mouse(int button, int state, int x, int y);
void TimerFunction_move1(int value);
void TimerFunction_move2(int value);
void TimerFunction_size(int value);
void TimerFunction_color(int value);

void create_rectangle(float* locate, int x, int y);
void random_color(GLclampf* color);
void draw_rectangle(RECTANGLE rec);
void move_rectangle_hor(float* locate, int* move);
void move_rectangle_ver(float* locate, int* move);
void size_x(float* locate, int* size);
void size_y(float* locate, int* size);

float conversion_x(int x);
float conversion_y(int y);

bool touch_left(float x1);
bool touch_right(float x2);
bool touch_up(float y1);
bool touch_down(float y2);

void main(int argc, char** argv)
{	
	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); 
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(MAX_DISPLAY, MAX_DISPLAY);
	glutCreateWindow("GL_Q4 : 사각형 다루기 3"); 

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
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMainLoop(); 
}

GLvoid drawScene()
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f); 
	glClear(GL_COLOR_BUFFER_BIT);

	for(int i = 0; i < 5; i++)
		draw_rectangle(rectangle[i]);

	glutSwapBuffers(); 
}

GLvoid Reshape(int w, int h) 
{
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(1, 2);

	switch (key) {
	case '1':
		if (move1_command) move1_command = FALSE;
		else {
			for (int i = 0; i < 5; i++) {
				rectangle[i].move_hor = dis(gen);
				rectangle[i].move_ver = dis(gen);
			}
			move1_command = TRUE;

			glutTimerFunc(10, TimerFunction_move1, 1);
		}
		
		break;
	case '2':
		if (move2_command) move2_command = FALSE;
		else {
			for (int i = 0; i < 5; i++) {
				rectangle[i].move_hor = dis(gen);
				rectangle[i].move_ver = dis(gen);
			}
			move2_command = TRUE;

			glutTimerFunc(10, TimerFunction_move2, 1);
		}
		break;
	case '3':
		if (size_command) size_command = FALSE;
		else {
			for (int i = 0; i < 5; i++) {
				rectangle[i].size_x = dis(gen);
				rectangle[i].size_y = dis(gen);
			}
			size_command = TRUE;
			glutTimerFunc(10, TimerFunction_size, 1);
		}
		break;
	case '4':
		if (color_command) color_command = FALSE;
		else {
			color_command = TRUE;
			glutTimerFunc(1000, TimerFunction_color, 1);
		}
		break;
	case 's':
		move1_command = FALSE;
		move2_command = FALSE;
		size_command = FALSE;
		color_command = FALSE;
		break;
	case 'm':
		move1_command = FALSE;
		move2_command = FALSE;
		size_command = FALSE;
		color_command = FALSE;
		for (int i = 0; i < 5; i++) {
			rectangle[i] = origin_rectangle[i];
		}
		break;
	case 'r':
		number = 0;
		for (int i = 0; i < 5; i++) {
			rectangle[i].locate[0] = 10;
			rectangle[i].locate[1] = 10;
			rectangle[i].locate[2] = 10;
			rectangle[i].locate[3] = 10;
		}
		break;
	case 'q': glutLeaveMainLoop();
	}
	glutPostRedisplay();
}

void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (number < 5) {
			create_rectangle(rectangle[number].locate, x, y);
			random_color(rectangle[number].color);

			origin_rectangle[number] = rectangle[number];
			number++;
		}
	}
}

void TimerFunction_move1(int value) {

	for (int i = 0; i < 5; i++) {
		move_rectangle_hor(rectangle[i].locate, &rectangle[i].move_hor);
		move_rectangle_ver(rectangle[i].locate, &rectangle[i].move_ver);
	}

	glutPostRedisplay();
	if (move1_command) glutTimerFunc(10, TimerFunction_move1, 1);
}

void TimerFunction_move2(int value) {

	for (int i = 0; i < 5; i++) {
		move_rectangle_hor(rectangle[i].locate, &rectangle[i].move_hor);
		move_rectangle_ver(rectangle[i].locate, &rectangle[i].move_ver);

		zigzag_count[i]++;

		if (zigzag_count[i] == 25) {
			zigzag_count[i] = 0;
			switch (rectangle[i].move_hor) {
			case 1: 
				rectangle[i].move_hor = 2;
				break;
			case 2: 
				rectangle[i].move_hor = 1;
				break;
			}
		}
	}

	glutPostRedisplay();
	if (move2_command) glutTimerFunc(10, TimerFunction_move2, 1);
}

void TimerFunction_size(int value) {

	for (int i = 0; i < 5; i++) {
		size_x(rectangle[i].locate, &rectangle[i].size_x);
		size_y(rectangle[i].locate, &rectangle[i].size_y);
	}

	glutPostRedisplay();
	if (size_command) glutTimerFunc(10, TimerFunction_size, 1);
}

void TimerFunction_color(int value) {

	for (int i = 0; i < 5; i++)
		random_color(rectangle[i].color);

	glutPostRedisplay();
	if (color_command) glutTimerFunc(1000, TimerFunction_color, 1);
}

void create_rectangle(float* locate , int x, int y) {
	locate[0] = conversion_x(x) - (LENGTH / 2);
	locate[1] = conversion_y(y) + (LENGTH / 2);
	locate[2] = conversion_x(x) + (LENGTH / 2);
	locate[3] = conversion_y(y) - (LENGTH / 2);
}

void random_color(GLclampf* color) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 1000);

	for (int i = 0; i < 3; i++)
		color[i] = ((GLclampf)dis(gen) / (GLclampf)dis(gen));
}

void draw_rectangle(RECTANGLE rec) {
	glColor3f(rec.color[0], rec.color[1], rec.color[2]);
	glRectf(rec.locate[0], rec.locate[1], rec.locate[2], rec.locate[3]);
}

void move_rectangle_hor(float* locate, int* move) {
	switch (*move) {
	case 1: //left
		locate[0] -= 0.01;
		locate[2] -= 0.01;

		if (touch_left(locate[0])) *move = 2;
		break;
	case 2: //right
		locate[0] += 0.01;
		locate[2] += 0.01;

		if (touch_right(locate[2])) *move = 1;
		break;
	}
}

void move_rectangle_ver(float* locate, int* move) {
	switch (*move) {
	case 1: //down
		locate[1] -= 0.01;
		locate[3] -= 0.01;

		if (touch_down(locate[3])) *move = 2;
		break;
	case 2: //up
		locate[1] += 0.01;
		locate[3] += 0.01;

		if (touch_up(locate[1])) *move = 1;
		break;
	}
}

void size_x(float* locate, int* size) {
	switch (*size) {
	case 1:
		locate[2] += 0.001;
		break;
	case 2:
		locate[2] -= 0.001;
		break;
	}
}

void size_y(float* locate, int* size) {
	switch (*size) {
	case 1:
		locate[3] -= 0.001;
		break;
	case 2:
		locate[3] += 0.001;
		break;
	}
}

float conversion_x(int x) { return (x - (MAX_DISPLAY / 2)) / (MAX_DISPLAY / 2); }

float conversion_y(int y) { return (y - (MAX_DISPLAY / 2)) / (MAX_DISPLAY / -2); }

bool touch_left(float x1) { return (x1 < -1.0); }

bool touch_right(float x2) { return (x2 > 1.0); }

bool touch_up(float y1) { return (y1 > 1.0); }

bool touch_down(float y2) { return (y2 < -1.0); }
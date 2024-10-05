#include <iostream>
#include <random>
#include <gl/glew.h> 
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

#define DISPLAY 600.0

typedef struct rectangle {
	bool show;
	GLfloat point[4];
	GLclampf color[3];
}REC;

typedef struct split_rectangle {
	REC rec[8];
	GLint move;
	GLint move_st;
}SPLIT_RECTANGLE;

REC rec[10];
SPLIT_RECTANGLE s_rec[10];
bool move_switch = FALSE;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

void TimerFunction(int value);
void Mouse(int button, int state, int x, int y);
void reset_rectangle();
void draw_rectangle(GLfloat* point, GLclampf* color);
void random_color(GLclampf* color);
void random_locate(GLfloat* point);

GLfloat random_size();
GLfloat conversion_x(int x);
GLfloat conversion_y(int y);

bool in_rectangle(GLfloat* rectangle, GLfloat x, GLfloat y);
bool disappear_rec(GLfloat y1, GLfloat y2);

void split4(REC* rec, SPLIT_RECTANGLE* split_rec);
void split8(REC* rec, SPLIT_RECTANGLE* split_rec);
void move_dia(SPLIT_RECTANGLE* split_rec);
void move_straight(SPLIT_RECTANGLE* split_rec);
void move_all(SPLIT_RECTANGLE* split_rec);
void every_dir(SPLIT_RECTANGLE* split_rec);
void reduce_size(SPLIT_RECTANGLE* split_rec);

int main(int argc, char** argv) 
{	
	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); 
	glutInitWindowPosition(100, 100);
	glutInitWindowSize((int)DISPLAY, (int)DISPLAY);
	glutCreateWindow("Example1"); 

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) 
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	reset_rectangle();

	glutDisplayFunc(drawScene);
	glutMouseFunc(Mouse);
	glutReshapeFunc(Reshape); 
	glutMainLoop(); 
}

GLvoid drawScene()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < 10; i++) {
		if (rec[i].show) draw_rectangle(rec[i].point, rec[i].color);
	}

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 8; j++) {
			if(s_rec[i].rec[j].show) draw_rectangle(s_rec[i].rec[j].point, s_rec[i].rec[j].color);
		}
	}

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

void TimerFunction(int value) {

	for (int i = 0; i < 10; i++) {
		if (s_rec[i].rec[0].show) {
			switch (s_rec[i].move) {
			case 0:
				move_straight(&s_rec[i]);
				break;
			case 1:
				move_dia(&s_rec[i]);
				break;
			case 2:
				move_all(&s_rec[i]);
				break;
			case 3:
				every_dir(&s_rec[i]);
				break;
			}
		}
	}

	glutPostRedisplay();
	glutTimerFunc(50, TimerFunction, 1);
}

void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		for (int i = 9; i >= 0; i--) {
			if (in_rectangle(rec[i].point, conversion_x(x), conversion_y(y)) && rec[i].show) {
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_int_distribution<int> dis(0, 3);

				rec[i].show = FALSE;
				s_rec[i].move = dis(gen);
				s_rec[i].move_st = dis(gen);

				if (s_rec[i].move < 3) split4(&rec[i], &s_rec[i]);
				else split8(&rec[i], &s_rec[i]);

				if (!move_switch) {
					glutTimerFunc(50, TimerFunction, 1);
					move_switch = TRUE;
				}
				break;
			}
		}
	}
}

void reset_rectangle() {
	for (int i = 0; i < 10; i++) {
		random_color(rec[i].color);
		random_locate((rec[i].point));
		rec[i].show = TRUE;
	}
}

void draw_rectangle(GLfloat* point, GLclampf* color) {
	glColor3f(color[0], color[1], color[2]);
	glRectf(point[0], point[1], point[2], point[3]);
}

void random_color(GLclampf* color) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(1, 1000);

	for (int i = 0; i < 3; i++)
		color[i] = (GLfloat)dis(gen) / (GLfloat)dis(gen);
}

void random_locate(GLfloat* point) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, (int)DISPLAY - 100);

	GLfloat size = random_size();

	point[0] = conversion_x(dis(gen));
	point[1] = conversion_y(dis(gen));
	point[2] = point[0] + size;
	point[3] = point[1] - size;
}

GLfloat random_size() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis1(1, 10);
	std::uniform_int_distribution<int> dis2(20, 50);

	return ((GLfloat)dis1(gen) / (GLfloat)dis2(gen));
}

GLfloat conversion_x(int x) { return (GLfloat)((x - (DISPLAY / 2)) / (DISPLAY / 2)); }

GLfloat conversion_y(int y) { return (GLfloat)((y - (DISPLAY / 2)) / (DISPLAY / -2)); }

bool in_rectangle(GLfloat* rectangle, GLfloat x, GLfloat y) {
	if (rectangle[0] <= x && x <= rectangle[2] && rectangle[3] <= y && y <= rectangle[1]) return TRUE;
	else return FALSE;
}

void split4(REC* rec, SPLIT_RECTANGLE* split_rec) {
// 길이측정
	GLfloat length = rec->point[2] - rec->point[0];

// 1번 도형
	split_rec->rec[0].point[0] = rec->point[0];
	split_rec->rec[0].point[1] = rec->point[1];
	split_rec->rec[0].point[2] = split_rec->rec[0].point[0] + (length / 2);
	split_rec->rec[0].point[3] = split_rec->rec[0].point[1] - (length / 2);

	for(int  i = 0; i< 3; i++)
		split_rec->rec[0].color[i] = rec->color[i];

	split_rec->rec[0].show = TRUE;

// 2번 도형
	split_rec->rec[1].point[0] = split_rec->rec[0].point[2];
	split_rec->rec[1].point[1] = split_rec->rec[0].point[1];
	split_rec->rec[1].point[2] = split_rec->rec[1].point[0] + (length / 2);
	split_rec->rec[1].point[3] = split_rec->rec[1].point[1] - (length / 2);

	for (int i = 0; i < 3; i++)
		split_rec->rec[1].color[i] = rec->color[i];

	split_rec->rec[1].show = TRUE;
// 3번 도형
	split_rec->rec[2].point[0] = split_rec->rec[0].point[0];
	split_rec->rec[2].point[1] = split_rec->rec[0].point[3];
	split_rec->rec[2].point[2] = split_rec->rec[2].point[0] + (length / 2);
	split_rec->rec[2].point[3] = split_rec->rec[2].point[1] - (length / 2);

	for (int i = 0; i < 3; i++)
		split_rec->rec[2].color[i] = rec->color[i];

	split_rec->rec[2].show = TRUE;

// 4번 도형
	split_rec->rec[3].point[0] = split_rec->rec[0].point[2];
	split_rec->rec[3].point[1] = split_rec->rec[0].point[3];
	split_rec->rec[3].point[2] = split_rec->rec[3].point[0] + (length / 2);
	split_rec->rec[3].point[3] = split_rec->rec[3].point[1] - (length / 2);

	for (int i = 0; i < 3; i++)
		split_rec->rec[3].color[i] = rec->color[i];

	split_rec->rec[3].show = TRUE;
}

void split8(REC* rec, SPLIT_RECTANGLE* split_rec) {
	GLfloat width = (rec->point[2] - rec->point[0]) / 2;
	GLfloat height = width / 2;

// 왼쪽 사각형 초기화 0~3번
	for (int i = 0; i < 4; i ++) {
		split_rec->rec[i].point[0] = rec->point[0];
		split_rec->rec[i].point[1] = rec->point[1] - (height * i);
		split_rec->rec[i].point[2] = split_rec->rec[i].point[0] + width;
		split_rec->rec[i].point[3] = split_rec->rec[i].point[1] - height;

		for (int j = 0; j < 3; j++)
			split_rec->rec[i].color[j] = rec->color[j];

		split_rec->rec[i].show = TRUE;
	}

// 오른쪽 사각형 초기화 4~7번
	for (int i = 0; i < 4; i++) {
		split_rec->rec[i + 4].point[0] = rec->point[0] + width;
		split_rec->rec[i + 4].point[1] = rec->point[1] - (height * i);
		split_rec->rec[i + 4].point[2] = split_rec->rec[i + 4].point[0] + width;
		split_rec->rec[i + 4].point[3] = split_rec->rec[i + 4].point[1] - height;

		for (int j = 0; j < 3; j++)
			split_rec->rec[i + 4].color[j] = rec->color[j];

		split_rec->rec[i + 4].show = TRUE;
	}
}

void move_straight(SPLIT_RECTANGLE* split_rec) {
	split_rec->rec[0].point[0] -= 0.01;
	split_rec->rec[0].point[2] -= 0.01;

	split_rec->rec[1].point[1] += 0.01;
	split_rec->rec[1].point[3] += 0.01;

	split_rec->rec[2].point[1] -= 0.01;
	split_rec->rec[2].point[3] -= 0.01;

	split_rec->rec[3].point[0] += 0.01;
	split_rec->rec[3].point[2] += 0.01;

	reduce_size(split_rec);
}

void move_dia(SPLIT_RECTANGLE* split_rec) {
	split_rec->rec[0].point[0] -= 0.01;
	split_rec->rec[0].point[1] += 0.01;
	split_rec->rec[0].point[2] -= 0.01;
	split_rec->rec[0].point[3] += 0.01;

	split_rec->rec[1].point[0] += 0.01;
	split_rec->rec[1].point[1] += 0.01;
	split_rec->rec[1].point[2] += 0.01;
	split_rec->rec[1].point[3] += 0.01;

	split_rec->rec[2].point[0] -= 0.01;
	split_rec->rec[2].point[1] -= 0.01;
	split_rec->rec[2].point[2] -= 0.01;
	split_rec->rec[2].point[3] -= 0.01;

	split_rec->rec[3].point[0] += 0.01;
	split_rec->rec[3].point[1] -= 0.01;
	split_rec->rec[3].point[2] += 0.01;
	split_rec->rec[3].point[3] -= 0.01;

	reduce_size(split_rec);
}

void move_all(SPLIT_RECTANGLE* split_rec) {
	
	switch (split_rec->move_st) {
	case 0:
		for (int i = 0; i < 4; i++) {
			split_rec->rec[i].point[0] -= 0.01;
			split_rec->rec[i].point[2] -= 0.01;
		}
		break;
	case 1:
		for (int i = 0; i < 4; i++) {
			split_rec->rec[i].point[1] += 0.01;
			split_rec->rec[i].point[3] += 0.01;
		}
		break;
	case 2:
		for (int i = 0; i < 4; i++) {
			split_rec->rec[i].point[0] += 0.01;
			split_rec->rec[i].point[2] += 0.01;
		}
		break;
	case 3:
		for (int i = 0; i < 4; i++) {
			split_rec->rec[i].point[1] -= 0.01;
			split_rec->rec[i].point[3] -= 0.01;
		}
		break;
	}

	reduce_size(split_rec);
}

void every_dir(SPLIT_RECTANGLE* split_rec) {
	split_rec->rec[0].point[0] -= 0.01;
	split_rec->rec[0].point[1] += 0.01;
	split_rec->rec[0].point[2] -= 0.01;
	split_rec->rec[0].point[3] += 0.01;

	split_rec->rec[1].point[0] -= 0.01;
	split_rec->rec[1].point[2] -= 0.01;

	split_rec->rec[2].point[1] -= 0.01;
	split_rec->rec[2].point[3] -= 0.01;

	split_rec->rec[3].point[0] -= 0.01;
	split_rec->rec[3].point[1] -= 0.01;
	split_rec->rec[3].point[2] -= 0.01;
	split_rec->rec[3].point[3] -= 0.01;

	split_rec->rec[4].point[0] += 0.01;
	split_rec->rec[4].point[1] += 0.01;
	split_rec->rec[4].point[2] += 0.01;
	split_rec->rec[4].point[3] += 0.01;

	split_rec->rec[5].point[1] += 0.01;
	split_rec->rec[5].point[3] += 0.01;

	split_rec->rec[6].point[0] += 0.01;
	split_rec->rec[6].point[2] += 0.01;

	split_rec->rec[7].point[0] += 0.01;
	split_rec->rec[7].point[1] -= 0.01;
	split_rec->rec[7].point[2] += 0.01;
	split_rec->rec[7].point[3] -= 0.01;

	reduce_size(split_rec);
}

void reduce_size(SPLIT_RECTANGLE* split_rec) {
	int count = 4;
	if (split_rec->move == 3) count = 8;

	for (int i = 0; i < count; i++) {
		split_rec->rec[i].point[0] += 0.0025;
		split_rec->rec[i].point[1] -= 0.0025;
		split_rec->rec[i].point[2] -= 0.0025;
		split_rec->rec[i].point[3] += 0.0025;
	}

	if (disappear_rec(split_rec->rec[0].point[1], split_rec->rec[0].point[3])) {
		split_rec->rec[0].show = FALSE;
		split_rec->rec[1].show = FALSE;
		split_rec->rec[2].show = FALSE;
		split_rec->rec[3].show = FALSE;
		split_rec->rec[4].show = FALSE;
		split_rec->rec[5].show = FALSE;
		split_rec->rec[6].show = FALSE;
		split_rec->rec[7].show = FALSE;
	}
}

bool disappear_rec(GLfloat y1, GLfloat y2) {
	return (y1 < y2);
}
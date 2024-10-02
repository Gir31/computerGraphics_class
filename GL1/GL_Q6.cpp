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
}SPLIT_RECTANGLE;

REC rec[10];
SPLIT_RECTANGLE s_rec[10];

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

void split4(REC* rec, SPLIT_RECTANGLE* split_rec);
void move_dia(GLfloat* point, GLfloat x, GLfloat y);

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
	glutPostRedisplay();
	glutTimerFunc(1, TimerFunction, 1);
}

void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		for (int i = 9; i >= 0; i--) {
			if (in_rectangle(rec[i].point, conversion_x(x), conversion_y(y)) && rec[i].show) {
				rec[i].show = FALSE;
				split4(&rec[i], &s_rec[i]);
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

void move_dia(SPLIT_RECTANGLE* split_rec) {

}
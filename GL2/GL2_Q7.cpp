#include "GL_My_header.h"

#define MAX_FIGURE 10
#define MAX_LENGTH (GLfloat)0.1

typedef struct figure {
	GLfloat shape[3][3];
	GLfloat color[3][3];
	GLint type;
	bool show;
}FIGURE;

char vertex[] = { "vertex1.glsl" };
char fragment[] = { "fragment1.glsl" };

GLuint shaderProgramID;
GLuint vao, vbo[2];
GLint shape_type = POINT_GL;
GLint figure_number = 0;

FIGURE figure[MAX_FIGURE];

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);

void make_shaderProgram();
void Mouse(int button, int state, int x, int y);
void TimerFunction(int value);

void make_point(FIGURE* fig, GLfloat cx, GLfloat cy);
void make_line(FIGURE* fig, GLfloat cx, GLfloat cy);
void make_triangle(FIGURE* fig, GLfloat cx, GLfloat cy);
void make_rectangle(FIGURE* fig, GLfloat cx, GLfloat cy);

void print_rectangle(FIGURE fig);
void move_figure(FIGURE* fig, GLint dir);

int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize((int)WIDTH, (int)HEIGHT);
	glutCreateWindow("Q7. 화면에 기본 도형 그리기");

	glewExperimental = GL_TRUE;
	glewInit();

	make_shaderProgram();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMainLoop();
}

GLvoid drawScene()
{
	glClearColor(0.2, 0.2, 0.2, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgramID);
	
	for (int number = 0; number < MAX_FIGURE; number++) {
		if(figure[number].show)
		{
			InitBuffer(vao, vbo, figure[number].shape, figure[number].color);
			glPointSize(vao);

			switch (figure[number].type) {
			case POINT_GL:
				glPointSize(10);
				glDrawArrays(GL_POINTS, 0, 1);
				break;
			case LINE_GL:
				glLineWidth(10);
				glDrawArrays(GL_LINES, 0, 2);
				break;
			case TRIANGLE_GL:
				glDrawArrays(GL_TRIANGLES, 0, 3);
				break;
			case RECTANGLE_GL:
				glDrawArrays(GL_TRIANGLES, 0, 3);
				print_rectangle(figure[number]);
				break;
			}
			
		}
	}

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h){glViewport(0, 0, w, h);}

void make_shaderProgram()
{
	make_VertexShaders(vertex);
	make_FragmentShaders(fragment);

	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexShader);
	glAttachShader(shaderProgramID, fragmentShader);
	glLinkProgram(shaderProgramID);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glUseProgram(shaderProgramID);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, figure_number);

	switch (key) {
	case 'p': shape_type = POINT_GL; break;
	case 'l': shape_type = LINE_GL; break;
	case 't': shape_type = TRIANGLE_GL; break;
	case 'r': shape_type = RECTANGLE_GL; break;
	case 'w':
		move_figure(&figure[dis(gen)], 0);
		break;
	case 'd':
		move_figure(&figure[dis(gen)], 1);
		break;
	case 's':
		move_figure(&figure[dis(gen)], 2);
		break;
	case 'a':
		move_figure(&figure[dis(gen)], 3);
		break;
	case 'c':
		figure_number = 0;
		for (int i = 0; i < MAX_FIGURE; i++) {
			figure[i] = {0};
		}
		break;
	case 'q': glutLeaveMainLoop();
	}
	glutPostRedisplay();
}

void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (figure_number < 10) {
			GLfloat mouse_x = conversion_x((GLfloat)x);
			GLfloat mouse_y = conversion_y((GLfloat)y);

			switch (shape_type) {
			case POINT_GL:
				make_point(&figure[figure_number], mouse_x, mouse_y);
				break;
			case LINE_GL:
				make_line(&figure[figure_number], mouse_x, mouse_y);
				break;
			case TRIANGLE_GL:
				make_triangle(&figure[figure_number], mouse_x, mouse_y);
				break;
			case RECTANGLE_GL:
				make_rectangle(&figure[figure_number], mouse_x, mouse_y);
				break;
			}

			figure_number++;
		}
	}
}

void TimerFunction(int value) {
	glutPostRedisplay();
	glutTimerFunc(10, TimerFunction, 1);
}

void make_point(FIGURE* fig, GLfloat cx, GLfloat cy) {
	// 첫번째 정점
	fig->shape[0][0] = cx;
	fig->shape[0][1] = cy;
	fig->shape[0][2] = 1.0;

	random_color(fig->color);

	fig->show = TRUE;
	fig->type = POINT_GL;
}

void make_line(FIGURE* fig, GLfloat cx, GLfloat cy) {
	
	// 첫번째 정점
	fig->shape[0][0] = cx - MAX_LENGTH;
	fig->shape[0][1] = cy;
	fig->shape[0][2] = 1.0;
	// 두번째 정점
	fig->shape[1][0] = cx + MAX_LENGTH;
	fig->shape[1][1] = cy;
	fig->shape[1][2] = 1.0;

	random_color(fig->color);

	fig->show = TRUE;
	fig->type = LINE_GL;
}

void make_triangle(FIGURE* fig, GLfloat cx, GLfloat cy) {

	// 첫번째 정점
	fig->shape[0][0] = cx - MAX_LENGTH;
	fig->shape[0][1] = cy - MAX_LENGTH;
	fig->shape[0][2] = 1.0;
	// 두번째 정점
	fig->shape[1][0] = cx + MAX_LENGTH;
	fig->shape[1][1] = cy - MAX_LENGTH;
	fig->shape[1][2] = 1.0;
	// 세번째 정점
	fig->shape[2][0] = cx;
	fig->shape[2][1] = cy + MAX_LENGTH;
	fig->shape[2][2] = 1.0;

	random_color(fig->color);

	fig->show = TRUE;
	fig->type = TRIANGLE_GL;
}

void make_rectangle(FIGURE* fig, GLfloat cx, GLfloat cy) {

	// 첫번째 정점
	fig->shape[0][0] = cx - MAX_LENGTH;
	fig->shape[0][1] = cy - MAX_LENGTH;
	fig->shape[0][2] = 1.0;
	// 두번째 정점
	fig->shape[1][0] = cx + MAX_LENGTH;
	fig->shape[1][1] = cy - MAX_LENGTH;
	fig->shape[1][2] = 1.0;
	// 세번째 정점
	fig->shape[2][0] = cx - MAX_LENGTH;
	fig->shape[2][1] = cy + MAX_LENGTH;
	fig->shape[2][2] = 1.0;

	random_color(fig->color);

	fig->show = TRUE;
	fig->type = RECTANGLE_GL;
}

void print_rectangle(FIGURE fig) {
	fig.shape[0][0] = fig.shape[0][0] + (MAX_LENGTH * 2);
	fig.shape[0][1] = fig.shape[0][1] + (MAX_LENGTH * 2);

	fig.shape[1][0] = fig.shape[0][0] - (MAX_LENGTH * 2);
	fig.shape[1][1] = fig.shape[0][1];

	fig.shape[2][0] = fig.shape[0][0];
	fig.shape[2][1] = fig.shape[0][1] - (MAX_LENGTH * 2);

	InitBuffer(vao, vbo, fig.shape, fig.color);
	glPointSize(vao);

	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void move_figure(FIGURE* fig, GLint dir) {
	switch (dir) {
	case 0:
		for (int count = 0; count < fig->type; count++) {
			fig->shape[count][1] += 0.05;
			if (count == 2) break;
		}
		break;
	case 1:
		for (int count = 0; count < fig->type; count++) {
			fig->shape[count][0] += 0.05;
			if (count == 2) break;
		}
		break;
	case 2:
		for (int count = 0; count < fig->type; count++) {
			fig->shape[count][1] -= 0.05;
			if (count == 2) break;
		}
		break;
	case 3:
		for (int count = 0; count < fig->type; count++) {
			fig->shape[count][0] -= 0.05;
			if (count == 2) break;
		}
		break;
	}
}
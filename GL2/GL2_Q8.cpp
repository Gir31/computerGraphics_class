#include "GL_My_header.h"

typedef struct triangle {
	GLfloat shape[3][3];
	GLfloat color[3][3];
	GLfloat speed;
	int count;
	bool x_inc, y_inc; // FALSE - / TRUE +
	bool show;
	bool type; // FALSE 는 면, TRUE 는 선
}TRIANGLE;


char vertex[] = { "vertex1.glsl" };
char fragment[] = { "fragment1.glsl" };

GLuint shaderProgramID;

GLfloat line_shape[2][3][3] = {
	{{-1.0, 0.0, 1.0}, {1.0, 0.0, 1.0}, {0.0, 0.0, 0.0} },
	{{0.0, 1.0, 1.0}, {0.0, -1.0, 1.0}, {0.0, 0.0, 0.0} }
};
GLfloat line_color[2][3][3] = {
	{{0.0, 0.0, 0.0},{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}},
	{{0.0, 0.0, 0.0},{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}
};

TRIANGLE tri[4][3];
GLuint vao, vbo[2];
GLint tri_count[4];

bool shape_type = FALSE;

GLvoid drawScene();
GLvoid Reshape(int w, int h);
void make_shaderProgram();

GLvoid Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void TimerFunction(int value);

void c_find(TRIANGLE t[][3]);
void make_triangle(TRIANGLE* t, GLfloat cx, GLfloat cy);

void dia_move(TRIANGLE* t);
void zigzag(TRIANGLE* t);
void hori_move(TRIANGLE* t);
void ver_move(TRIANGLE* t);

GLfloat random_length();
GLint in_quadrant(int x, int y);

bool touch_top(GLfloat top);
bool touch_bottom(GLfloat bottom);
bool touch_right(GLfloat right);
bool touch_left(GLfloat left);

int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Q8. 화면에 삼각형 그리기");

	glewExperimental = GL_TRUE;
	glewInit();

	c_find(tri);

	make_shaderProgram();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard);
	glutMainLoop();
}

GLvoid drawScene()
{
	glClearColor(0.5, 0.5, 0.5, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgramID);

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			InitBuffer(vao, vbo, tri[i][j].shape, tri[i][j].color);
			glPointSize(vao);

			if (tri[i][j].type) glPolygonMode(GL_FRONT, GL_LINE);
			else  glPolygonMode(GL_FRONT, GL_FILL);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
	}

	for (int i = 0; i < 2; i++) {
		InitBuffer(vao, vbo, line_shape[i], line_color[i]);
		glPointSize(vao);

		glDrawArrays(GL_LINES, 0, 2);
	}

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

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

void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		int quadrant = in_quadrant(x, y);

		if (tri_count[quadrant] < 2){
			tri_count[quadrant]++;
			make_triangle(&tri[quadrant][tri_count[quadrant]], conversion_x(x), conversion_y(y));
		}

	} else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		int quadrant = in_quadrant(x, y);

		make_triangle(&tri[quadrant][tri_count[quadrant]], conversion_x(x), conversion_y(y));
	}
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case '1':
		glutTimerFunc(10, TimerFunction, 1);
		break;
	case 'a': shape_type = FALSE; break;
	case 'b': shape_type = TRUE; break;
	case 'q': glutLeaveMainLoop();
	}
	glutPostRedisplay();
}

void TimerFunction(int value) {
	glutPostRedisplay();

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			zigzag(&tri[i][j]);//dia_move(&tri[i][j]);
		}
	}

	glutTimerFunc(10, TimerFunction, 1);
}


void c_find(TRIANGLE t[][3]) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(100, HEIGHT / 2 - 100);
	GLfloat x, y;

	// 1사분면 삼각형
	x = conversion_x(dis(gen)) * -1;
	y = conversion_x(dis(gen));

	make_triangle(&t[0][0], x, y);

	// 2사분면 삼각형
	x = conversion_x(dis(gen));
	y = conversion_x(dis(gen));

	make_triangle(&t[1][0], x, y);

	// 3사분면 삼각형
	x = conversion_x(dis(gen));
	y = conversion_x(dis(gen)) * -1;

	make_triangle(&t[2][0], x, y);

	// 4사분면 삼각형
	x = conversion_x(dis(gen)) * -1;
	y = conversion_x(dis(gen)) * -1;

	make_triangle(&t[3][0], x, y);
}

void make_triangle(TRIANGLE* t, GLfloat cx, GLfloat cy) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(1, 4);

	GLfloat length = random_length();

	// 첫번째 정점
	t->shape[0][0] = cx - (length / 2);
	t->shape[0][1] = cy - length;
	t->shape[0][2] = 0.5;
	// 두번째 정점
	t->shape[1][0] = cx + (length / 2);
	t->shape[1][1] = cy - length;
	t->shape[1][2] = 0.5;
	// 세번째 정점
	t->shape[2][0] = cx;
	t->shape[2][1] = cy + length;
	t->shape[2][2] = 0.5;

	random_color(t->color);

	t->show = TRUE;
	t->type = shape_type;
	t->x_inc = dis(gen) % 2;
	t->y_inc = dis(gen) % 2;
	t->speed = (GLfloat)dis(gen) / (GLfloat)100;
}

void dia_move(TRIANGLE* t) {
	if (touch_top(t->shape[2][1])) t->y_inc = FALSE;
	else if (touch_bottom(t->shape[0][1])) t->y_inc = TRUE;
	if (touch_right(t->shape[1][0])) t->x_inc = FALSE;
	else if (touch_left(t->shape[0][0])) t->x_inc = TRUE;

	hori_move(t);

	ver_move(t);
}

void zigzag(TRIANGLE* t) {
	if (touch_top(t->shape[2][1])) t->y_inc = FALSE;
	else if (touch_bottom(t->shape[0][1])) t->y_inc = TRUE;

	if (touch_right(t->shape[1][0])) {
		t->x_inc = FALSE;
		t->count = 0;
		hori_move(t);
	}
	else if (touch_left(t->shape[0][0])) {
		t->x_inc = TRUE;
		t->count = 0;
		hori_move(t);
	}
	
	if (t->count == 8) hori_move(t);
	else { 
		ver_move(t);
		t->count++;
	}
}

void rec_spiral(TRIANGLE* t) {

}

void hori_move(TRIANGLE* t) {
	for (int i = 0; i < 3; i++) {
		if (t->x_inc) t->shape[i][0] += t->speed;
		else t->shape[i][0] -= t->speed;
	}
}

void ver_move(TRIANGLE* t) {
	for (int i = 0; i < 3; i++) {
		if (t->y_inc) t->shape[i][1] += t->speed;
		else t->shape[i][1] -= t->speed;
	}
}

GLfloat random_length() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(1, 5);

	return (GLfloat)dis(gen) / (GLfloat)20;
}

GLint in_quadrant(int x, int y) {
	if (x > WIDTH / 2) {
		if (y < HEIGHT / 2) return 0;
		else return 3;
	}
	else {
		if (y < HEIGHT / 2) return 1;
		else return 2;
	}
}

bool touch_top(GLfloat top) { return (1.0 < top); }
bool touch_bottom(GLfloat bottom) { return (-1.0 > bottom); }
bool touch_right(GLfloat right) { return (1.0 < right); }
bool touch_left(GLfloat left) { return (-1.0 > left); }
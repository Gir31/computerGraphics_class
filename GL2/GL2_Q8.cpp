#include "GL_My_header.h"

typedef struct triangle {
	GLfloat shape[3][3];
	GLfloat color[3][3];
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

bool shape_type = FALSE;

GLvoid drawScene();
GLvoid Reshape(int w, int h);
void make_shaderProgram();

GLvoid Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);

void make_triangle(TRIANGLE* t, GLfloat cx, GLfloat cy);

GLfloat random_length();
GLint in_quadrant(int x, int y);

int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Q8. 화면에 삼각형 그리기");

	glewExperimental = GL_TRUE;
	glewInit();

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

	

	InitBuffer(vao, vbo, tri[0][0].shape, tri[0][0].color);
	glPointSize(vao);
	
	if(tri[0][0].type) glPolygonMode(GL_FRONT, GL_LINE);
	else  glPolygonMode(GL_FRONT, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, 3);

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
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		make_triangle(&tri[in_quadrant(x, y)][0], conversion_x(x), conversion_y(y));
	}
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'a': shape_type = FALSE; break;
	case 'b': shape_type = TRUE; break;
	case 'q': glutLeaveMainLoop();
	}
	glutPostRedisplay();
}

void make_triangle(TRIANGLE* t, GLfloat cx, GLfloat cy) {
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
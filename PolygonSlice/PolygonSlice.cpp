#include "GL_My_header.h"

typedef struct SHAPE {
	GLfloat point[12];
	GLfloat color[12];

	GLfloat cp[3][3]; // 곡선 움직임을 위한 컨트롤 포인트

	struct SHAPE* next; 
}SHAPE;
SHAPE shape;
//SHAPE* start = NULL;

typedef struct RUBBLE {
	GLfloat point[9];
	GLfloat color[9];

	struct RUBBLE* next;
}RUBBLE;

char vertex[] = { "vertex.glsl" };
char fragment[] = { "fragment.glsl" };

GLuint shaderProgramID;
GLuint vao, vbo[2];

GLvoid drawScene();
GLvoid InitBuffer(GLfloat point[], GLfloat colors[]);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);

void make_shaderProgram();
void Mouse(int button, int state, int x, int y);
void TimerFunction(int value);

GLvoid setPath();
GLvoid makeShape();


int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800.0, 800.0);
	glutCreateWindow("Q7. 화면에 기본 도형 그리기");

	glewExperimental = GL_TRUE;
	glewInit();

	make_shaderProgram();
	makeShape();
	setPath();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMainLoop();
}

GLvoid drawScene()
{
	glClearColor(1.f, 1.f, 1.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgramID);

	

	InitBuffer(shape.point, shape.color);
	glDrawArrays(GL_QUADS, 0, 4);

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h) { glViewport(0, 0, w, h); }

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
	switch (key) {
	case 'q': glutLeaveMainLoop();
	}
	glutPostRedisplay();
}

void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {

	}
}

void TimerFunction(int value) {
	glutPostRedisplay();
	glutTimerFunc(10, TimerFunction, 1);
}

GLvoid makeShape() {
	shape.point[0] = -0.5f;
	shape.point[1] = 0.5f;

	shape.point[3] = -0.5f;
	shape.point[4] = -0.5f;

	shape.point[6] = 0.5f;
	shape.point[7] = -0.5f;

	shape.point[9] = 0.5f;
	shape.point[10] = 0.5f;
}

GLvoid setPath() {
	srand((unsigned int)time(NULL));

	shape.cp[0][0] = (rand() % 2 ? -1.f : 1.f);
	shape.cp[0][1] = (GLfloat)(rand() % 100) / 100.f;

	shape.cp[1][0] = shape.cp[0][0] * -1.f * (GLfloat)(rand() % 100) / 100.f;
	shape.cp[1][1] = 1.f;

	shape.cp[2][0] = shape.cp[0][0] * -1;
	shape.cp[2][1] = (GLfloat)(rand() % 100) / 100.f;
}

void InitBuffer(GLfloat point[], GLfloat colors[]) {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(2, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(point) * 6, point, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors) * 6, colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
}
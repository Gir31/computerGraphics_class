#include "GL_My_header.h"

#define LENGTH 0.1f

typedef struct SHAPE {
	GLfloat point[12];
	GLfloat color[12];

	GLfloat clippingPoint[4];

	GLfloat cx, cy;
	GLfloat cp[3][3]; // 곡선 움직임을 위한 컨트롤 포인트

	GLint n; // 도형 0 : 삼각형 1 : 사각형

	GLfloat t; // 매개변수 t
}SHAPE;
SHAPE shape;

typedef struct RUBBLE {
	GLfloat point[9];
	GLfloat color[9];

	GLfloat minX, maxX;

	struct RUBBLE* next;
}RUBBLE;

RUBBLE* start = NULL;

char vertex[] = { "vertex.glsl" };
char fragment[] = { "fragment.glsl" };

GLfloat mousePoint[2][2];

GLuint shaderProgramID;
GLuint vao, vbo[2];

GLfloat basketPoint[] = {
	-0.2f, -0.9f, 0.f,
	-0.2f, -1.f, 0.f,
	0.2f, -1.f, 0.f,
	0.2f, -0.9f, 0.f,
};
GLfloat basketColor[] = {
	0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
};

GLfloat basketMoveValue = 0.01f;

GLvoid drawScene();
GLvoid InitBuffer(GLfloat point[], GLfloat colors[]);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);

void make_shaderProgram();
void Mouse(int button, int state, int x, int y);
void TimerFunction(int value);

GLvoid setColor();
GLvoid setPath();
GLvoid makeShape();
GLvoid moveShape();
GLvoid shapeReset(); 

GLboolean slice();

GLvoid createRubble();
GLvoid drawRubble(); 
GLvoid fallRubble();

GLvoid basketMove();
GLboolean touchBasket(RUBBLE* target);

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
	setPath();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutTimerFunc(10, TimerFunction, 1);
	glutMainLoop();
}

GLvoid drawScene()
{
	glClearColor(1.f, 1.f, 1.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgramID);

	moveShape();

	InitBuffer(shape.point, shape.color);
	glDrawArrays(GL_QUADS, 0, 4);

	InitBuffer(basketPoint, basketColor);
	glDrawArrays(GL_QUADS, 0, 4);

	drawRubble(); 

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
		mousePoint[0][0] = ((GLfloat)x / 400.0f) - 1.f;
		mousePoint[0][1] = (((GLfloat)y / 400.0f) - 1.f) * -1;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		mousePoint[1][0] = ((GLfloat)x / 400.0f) - 1.f;
		mousePoint[1][1] = (((GLfloat)y / 400.0f) - 1.f) * -1;

		if (slice()) {
			createRubble();
			shapeReset();
		}
	}
}

void TimerFunction(int value) {
	glutPostRedisplay();

	shape.t += 0.0025f;
	if (shape.t > 1) {
		shapeReset();
	}
	fallRubble();
	basketMove();

	glutTimerFunc(10, TimerFunction, 1);
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

GLvoid makeShape() {

	if (shape.n) {
		shape.point[0] = shape.cx - LENGTH;
		shape.point[1] = shape.cy + LENGTH;

		shape.point[3] = shape.cx - LENGTH;
		shape.point[4] = shape.cy - LENGTH;

		shape.point[6] = shape.cx + LENGTH;
		shape.point[7] = shape.cy - LENGTH;

		shape.point[9] = shape.cx + LENGTH;
		shape.point[10] = shape.cy + LENGTH;
	}
	else {
		shape.point[0] = shape.cx;
		shape.point[1] = shape.cy + LENGTH;

		shape.point[3] = shape.cx - LENGTH;
		shape.point[4] = shape.cy - LENGTH;

		shape.point[6] = shape.cx + LENGTH;
		shape.point[7] = shape.cy - LENGTH;

		shape.point[9] = shape.cx;
		shape.point[10] = shape.cy + LENGTH;
	}

	shape.clippingPoint[0] = shape.point[3];
	shape.clippingPoint[1] = shape.point[1];
	shape.clippingPoint[2] = shape.point[6];
	shape.clippingPoint[3] = shape.point[7];
}

GLvoid setColor() {
	shape.color[0] = shape.color[3] = shape.color[6] = shape.color[9] = (GLfloat)(rand() % 100) / 100.f;
	shape.color[1] = shape.color[4] = shape.color[7] = shape.color[10] = (GLfloat)(rand() % 100) / 100.f;
	shape.color[2] = shape.color[5] = shape.color[8] = shape.color[11] = (GLfloat)(rand() % 100) / 100.f;
}

GLvoid setPath() {
	srand((unsigned int)time(NULL));

	shape.cp[0][0] = (rand() % 2 ? -1.f : 1.f);
	shape.cp[0][1] = (GLfloat)(rand() % 100) / 100.f - 0.3f;

	shape.cp[1][0] = shape.cp[0][0] * -1.f * (GLfloat)(rand() % 100) / 100.f;
	shape.cp[1][1] = (GLfloat)(rand() % 100) / 100.f - 0.3f;

	shape.cp[2][0] = shape.cp[0][0] * -1;
	shape.cp[2][1] = (GLfloat)(rand() % 100) / 100.f - 0.3f;

	shape.n = rand() % 2;

	setColor();
}

// f(t) = (1-t^2)p0 + 2t(1-t)p1 + t^2p2
GLvoid moveShape() {
	shape.cx = (1 - powf(shape.t, 2)) * shape.cp[0][0] + (2 * shape.t) * (1 - shape.t) * shape.cp[1][0] + powf(shape.t, 2) * shape.cp[2][0];
	shape.cy = (1 - powf(shape.t, 2)) * shape.cp[0][1] + (2 * shape.t) * (1 - shape.t) * shape.cp[1][1] + powf(shape.t, 2) * shape.cp[2][1];

	makeShape();
}

GLvoid shapeReset() {
	shape = {NULL};

	setPath();
}

GLboolean slice() {
	GLfloat minX = shape.clippingPoint[0];
	GLfloat maxY = shape.clippingPoint[1];  
	GLfloat maxX = shape.clippingPoint[2]; 
	GLfloat minY = shape.clippingPoint[3]; 

	GLfloat x, y;
	GLfloat m = (mousePoint[1][1] - mousePoint[0][1]) / (mousePoint[1][0] - mousePoint[0][0]);

	y = mousePoint[0][1] + (m * (minX - mousePoint[0][0]));
	if (y > minY && y < maxY) return TRUE;

	y = mousePoint[0][1] + (m * (maxX - mousePoint[0][0]));
	if (y > minY && y < maxY) return TRUE;

	x = mousePoint[0][0] + ((minY - mousePoint[0][1]) / m);
	if (x > minX && x < maxX) return TRUE;

	x = mousePoint[0][0] + ((maxY - mousePoint[0][1]) / m);
	if (x > minX && x < maxX) return TRUE;



	return FALSE; 
}

GLvoid createRubble() {
	RUBBLE* left = (RUBBLE*)malloc(sizeof(RUBBLE));
	RUBBLE* right = (RUBBLE*)malloc(sizeof(RUBBLE));

	memset(left, 0, sizeof(RUBBLE)); 
	memset(right, 0, sizeof(RUBBLE)); 

	if (shape.n) {
		left->point[0] = shape.point[0] - 0.01f; // min
		left->point[1] = shape.point[1]; // 윗점

		left->point[3] = shape.point[0] - 0.01f; // min
		left->point[4] = shape.point[4];

		left->point[6] = shape.point[6] - 0.01f; // max
		left->point[7] = shape.point[4];

		left->minX = left->point[0]; left->maxX = left->point[6];

		right->point[0] = shape.point[0] + 0.01f; // min
		right->point[1] = shape.point[1]; // 윗점

		right->point[3] = shape.point[6] + 0.01f; // max
		right->point[4] = shape.point[4];

		right->point[6] = shape.point[9] + 0.01f; // max
		right->point[7] = shape.point[10];

		right->minX = right->point[0]; right->maxX = right->point[3];

		left->point[2] = 0.f; left->point[5] = 0.f; left->point[8] = 0.f;
		right->point[2] = 0.f; right->point[5] = 0.f; right->point[8] = 0.f;
	}
	else {
		left->point[0] = shape.point[0] - 0.01f; // max
		left->point[1] = shape.point[1]; // 윗점

		left->point[3] = shape.point[3] - 0.01f; // min
		left->point[4] = shape.point[4];

		left->point[6] = shape.point[0] - 0.01f; // max
		left->point[7] = shape.point[4];

		left->minX = left->point[3]; left->maxX = left->point[0];

		right->point[0] = shape.point[0] + 0.01f; // min
		right->point[1] = shape.point[1]; // 윗점

		right->point[3] = shape.point[0] + 0.01f; // min
		right->point[4] = shape.point[4];

		right->point[6] = shape.point[6] + 0.01f; // max
		right->point[7] = shape.point[4];

		right->minX = right->point[0]; right->maxX = right->point[6];

		left->point[2] = 0.f; left->point[5] = 0.f; left->point[8] = 0.f;
		right->point[2] = 0.f; right->point[5] = 0.f; right->point[8] = 0.f;
	}
	
	for (int i = 0; i < 9; i++) {
		left->color[i] = right->color[i] = shape.color[i];
	}

	right->next = NULL;
	left->next = right;

	RUBBLE* cur = start;
	if (cur == NULL) start = left;
	else {
		while (cur->next != NULL) {
			cur = cur->next;
		}
		cur->next = left;
	}
}

GLvoid drawRubble() {
	RUBBLE* cur = start;

	while (cur != NULL) {

		InitBuffer(cur->point, cur->color);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		cur = cur->next;
	}
}

GLvoid fallRubble() {
	RUBBLE* cur = start;

	while (cur != NULL) {
		if(touchBasket(cur)) {
			cur->point[1] -= 0.01f;
			cur->point[4] -= 0.01f;
			cur->point[7] -= 0.01f;
		}

		if (cur->point[1] < -1.f) {
			if (cur == start) {
				start = start->next;
				free(cur); 
				cur = start;
			}
			else {
				RUBBLE* prev = start;
				while (prev->next != cur) {
					prev = prev->next;
				}
				prev->next = cur->next;
				free(cur);
				cur = prev;
			}
		}
		if(cur != NULL) cur = cur->next;
	}
}

GLvoid basketMove() {
	basketPoint[0] += basketMoveValue;
	basketPoint[3] += basketMoveValue;
	basketPoint[6] += basketMoveValue;
	basketPoint[9] += basketMoveValue;

	if (basketPoint[0] < -1.0f) basketMoveValue = 0.01f;
	else if(basketPoint[6] > 1.0f) basketMoveValue = -0.01f;
}
// min 0 , max 6
GLboolean touchBasket(RUBBLE* target) {
	if (target->point[4] < basketPoint[1] && target->point[4] > basketPoint[4])
		if ((target->minX > basketPoint[0] && target->minX < basketPoint[6]) || (target->maxX > basketPoint[0] && target->maxX < basketPoint[6]))
			return FALSE;

	return TRUE;
}
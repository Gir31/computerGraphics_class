#include "GL_My_header.h"

#define PI 3.141592
#define R 0.001

char vertex[] = { "vertex1.glsl" };
char fragment[] = { "fragment1.glsl" };

typedef struct {
	GLfloat color[3][3];
	GLfloat cx, cy, r; // 중심 좌표, 반지름
	GLint degree, speed;
}CIRCLE;

GLuint shaderProgramID;
GLuint vao, vbo[2];
GLint count = 0;
GLint number = 300;

bool time_control = FALSE;

CIRCLE c;

GLvoid drawScene();
GLvoid Reshape(int w, int h);

GLint sprial_count = 1;

void make_shaderProgram();
void Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void TimerFunction(int value);

void random_color(GLfloat* color[3]);
void circle_spiral(GLfloat cx, GLfloat cy, GLfloat r, GLint degree, GLint speed);

void changeBackgroundColor();

int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Example1");

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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glClearColor(0.5, 0.5, 0.5, 0.01f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgramID);

	for (int i = 0; i < sprial_count; i++) {
		circle_spiral(c.cx + (R *100*i), c.cy + (R * 210 * i), c.r, c.degree, c.speed);
	}

	/*InitBuffer(vao, vbo, c.shape, c.color);
	glPointSize(5);
	glDrawArrays(GL_POINTS, 0, 1);*/
	
	
	

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

void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case '1': case '2': case '3': case '4': case '5':  // 1~5개 스파이럴 그리기
		sprial_count = key - '0';  // 숫자 변환
		break;
	}
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (!time_control) {
			changeBackgroundColor();

			c.cx = conversion_x(x);
			c.cy = conversion_y(y);

			count = 0;
			c.r = 0;
			c.speed = 6;
			random_color(c.color);

			time_control = TRUE;
			glutTimerFunc(10, TimerFunction, 1);
		}
	}
}

void TimerFunction(int value) {
	glutPostRedisplay();
	count++;
	if (count == 300) time_control = FALSE;
	if(time_control) glutTimerFunc(10, TimerFunction, 1);
}

void random_color(GLfloat* color[3]) {
	for (int i = 0; i < 3; i++) {
		color[i][0] = 1.0f;
		color[i][1] = 1.0f;
		color[i][2] = 1.0f;
	}
}

void circle_spiral(GLfloat cx, GLfloat cy, GLfloat r, GLint degree, GLint speed) {
	GLfloat x, y;

	glBegin(GL_LINES);
	for (int i = 0; i < count; i++) {
		if (i < 150) {
			x = cx + (r * cosf(degree * PI / 180));
			y = cy + (r * sinf(degree * PI / 180));
			r += R;
			degree = (degree + speed) % 360;
		}
		else {
			x = cx + (r * cosf(degree * PI / 180));
			y = cy + (r * sinf(degree * PI / 180));
			r -= R;
			degree = (degree - speed) % 360;
		}

		if (i == 150) {
			cx -= r * 2;
			degree -= 180;
		}
		glPointSize(10);
		glVertex2f(x, y);
	}
	glEnd();
}

void changeBackgroundColor()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(100, HEIGHT / 2 - 100);
	
	GLfloat RGB[3];

	random_color_single(RGB);

	glClearColor(RGB[0], RGB[1], RGB[2], 1.0f);
}

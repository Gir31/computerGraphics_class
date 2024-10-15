#include "GL_My_header.h"

#define SMALL_LENGTH_MAX 0.25f
#define BIG_LENGTH_MAX 0.5f
#define PI 3.141592f
#define CENTER 0.5f

typedef struct {
	GLfloat point[5][3];
	GLuint index[3][3];
	GLfloat RGB[5][3]; 
	GLint type; // | 0 : 선 | 1 : 삼각형 | 2 : 사각형 | 3 : 오각형 |
	GLsizei vertex; // 꼭짓점의 수

}SHAPE;

SHAPE shape[4];
SHAPE big_shape;

GLfloat center[4][2] = {
	{CENTER, CENTER}, {-CENTER, CENTER},
	{-CENTER, -CENTER}, {CENTER, -CENTER}
}; // 사분면에서의 중점 좌표(X, Y)
GLint vertex_value[] = { 2, 3, 6, 9 };

char vertex[] = { "vertex1.glsl" };
char fragment[] = { "fragment1.glsl" };

GLuint VAO, VBO, VBO_pos, EBO; 

GLuint shaderProgramID;

bool big_small = FALSE;

GLvoid drawScene();
GLvoid Reshape(int w, int h);

void InitBuffer_EBO(GLfloat vPositionList[][3], GLuint index[][3], GLfloat color[][3], GLuint vSize, GLuint iSize); // 사이즈를 인수로 넣어줘야 함.
void make_shaderProgram();
void Keyboard(unsigned char key, int x, int y);
void TimerFunction(int value);

void reset_shape();

void make_line(SHAPE* s, GLfloat cx, GLfloat cy, GLfloat l);
void line_to_triangle_animation(SHAPE* s, GLfloat cx, GLfloat cy, GLfloat l);

void make_triangle(SHAPE* s, GLfloat cx, GLfloat cy, GLfloat l);
void triangle_to_rectangle_animation(SHAPE* s, GLfloat cx, GLfloat cy, GLfloat l);

void make_rectangle(SHAPE* s, GLfloat cx, GLfloat cy, GLfloat l);
void rectangle_to_pentagon_animation(SHAPE* s, GLfloat cx, GLfloat cy, GLfloat l);

void make_pentagon(SHAPE* s, GLfloat cx, GLfloat cy, GLfloat l);
void pentagon_to_line_animation(SHAPE* s, GLfloat cx, GLfloat cy, GLfloat l);

void random_RGB(GLfloat RGB[][3]);

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

	reset_shape();

	glutDisplayFunc(drawScene);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(10, TimerFunction, 1);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

GLvoid drawScene()
{
	glClearColor(0.8, 0.8, 0.8, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgramID);
	if (big_small) {
		InitBuffer_EBO(big_shape.point, big_shape.index, big_shape.RGB, sizeof(big_shape.point), sizeof(big_shape.index));
		glBindVertexArray(VAO);
		if (big_shape.vertex == 2)
			glDrawElements(GL_LINES, big_shape.vertex, GL_UNSIGNED_INT, 0);
		else
			glDrawElements(GL_TRIANGLES, big_shape.vertex, GL_UNSIGNED_INT, 0);
	}
	else {
		for (int i = 0; i < 4; i++) {
			InitBuffer_EBO(shape[i].point, shape[i].index, shape[i].RGB, sizeof(shape[i].point), sizeof(shape[i].index));
			glBindVertexArray(VAO);
			if (shape[i].vertex == 2)
				glDrawElements(GL_LINES, shape[i].vertex, GL_UNSIGNED_INT, 0);
			else
				glDrawElements(GL_TRIANGLES, shape[i].vertex, GL_UNSIGNED_INT, 0);
		}
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

void InitBuffer_EBO(GLfloat vPositionList[][3], GLuint index[][3], GLfloat color[][3], GLuint vSize, GLuint iSize)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO_pos);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_pos);
	glBufferData(GL_ARRAY_BUFFER, vSize, vPositionList, GL_STATIC_DRAW);
	
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //--- GL_ELEMENT_ARRAY_BUFFER 버퍼 유형으로 바인딩
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, iSize, index, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 15 * sizeof(GLfloat), color, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'l':
		big_small = TRUE;
		make_line(&big_shape, 0.0f, 0.0f, BIG_LENGTH_MAX);
		big_shape.type = 0;
		big_shape.vertex = vertex_value[0];
		random_RGB(big_shape.RGB);
		break;
	case 't':
		big_small = TRUE;
		make_triangle(&big_shape, 0.0f, 0.0f, BIG_LENGTH_MAX);
		big_shape.type = 1;
		big_shape.vertex = vertex_value[1];
		random_RGB(big_shape.RGB);
		break;
	case 'r':
		big_small = TRUE;
		make_rectangle(&big_shape, 0.0f, 0.0f, BIG_LENGTH_MAX);
		big_shape.type = 2;
		big_shape.vertex = vertex_value[2];
		random_RGB(big_shape.RGB);
		break;
	case 'p':
		big_small = TRUE;
		make_pentagon(&big_shape, 0.0f, 0.0f, BIG_LENGTH_MAX);
		big_shape.type = 3;
		big_shape.vertex = vertex_value[3];
		random_RGB(big_shape.RGB);
		break;
	case 'a':
		big_small = FALSE;
		reset_shape();
		break;
	}
}

void TimerFunction(int value) {
	glutPostRedisplay();
	if (big_small) {
		switch (big_shape.type) {
		case 0:
			line_to_triangle_animation(&big_shape, 0.0f, 0.0f, BIG_LENGTH_MAX);
			break;
		case 1:
			triangle_to_rectangle_animation(&big_shape, 0.0f, 0.0f, BIG_LENGTH_MAX);
			break;
		case 2:
			rectangle_to_pentagon_animation(&big_shape, 0.0f, 0.0f, BIG_LENGTH_MAX);
			break;
		case 3:
			pentagon_to_line_animation(&big_shape, 0.0f, 0.0f, BIG_LENGTH_MAX);
			break;
		}
	}
	else {
		for (int i = 0; i < 4; i++) {
			switch (shape[i].type) {
			case 0:
				line_to_triangle_animation(&shape[i], center[i][0], center[i][1], SMALL_LENGTH_MAX);
				break;
			case 1:
				triangle_to_rectangle_animation(&shape[i], center[i][0], center[i][1], SMALL_LENGTH_MAX);
				break;
			case 2:
				rectangle_to_pentagon_animation(&shape[i], center[i][0], center[i][1], SMALL_LENGTH_MAX);
				break;
			case 3:
				pentagon_to_line_animation(&shape[i], center[i][0], center[i][1], SMALL_LENGTH_MAX);
				break;
			}
		}
	}
	glutTimerFunc(10, TimerFunction, 1);
}

void reset_shape() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 3);

	for (int i = 0; i < 4; i++) {
		shape[i].type = dis(gen);

		switch (shape[i].type) {
		case 0:
			make_line(&shape[i], center[i][0], center[i][1], SMALL_LENGTH_MAX);
			break;
		case 1:
			make_triangle(&shape[i], center[i][0], center[i][1], SMALL_LENGTH_MAX);
			break;
		case 2:
			make_rectangle(&shape[i], center[i][0], center[i][1], SMALL_LENGTH_MAX);
			break;
		case 3:
			make_pentagon(&shape[i], center[i][0], center[i][1], SMALL_LENGTH_MAX);
			break;
		}

		shape[i].vertex = vertex_value[shape[i].type];
		random_RGB(shape[i].RGB);
	}
}

void make_line(SHAPE* s, GLfloat cx, GLfloat cy, GLfloat l) {
	// 왼쪽 점
	s->point[0][0] = cx - l;
	s->point[0][1] = cy - l;
	// 오른쪽 점
	s->point[1][0] = cx + l;
	s->point[1][1] = cy - l;
	// 애니메이션을 위한 꼭대기 점 초기화
	s->point[2][0] = cx;
	s->point[2][1] = cy - l;

	// 인덱스 저장
	s->index[0][0] = 0;
	s->index[0][1] = 1;
}

void line_to_triangle_animation(SHAPE* s, GLfloat cx, GLfloat cy, GLfloat l) {
	// 꼭대기 점 (애니메이션 대상)
	s->point[2][1] += 0.005f;

	// 꼭대기 점이 l를 넘었다면 다시 초기화
	if (s->point[2][1] - cy > l)
		s->point[2][1] = cy - l;

	// 인덱스 저장
	s->index[0][2] = 2;

	// 꼭짓점의 수 바꿈
	s->vertex = vertex_value[1];
}

void make_triangle(SHAPE* s, GLfloat cx, GLfloat cy, GLfloat l) {
	// 왼쪽 아래 점
	s->point[0][0] = cx - l;
	s->point[0][1] = cy - l;
	// 오른쪽 아래 점
	s->point[1][0] = cx + l;
	s->point[1][1] = cy - l;
	// 꼭대기 점
	s->point[2][0] = cx;
	s->point[2][1] = cy + l;
	// 애니메이션을 위한 꼭대기 점 초기화
	s->point[3][0] = cx;
	s->point[3][1] = cy + l;

	// 인덱스 저장
	s->index[0][0] = 0;
	s->index[0][1] = 1;
	s->index[0][2] = 2;
}

void triangle_to_rectangle_animation(SHAPE* s, GLfloat cx, GLfloat cy, GLfloat l) {
	// 꼭대기 점 (애니메이션 대상)
	s->point[2][0] -= 0.0025f;
	s->point[3][0] += 0.0025f;

	if (s->point[3][0] - s->point[2][0] > l * 2)
		s->point[2][0] = s->point[3][0] = cx;

	// 인덱스 저장
	s->index[1][0] = 1;
	s->index[1][1] = 3;
	s->index[1][2] = 2;

	// 꼭짓점의 수 바꿈
	s->vertex = vertex_value[2];
}

void make_rectangle(SHAPE* s, GLfloat cx, GLfloat cy, GLfloat l) {
	// 왼쪽 아래 점
	s->point[0][0] = cx - l;
	s->point[0][1] = cy - l;
	// 오른쪽 아래 점
	s->point[1][0] = cx + l;
	s->point[1][1] = cy - l;
	// 왼쪽 위 점
	s->point[2][0] = cx - l;
	s->point[2][1] = cy + l;
	// 오른쪽 위 점
	s->point[3][0] = cx + l;
	s->point[3][1] = cy + l;
	// 애니메이션을 위한 꼭대기 점 초기화
	s->point[4][0] = cx;
	s->point[4][1] = cy + l;

	// --- 인덱스 저장 ---
	// 첫번째 삼각형
	s->index[0][0] = 0;
	s->index[0][1] = 1;
	s->index[0][2] = 2;
	// 두번째 삼각형
	s->index[1][0] = 1;
	s->index[1][1] = 3;
	s->index[1][2] = 2;
}

void rectangle_to_pentagon_animation(SHAPE* s, GLfloat cx, GLfloat cy, GLfloat l) {
	// (애니메이션 대상)
	s->point[3][0] -= 0.00005f;
	s->point[3][1] -= 0.002f;

	s->point[2][0] += 0.00005f;
	s->point[2][1] -= 0.002f;

	s->point[1][0] -= 0.001f;

	s->point[0][0] += 0.001f;

	s->point[4][1] -= 0.00075f;

	if (s->point[1][0] - s->point[0][0] < l) {
		s->point[0][0] = cx - l;

		s->point[1][0] = cx + l;

		s->point[2][0] = cx - l;
		s->point[2][1] = cy + l;

		s->point[3][0] = cx + l;
		s->point[3][1] = cy + l;

		s->point[4][1] = cy + l;
	}

	// 인덱스 저장
	s->index[0][0] = 2;
	s->index[0][1] = 0;
	s->index[0][2] = 4;

	s->index[1][0] = 0;
	s->index[1][1] = 1;
	s->index[1][2] = 4;

	s->index[2][0] = 1;
	s->index[2][1] = 3;
	s->index[2][2] = 4;

	// 꼭짓점의 수 바꿈
	s->vertex = vertex_value[3];
}

void make_pentagon(SHAPE* s, GLfloat cx, GLfloat cy, GLfloat l) {
	// 위쪽 점
	s->point[0][0] = cx;
	s->point[0][1] = cy + l;
	// 오른쪽 윗점
	s->point[1][0] = cx + (l * cosf(18 * PI / 180));
	s->point[1][1] = cy + (l * sinf(18 * PI / 180));
	// 오른쪽 아래점
	s->point[2][0] = cx + (l * cosf(306 * PI / 180));
	s->point[2][1] = cy + (l * sinf(306 * PI / 180));
	// 왼쪽 아래점
	s->point[3][0] = cx + (l * cosf(234 * PI / 180));
	s->point[3][1] = cy + (l * sinf(234 * PI / 180));
	// 왼쪽 윗점
	s->point[4][0] = cx + (l * cosf(162 * PI / 180));
	s->point[4][1] = cy + (l * sinf(162 * PI / 180));

	// --- 인덱스 저장 ---
	// 첫번째 삼각형
	s->index[0][0] = 0;
	s->index[0][1] = 2;
	s->index[0][2] = 1;
	// 두번째 삼각형
	s->index[1][0] = 0;
	s->index[1][1] = 3;
	s->index[1][2] = 2;
	// 세번째 삼각형
	s->index[2][0] = 0;
	s->index[2][1] = 4;
	s->index[2][2] = 3;
}

void pentagon_to_line_animation(SHAPE* s, GLfloat cx, GLfloat cy, GLfloat l) {
	if (s->point[0][1] <= s->point[1][1]) {
		s->point[0][1] = s->point[1][1];
		if (s->point[1][0] <= s->point[0][0]) {
			s->point[1][0] = s->point[0][0];
			s->point[4][0] = s->point[0][0];
			s->point[0][1] -= 0.005f;
			s->point[1][1] -= 0.005f;
			s->point[4][1] -= 0.005f;
		}
		else {
			s->point[1][0] -= 0.005f;
			s->point[4][0] += 0.005f;
		}
	}
	else {
		s->point[0][1] -= 0.005f;
	}

	if (s->point[0][1] < s->point[2][1]) {
		s->point[0][1] = cy + l;
		s->point[1][0] = cx + (l * cosf(18 * PI / 180));
		s->point[1][1] = cy + (l * sinf(18 * PI / 180));
		s->point[4][0] = cx + (l * cosf(162 * PI / 180));
		s->point[4][1] = cy + (l * sinf(162 * PI / 180));
	}
}

void random_RGB(GLfloat RGB[][3]) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(1, 1000);

	for (int i = 0; i < 3; i++)
	{
		GLfloat value = (GLfloat)dis(gen) / (GLfloat)dis(gen);

		RGB[0][i] = RGB[1][i] = RGB[2][i] = RGB[3][i] = RGB[4][i] = value;
	}
}

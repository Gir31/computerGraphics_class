#include "GL_My_header.h"

#define SMALL_LENGTH_MAX 0.05f
#define PI 3.141592f
#define CENTER 0.5f

typedef struct {
	GLfloat point[5][3];
	GLuint index[3][3];
	GLfloat RGB[5][3];
	GLint type; // | 0 : 선 | 1 : 삼각형 | 2 : 사각형 | 3 : 오각형 |
	GLsizei vertex; // 꼭짓점의 수

}SHAPE;

SHAPE shape[15];

GLfloat center[4][2] = {
	{CENTER, CENTER}, {-CENTER, CENTER},
	{-CENTER, -CENTER}, {CENTER, -CENTER}
}; // 사분면에서의 중점 좌표(X, Y)
GLint vertex_value[] = { 2, 3, 6, 9 };

char vertex[] = { "vertex1.glsl" };
char fragment[] = { "fragment1.glsl" };

GLuint VAO, VBO, VBO_pos, EBO;

GLuint shaderProgramID;

GLvoid drawScene();
GLvoid Reshape(int w, int h);

void InitBuffer_EBO(GLfloat vPositionList[][3], GLuint index[][3], GLfloat color[][3], GLuint vSize, GLuint iSize); // 사이즈를 인수로 넣어줘야 함.
void make_shaderProgram();
void TimerFunction(int value);

void reset_shape();

void make_line(SHAPE* s, GLfloat cx, GLfloat cy, GLfloat l);
void make_triangle(SHAPE* s, GLfloat cx, GLfloat cy, GLfloat l);
void make_rectangle(SHAPE* s, GLfloat cx, GLfloat cy, GLfloat l);
void make_pentagon(SHAPE* s, GLfloat cx, GLfloat cy, GLfloat l);
void make_point(SHAPE* s, GLfloat cx, GLfloat cy);

void random_RGB(GLfloat RGB[][3]);

GLfloat random_locate_x();
GLfloat random_locate_y();

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
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

GLvoid drawScene()
{
	glClearColor(0.8, 0.8, 0.8, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgramID);

	for (int i = 0; i < 15; i++) {
		InitBuffer_EBO(shape[i].point, shape[i].index, shape[i].RGB, sizeof(shape[i].point), sizeof(shape[i].index));
		glBindVertexArray(VAO);
		if (shape[i].vertex == 2)
			glDrawElements(GL_LINES, shape[i].vertex, GL_UNSIGNED_INT, 0);
		else
			glDrawElements(GL_TRIANGLES, shape[i].vertex, GL_UNSIGNED_INT, 0);
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

void TimerFunction(int value) {
	glutPostRedisplay();
	
	glutTimerFunc(10, TimerFunction, 1);
}

void reset_shape() {
	for (int i = 0; i < 15; i++) {
		shape[i].type = i % 5;

		switch (shape[i].type) {
		case 0:
			make_line(&shape[i], random_locate_x(), random_locate_y(), SMALL_LENGTH_MAX);
			break;
		case 1:
			make_triangle(&shape[i], random_locate_x(), random_locate_y(), SMALL_LENGTH_MAX);
			break;
		case 2:
			make_rectangle(&shape[i], random_locate_x(), random_locate_y(), SMALL_LENGTH_MAX);
			break;
		case 3:
			make_pentagon(&shape[i], random_locate_x(), random_locate_y(), SMALL_LENGTH_MAX);
			break;
		case 4: 
			make_point(&shape[i], random_locate_x(), random_locate_y());
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

void make_point(SHAPE* s, GLfloat cx, GLfloat cy) {
	// 왼쪽 아래 점
	s->point[0][0] = cx - 0.005;
	s->point[0][1] = cy - 0.005;
	// 오른쪽 아래 점
	s->point[1][0] = cx + 0.005;
	s->point[1][1] = cy - 0.005;
	// 왼쪽 위 점
	s->point[2][0] = cx - 0.005;
	s->point[2][1] = cy + 0.005;
	// 오른쪽 위 점
	s->point[3][0] = cx + 0.005;
	s->point[3][1] = cy + 0.005;
	// 애니메이션을 위한 꼭대기 점 초기화
	s->point[4][0] = cx;
	s->point[4][1] = cy + 0.005;

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

bool in_shape(SHAPE* s, GLfloat mx, GLfloat my) {

	return FALSE;
}

GLfloat random_locate_x() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution <int> dis(0, WIDTH);

	return conversion_x(dis(gen));
}

GLfloat random_locate_y() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution <int> dis(0, HEIGHT);

	return conversion_y(dis(gen));
}
#include "GL_My_header.h"

#define SMALL_LENGTH_MAX 0.1f
#define PI 3.141592f

typedef struct {
	GLfloat point[6][3];
	GLuint index[4][3];
	GLfloat RGB[6][3];
	GLfloat cx, cy;
	GLint type; // | 0 : 점 | 1 : 선 | 2 : 삼각형 | 3 : 사각형 | 4 : 오각형 | 5 : 육각형 |
	GLsizei vertex; // 꼭짓점의 수

	bool x, y; // FALSE - , TRUE +
	bool move;
}SHAPE;

SHAPE shape[15];

GLsizei vertex_value[] = { 2, 3, 6, 9, 12 };

char vertex[] = { "vertex1.glsl" };
char fragment[] = { "fragment1.glsl" };

GLuint VAO, VBO, VBO_pos, EBO;

GLuint shaderProgramID;

bool left_button = FALSE;
bool select_lock = FALSE;
bool time_switch = FALSE;

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLint shape_num;

void InitBuffer_EBO(GLfloat vPositionList[][3], GLuint index[][3], GLfloat color[][3], GLuint vSize, GLuint iSize); // 사이즈를 인수로 넣어줘야 함.
void make_shaderProgram();
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
void TimerFunction(int value);

void reset_shape();

void make_line(SHAPE* s, GLfloat cx, GLfloat cy, GLfloat l);
void make_triangle(SHAPE* s, GLfloat cx, GLfloat cy, GLfloat l);
void make_rectangle(SHAPE* s, GLfloat cx, GLfloat cy, GLfloat l);
void make_pentagon(SHAPE* s, GLfloat cx, GLfloat cy, GLfloat l);
void make_point(SHAPE* s, GLfloat cx, GLfloat cy);
void make_hexagon(SHAPE* s, GLfloat cx, GLfloat cy, GLfloat l);

void random_RGB(GLfloat RGB[][3]);

GLfloat random_locate_x();
GLfloat random_locate_y();

void x_move(SHAPE* s);
void y_move(SHAPE* s);
void dia_move(SHAPE* s);

bool in_shape(SHAPE* s, GLfloat mx, GLfloat my);
bool overlap_rectangle(GLfloat cx1, GLfloat cy1, GLfloat cx2, GLfloat cy2);
bool touch_top(GLfloat cy);
bool touch_bottom(GLfloat cy);
bool touch_right(GLfloat cx);
bool touch_left(GLfloat cx);

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
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
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
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), color, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
}

void TimerFunction(int value) {
	glutPostRedisplay();

	if(shape[shape_num].move)dia_move(&shape[shape_num]);
	
	glutTimerFunc(10, TimerFunction, 1);
}

void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		GLfloat mx = conversion_x(x);
		GLfloat my = conversion_y(y);
		for (int i = 0; i < 15; i++) {
			if (in_shape(&shape[i], mx, my)) {
				select_lock = TRUE;
				shape_num = i;
				break;
			}
		}
		left_button = TRUE;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		for (int i = 0; i < 15; i++) {
			if (i != shape_num && overlap_rectangle(shape[shape_num].cx, shape[shape_num].cy, shape[i].cx, shape[i].cy)) {
				GLint type = (shape[shape_num].type + shape[i].type + 1) % 6; 
				shape[shape_num] = { 0 };
				shape[shape_num].type = type;
				shape[i] = {0}; 
				shape[i].cx = -12.0f;
				shape[i].cy = -12.0f;

				GLfloat mx = conversion_x(x); 
				GLfloat my = conversion_y(y); 
				switch (shape[shape_num].type) {
				case 0:
					make_point(&shape[shape_num], mx, my);
					break;
				case 1:
					make_line(&shape[shape_num], mx, my, SMALL_LENGTH_MAX);
					break;
				case 2:
					make_triangle(&shape[shape_num], mx, my, SMALL_LENGTH_MAX);
					break;
				case 3:
					make_rectangle(&shape[shape_num], mx, my, SMALL_LENGTH_MAX);
					break;
				case 4:
					make_pentagon(&shape[shape_num], mx, my, SMALL_LENGTH_MAX);
					break;
				case 5:
					make_hexagon(&shape[shape_num], mx, my, SMALL_LENGTH_MAX);
					break;
				}
				if (shape[shape_num].type == 0) shape[shape_num].vertex = vertex_value[3];
				else shape[shape_num].vertex = vertex_value[shape[shape_num].type - 1];
				random_RGB(shape[shape_num].RGB);
				shape[shape_num].move = TRUE;
				break;
			}
		}
		
		select_lock = FALSE;
		left_button = FALSE;
	}
}

void Motion(int x, int y) {
	if (left_button == TRUE) {
		GLfloat mx = conversion_x(x);
		GLfloat my = conversion_y(y);
		if (in_shape(&shape[shape_num], mx, my)) {
			switch (shape[shape_num].type) {
			case 0:
				make_point(&shape[shape_num], mx, my);
				break;
			case 1:
				make_line(&shape[shape_num], mx, my, SMALL_LENGTH_MAX);
				break;
			case 2:
				make_triangle(&shape[shape_num], mx, my, SMALL_LENGTH_MAX);
				break;
			case 3:
				make_rectangle(&shape[shape_num], mx, my, SMALL_LENGTH_MAX);
				break;
			case 4:
				make_pentagon(&shape[shape_num], mx, my, SMALL_LENGTH_MAX);
				break;
			case 5:
				make_hexagon(&shape[shape_num], mx, my, SMALL_LENGTH_MAX);
				break;
			}
		}

		if (!time_switch) {
			time_switch = TRUE;
			glutTimerFunc(0.1, TimerFunction, 1);
		}
	}
}

void reset_shape() {
	for (int i = 0; i < 15; i++) {
		shape[i].type = i % 5;
		
		switch (shape[i].type) {
		case 0:
			make_point(&shape[i], random_locate_x(), random_locate_y());
			break;
		case 1:
			make_line(&shape[i], random_locate_x(), random_locate_y(), SMALL_LENGTH_MAX);
			break;
		case 2:
			make_triangle(&shape[i], random_locate_x(), random_locate_y(), SMALL_LENGTH_MAX);
			break;
		case 3:
			make_rectangle(&shape[i], random_locate_x(), random_locate_y(), SMALL_LENGTH_MAX);
			break;
		case 4:
			make_pentagon(&shape[i], random_locate_x(), random_locate_y(), SMALL_LENGTH_MAX);
			break;
		}

		if(shape[i].type == 0) shape[i].vertex = vertex_value[3];
		else shape[i].vertex = vertex_value[shape[i].type - 1];
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

	// 중점 저장
	s->cx = cx;
	s->cy = cy;

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

	// 중점 저장
	s->cx = cx;
	s->cy = cy;

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

	// 중점 저장
	s->cx = cx;
	s->cy = cy;

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

	// 중점 저장
	s->cx = cx;
	s->cy = cy;

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

void make_hexagon(SHAPE* s, GLfloat cx, GLfloat cy, GLfloat l) {
	// 오른쪽 윗점
	s->point[0][0] = cx + (l * cosf(60 * PI / 180));
	s->point[0][1] = cy + (l * sinf(60 * PI / 180));
	// 오른쪽점
	s->point[1][0] = cx + l;
	s->point[1][1] = cy;
	// 오른쪽 아래점
	s->point[2][0] = cx + (l * cosf(300 * PI / 180));
	s->point[2][1] = cy + (l * sinf(300 * PI / 180));
	// 왼쪽 아래점
	s->point[3][0] = cx + (l * cosf(240 * PI / 180));
	s->point[3][1] = cy + (l * sinf(240 * PI / 180));
	// 왼쪽점
	s->point[4][0] = cx - l;
	s->point[4][1] = cy;
	// 왼쪽 윗점
	s->point[5][0] = cx + (l * cosf(120 * PI / 180));
	s->point[5][1] = cy + (l * sinf(120 * PI / 180));

	// 중점 저장
	s->cx = cx;
	s->cy = cy;

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
	s->index[2][0] = 5;
	s->index[2][1] = 3;
	s->index[2][2] = 0;
	// 네번째 삼각형
	s->index[3][0] = 5;
	s->index[3][1] = 4;
	s->index[3][2] = 3;
}

void make_point(SHAPE* s, GLfloat cx, GLfloat cy) {
	// 왼쪽 아래 점
	s->point[0][0] = cx - 0.01;
	s->point[0][1] = cy - 0.01;
	// 오른쪽 아래 점
	s->point[1][0] = cx + 0.01;
	s->point[1][1] = cy - 0.01;
	// 왼쪽 위 점
	s->point[2][0] = cx - 0.01;
	s->point[2][1] = cy + 0.01;
	// 오른쪽 위 점
	s->point[3][0] = cx + 0.01;
	s->point[3][1] = cy + 0.01;
	// 애니메이션을 위한 꼭대기 점 초기화
	s->point[4][0] = cx;
	s->point[4][1] = cy + 0.01;

	// 중점 저장
	s->cx = cx;
	s->cy = cy;

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

		RGB[0][i] = RGB[1][i] = RGB[2][i] = RGB[3][i] = RGB[4][i] = RGB[5][i] = value;
	}
}

bool in_shape(SHAPE* s, GLfloat mx, GLfloat my) {
	if (s->cx + SMALL_LENGTH_MAX >= mx && s->cx - SMALL_LENGTH_MAX <= mx) {
		if (s->cy + SMALL_LENGTH_MAX >= my && s->cy - SMALL_LENGTH_MAX <= my)
			return TRUE;
	}
	return FALSE;
}

bool overlap_rectangle(GLfloat cx1, GLfloat cy1, GLfloat cx2, GLfloat cy2) {
	bool result = FALSE;
	GLfloat rectangle1[4] = {
		(cx1 - SMALL_LENGTH_MAX), (cy1 + SMALL_LENGTH_MAX),
		(cx1 + SMALL_LENGTH_MAX), (cy1 - SMALL_LENGTH_MAX)
	};
	GLfloat rectangle2[4] = {
		(cx2 - SMALL_LENGTH_MAX), (cy2 + SMALL_LENGTH_MAX),
		(cx2 + SMALL_LENGTH_MAX), (cy2 - SMALL_LENGTH_MAX)
	};

	if (rectangle1[0] <= rectangle2[0] && rectangle1[2] >= rectangle2[0]) {
		if (rectangle1[1] >= rectangle2[1] && rectangle1[3] <= rectangle2[1]) {
			result = TRUE;
		}
		else if (rectangle1[1] >= rectangle2[3] && rectangle1[3] <= rectangle2[3]) {
			result = TRUE;
		}
	}
	else if (rectangle1[0] <= rectangle2[2] && rectangle1[2] >= rectangle2[2]) {
		if (rectangle1[1] >= rectangle2[1] && rectangle1[3] <= rectangle2[1]) {
			result = TRUE;
		}
		else if (rectangle1[1] >= rectangle2[3] && rectangle1[3] <= rectangle2[3]) {
			result = TRUE;
		}
	}
	else if (rectangle2[0] <= rectangle1[0] && rectangle2[2] >= rectangle1[0]) {
		if (rectangle2[1] >= rectangle1[1] && rectangle2[3] <= rectangle1[1]) {
			result = TRUE;
		}
		else if (rectangle2[1] >= rectangle1[3] && rectangle2[3] <= rectangle1[3]) {
			result = TRUE;
		}
	}
	else if (rectangle2[0] <= rectangle1[2] && rectangle2[2] >= rectangle1[2]) {
		if (rectangle2[1] >= rectangle1[1] && rectangle2[3] <= rectangle1[1]) {
			result = TRUE;
		}
		else if (rectangle2[1] >= rectangle1[3] && rectangle2[3] <= rectangle1[3]) {
			result = TRUE;
		}
	}

	return result;
}

void x_move(SHAPE* s) { 
	if (s->x) {
		for (int i = 0; i < 6; i++)
			s->point[i][0] += 0.01f;

		s->cx += 0.01f;
	}
	else {
		for (int i = 0; i < 6; i++)
			s->point[i][0] -= 0.01f;
		s->cx -= 0.01f;
	}
}

void y_move(SHAPE* s) { 
	if (s->y) {
		for (int i = 0; i < 6; i++)
			s->point[i][1] += 0.01f;

		s->cy += 0.01f;
	}
	else {
		for (int i = 0; i < 6; i++)
			s->point[i][1] -= 0.01f;

		s->cy -= 0.01f;
	}
}

void dia_move(SHAPE* s) {
	if (touch_top(s->cy)) s->y = FALSE;
	else if (touch_bottom(s->cy)) s->y = TRUE;
	if (touch_right(s->cx)) s->x = FALSE;
	else if (touch_left(s->cx)) s->x = TRUE;

	x_move(s);
	y_move(s);
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


bool touch_top(GLfloat cy) { return (1.0 < cy); }
bool touch_bottom(GLfloat cy) { return (-1.0 > cy); }
bool touch_right(GLfloat cx) { return (1.0 < cx); }
bool touch_left(GLfloat cx) { return (-1.0 > cx); }
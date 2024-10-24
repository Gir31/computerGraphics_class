#include "GL_My_header.h"

#define CUBE_LENGTH 0.5f

typedef struct {
	GLfloat point[8][3];
	GLuint index[12][3];
	GLfloat color[8][3];
	GLsizei vertex;
}CUBE;

typedef struct {
	GLfloat point[4][3];
	GLuint index[4][3];
	GLfloat color[4][3];
	GLsizei vertex;
}TETRAHEDRON;

CUBE cube;
TETRAHEDRON tetrahedron;

GLfloat cube_x_point[4] = {
	-CUBE_LENGTH, -CUBE_LENGTH,
	CUBE_LENGTH, CUBE_LENGTH
};
GLfloat cube_y_point[2] = {
	CUBE_LENGTH, -CUBE_LENGTH
};
GLfloat cube_z_point[4] = {
	-CUBE_LENGTH, CUBE_LENGTH,
	CUBE_LENGTH, -CUBE_LENGTH
};
GLint cube_index[6][6] = {
	{0, 1, 2, 0, 2, 3},
	{1, 5, 6, 1, 6, 2},
	{2, 6, 3, 3, 6, 7},
	{4, 5, 1, 4, 1, 0},
	{4, 0, 3, 4, 3, 7},
	{7, 6, 5, 7, 5, 4}
};

GLfloat line[3][2][3] = {
	{{1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}}, // x축
	{{0.0f, 1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}}, // y축
	{{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}} // z축
};
GLfloat line_color[1][3];
GLuint line_index[1][3] = { 0, 1 };
GLsizei line_vertex = 2;

char vertex[] = { "vertex.glsl" };
char fragment[] = { "fragment.glsl" };
GLuint VAO, VBO, VBO_pos, EBO;

GLboolean shape = FALSE; // FALSE : 정육면체 | TRUE : 정사면체

GLuint shaderProgramID;

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);

void make_shaderProgram();
void InitBuffer_EBO(GLfloat vPositionList[][3], GLuint index[][3], GLfloat color[][3], GLuint vSize, GLuint iSize, GLuint cSize);

void make_cube();
void show_one_side_cube(GLint number);
void show_two_side_cube();

void make_tetrahedron();
void show_one_side_tetrahedron(GLint number);
void show_two_side_tetrahedron();

void red_color(GLfloat* color);
void green_color(GLfloat* color);
void blue_color(GLfloat* color);

int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Example1");

	glewExperimental = GL_TRUE;
	glewInit();

	make_shaderProgram();
	
	glEnable(GL_DEPTH_TEST);

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMainLoop();
}

GLvoid drawScene()
{
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgramID);

	for (int i = 0; i < 3; i++) {
		rotate_shape(10.0f, 10.0f, 0.0f, shaderProgramID);

		InitBuffer_EBO(line[i], line_index, line_color, sizeof(line[i]), sizeof(line_index), sizeof(line_color));
		glBindVertexArray(VAO);

		glDrawElements(GL_LINES, line_vertex, GL_UNSIGNED_INT, 0);
	}

	
	rotate_shape(10.0f, 10.0f, 0.0f, shaderProgramID);

	if (shape) {
		InitBuffer_EBO(tetrahedron.point, tetrahedron.index, tetrahedron.color, sizeof(tetrahedron.point), sizeof(tetrahedron.index), sizeof(tetrahedron.color));
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, tetrahedron.vertex, GL_UNSIGNED_INT, 0);
	}
	else
	{
		InitBuffer_EBO(cube.point, cube.index, cube.color, sizeof(cube.point), sizeof(cube.index), sizeof(cube.color));
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, cube.vertex, GL_UNSIGNED_INT, 0);
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

void InitBuffer_EBO(GLfloat vPositionList[][3], GLuint index[][3], GLfloat color[][3], GLuint vSize, GLuint iSize, GLuint cSize)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO_pos);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_pos);
	glBufferData(GL_ARRAY_BUFFER, vSize, vPositionList, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, iSize, index, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, cSize, color, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case '1' : case '2' : case '3': case '4': case '5': case '6':
		show_one_side_cube(key - '1');
		shape = FALSE;
		break;
	case '7': case '8': case '9': case '0':
		if(key == '0') show_one_side_tetrahedron(key - '0');
		else show_one_side_tetrahedron(key - '6');
		shape = TRUE;
		break;
	case 'c':
		show_two_side_tetrahedron();
		shape = TRUE;
		break;
	case 't':
		show_two_side_cube();
		shape = FALSE;
		break;
	}
	glutPostRedisplay();
}

void make_cube() {
	for (int i = 0; i < 8; i++) {
		cube.point[i][0] = cube_x_point[i % 4];
		cube.point[i][1] = cube_y_point[i / 4];
		cube.point[i][2] = cube_z_point[i % 4];
	}

	for (int i = 0; i < 6; i++) {
		cube.index[i][0] = cube_index[i][0];
		cube.index[i][1] = cube_index[i][1];
		cube.index[i][2] = cube_index[i][2];

		cube.index[i + 6][0] = cube_index[i][3];
		cube.index[i + 6][1] = cube_index[i][4];
		cube.index[i + 6][2] = cube_index[i][5];
	}

	for (int i = 0; i < 8; i++) {

		switch (i % 3) {
		case 0:
			red_color(cube.color[i]);
			break;
		case 1:
			green_color(cube.color[i]);
			break;
		case 2:
			blue_color(cube.color[i]);
			break;
		}
	}

	cube.vertex = 36;
}

void make_tetrahedron() {
	// 꼭짓점 좌표 초기화
	tetrahedron.point[0][0] = 0.0f;
	tetrahedron.point[0][1] = sqrtf(6.0) * 0.2f;
	tetrahedron.point[0][2] = 0.0f;

	tetrahedron.point[1][0] = -0.6f;
	tetrahedron.point[1][1] = sqrtf(6.0) * -0.2f;
	tetrahedron.point[1][2] = sqrtf(3.0) * 0.2f;

	tetrahedron.point[2][0] = 0.6f;
	tetrahedron.point[2][1] = sqrtf(6.0) * -0.2f;
	tetrahedron.point[2][2] = sqrtf(3.0) * 0.2f;

	tetrahedron.point[3][0] = 0.0f;
	tetrahedron.point[3][1] = sqrtf(6.0) * -0.2f;
	tetrahedron.point[3][2] = sqrtf(3.0) * -0.4f;

	// 인덱스 초기화
	tetrahedron.index[0][0] = 0;
	tetrahedron.index[0][1] = 1;
	tetrahedron.index[0][2] = 2;

	tetrahedron.index[1][0] = 0;
	tetrahedron.index[1][1] = 2;
	tetrahedron.index[1][2] = 3;

	tetrahedron.index[2][0] = 0;
	tetrahedron.index[2][1] = 3;
	tetrahedron.index[2][2] = 1;

	tetrahedron.index[3][0] = 3;
	tetrahedron.index[3][1] = 2;
	tetrahedron.index[3][2] = 1;

	for (int i = 0; i < 4; i++) {

		switch (i % 3) {
		case 0:
			red_color(tetrahedron.color[i]);
			break;
		case 1:
			green_color(tetrahedron.color[i]);
			break;
		case 2:
			blue_color(tetrahedron.color[i]);
			break;
		}
	}

	tetrahedron.vertex = 12;
}

void show_one_side_cube(GLint number) {
	make_cube();

	for (int i = 0; i < 6; i++) {
		if (number != i) {
			cube.index[i][0] = 0;
			cube.index[i][1] = 0;
			cube.index[i][2] = 0;

			cube.index[i + 6][0] = 0;
			cube.index[i + 6][1] = 0;
			cube.index[i + 6][2] = 0;
		}
	}
}

void show_two_side_cube() {
	make_cube();

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 5);

	GLint number1 = dis(gen);
	GLint number2;

	while (1) {
		number2 = dis(gen);
		if (number2 != number1) break;
	}

	for (int i = 0; i < 6; i++) {
		if (number1 != i && number2 != i) {
			cube.index[i][0] = 0;
			cube.index[i][1] = 0;
			cube.index[i][2] = 0;

			cube.index[i + 6][0] = 0;
			cube.index[i + 6][1] = 0;
			cube.index[i + 6][2] = 0;
		}
	}
}

void show_one_side_tetrahedron(GLint number) {
	make_tetrahedron();
	for (int i = 0; i < 4; i++) {
		if (number != i) {
			tetrahedron.index[i][0] = 0;
			tetrahedron.index[i][1] = 0;
			tetrahedron.index[i][2] = 0;
		}
	}
}

void show_two_side_tetrahedron() {
	make_tetrahedron();

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 3);

	GLint number1 = dis(gen);
	GLint number2;

	while (1) {
		number2 = dis(gen);
		if (number2 != number1) break;
	}

	for (int i = 0; i < 4; i++) {
		if (number1 != i && number2 != i) {
			tetrahedron.index[i][0] = 0;
			tetrahedron.index[i][1] = 0;
			tetrahedron.index[i][2] = 0;
		}
	}
}

void red_color(GLfloat* color) {
	color[0] = 1.0f;
	color[1] = 0.0f;
	color[2] = 0.0f;
}

void green_color(GLfloat* color) {
	color[0] = 0.0f;
	color[1] = 1.0f;
	color[2] = 0.0f;
}

void blue_color(GLfloat* color) {
	color[0] = 0.0f;
	color[1] = 0.0f;
	color[2] = 1.0f;
}
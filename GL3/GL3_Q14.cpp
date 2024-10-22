#include "GL_My_header.h"

#define CUBE_LENGTH 0.5f

typedef struct {
	GLfloat point[8][3];
	GLuint index[12][3];
	GLfloat color[8][3];
	GLsizei vertex;
}CUBE;

typedef struct {
	GLfloat point[5][3];
	GLuint index[6][3];
	GLfloat color[5][3];
	GLsizei vertex;
}PIRAMID;

CUBE cube;
PIRAMID piramid;

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

GLfloat rotate[3] = { 30.0f, 30.0f, 0.0f }; // x, y, z
GLfloat translate[3] = { 0.0f, 0.0f, 0.0f }; // x, y, z

GLfloat degree_value[2] = { 1.0f, -1.0f };

GLboolean shape = FALSE; // FALSE : 피라미드 | TRUE : 정사면체
GLboolean time_switch = FALSE;
GLboolean xRotate = FALSE;
GLboolean yRotate = FALSE;
GLboolean depth_switch = TRUE;
GLboolean line_switch = FALSE;

GLint dValue;

GLuint shaderProgramID;

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid SpecialKeyboard(int key, int x, int y);
GLvoid TimerFunction(int value);

GLvoid make_shaderProgram();
GLvoid InitBuffer_EBO(GLfloat vPositionList[][3], GLuint index[][3], GLfloat color[][3], GLuint vSize, GLuint iSize, GLuint cSize);

GLvoid make_cube();
GLvoid make_piramid();

GLvoid red_color(GLfloat* color);
GLvoid green_color(GLfloat* color);
GLvoid blue_color(GLfloat* color);

GLvoid trans_shape(GLfloat* degree, GLfloat* trans, GLuint shaderProgramID);

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

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKeyboard);
	glutMainLoop();
}

GLvoid drawScene()
{
	glEnable(GL_DEPTH_TEST);
	if (depth_switch) glDisable(GL_DEPTH_TEST);

	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgramID);

	for (int i = 0; i < 3; i++) {
		rotate_shape(0.0f, 0.0f, 0.0f, shaderProgramID);

		InitBuffer_EBO(line[i], line_index, line_color, sizeof(line[i]), sizeof(line_index), sizeof(line_color));
		glBindVertexArray(VAO);

		glDrawElements(GL_LINES, line_vertex, GL_UNSIGNED_INT, 0);
	}


	trans_shape(rotate, translate, shaderProgramID);

	glPolygonMode(GL_FRONT_AND_BACK, line_switch ? GL_LINE : GL_FILL);

	if (shape) {
		make_cube();

		InitBuffer_EBO(cube.point, cube.index, cube.color, sizeof(cube.point), sizeof(cube.index), sizeof(cube.color));
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, cube.vertex, GL_UNSIGNED_INT, 0);
	}
	else {
		make_piramid();

		InitBuffer_EBO(piramid.point, piramid.index, piramid.color, sizeof(piramid.point), sizeof(piramid.index), sizeof(piramid.color));
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, piramid.vertex, GL_UNSIGNED_INT, 0);
	}

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

GLvoid make_shaderProgram()
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

GLvoid InitBuffer_EBO(GLfloat vPositionList[][3], GLuint index[][3], GLfloat color[][3], GLuint vSize, GLuint iSize, GLuint cSize)
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
	case 'c':
		shape = TRUE;
		break;
	case 'p':
		shape = FALSE;
		break;
	case 'h':
		if (depth_switch) 
			depth_switch = FALSE;
		else 
			depth_switch = TRUE;
		break;
	case 's':
		xRotate = FALSE;
		yRotate = FALSE;
		time_switch = FALSE;

		rotate[0] = 30.0f;
		rotate[1] = 30.0f;
		rotate[2] = 0.0f;;

		translate[0] = 0.0f;
		translate[1] = 0.0f;
		translate[2] = 0.0f;
		break;
	case 'w':
		line_switch = TRUE;
		break;
	case 'W':
		line_switch = FALSE;
		break;
	case 'x':
		dValue = 0;
		if (!xRotate) {
			xRotate = TRUE;
			yRotate = FALSE;
			if (!time_switch) {
				glutTimerFunc(10, TimerFunction, 1);
				time_switch = TRUE;
			}
		}
		break;
	case 'X':
		dValue = 1;
		if (!xRotate) {
			xRotate = TRUE;
			yRotate = FALSE;
			if (!time_switch) {
				glutTimerFunc(10, TimerFunction, 1);
				time_switch = TRUE;
			}
		}
		break;
	case 'y':
		dValue = 0;
		if (!yRotate) {
			xRotate = FALSE;
			yRotate = TRUE;
			if (!time_switch) {
				glutTimerFunc(10, TimerFunction, 1);
				time_switch = TRUE;
			}
		}
		break;
	case 'Y':
		dValue = 1;
		if (!yRotate) {
			xRotate = FALSE;
			yRotate = TRUE;
			if (!time_switch) {
				glutTimerFunc(10, TimerFunction, 1);
				time_switch = TRUE;
			}
		}
		break;
	}
	glutPostRedisplay();
}

GLvoid SpecialKeyboard(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		translate[0] -= 0.01f;
		break;
	case GLUT_KEY_RIGHT:
		translate[0] += 0.01f;
		break;
	case GLUT_KEY_UP:
		translate[1] += 0.01f;
		break;
	case GLUT_KEY_DOWN:
		translate[1] -= 0.01f;
		break;
	}
	glutPostRedisplay();
}

GLvoid TimerFunction(int value) {
	glutPostRedisplay();
	if (xRotate) {
		rotate[0] += degree_value[dValue];
		glutTimerFunc(10, TimerFunction, 1);
	}
	else if(yRotate) {
		rotate[1] += degree_value[dValue];
		glutTimerFunc(10, TimerFunction, 1);
	}
}

GLvoid make_cube() {
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

GLvoid make_piramid() {
	// 포인트 초기화
	GLfloat height = sqrtf(6.0f) * 0.25f;

	piramid.point[0][0] = piramid.point[0][2] = 0.0f;
	piramid.point[0][1] = height;

	piramid.point[1][0] = piramid.point[2][0] = -0.5f;
	piramid.point[3][0] = piramid.point[4][0] = 0.5f;

	piramid.point[1][1] = piramid.point[2][1] = piramid.point[3][1] = piramid.point[4][1] = height * -1.0f;

	piramid.point[1][2] = piramid.point[4][2] = -0.5f;
	piramid.point[2][2] = piramid.point[3][2] = 0.5f;

	// 인덱스 초기화
	piramid.index[0][0] = 0;
	piramid.index[0][1] = 2;
	piramid.index[0][2] = 3;

	piramid.index[1][0] = 0;
	piramid.index[1][1] = 3;
	piramid.index[1][2] = 4;

	piramid.index[2][0] = 0;
	piramid.index[2][1] = 4;
	piramid.index[2][2] = 1;

	piramid.index[3][0] = 0;
	piramid.index[3][1] = 1;
	piramid.index[3][2] = 2;

	piramid.index[4][0] = 1;
	piramid.index[4][1] = 4;
	piramid.index[4][2] = 2;

	piramid.index[5][0] = 2;
	piramid.index[5][1] = 4;
	piramid.index[5][2] = 3;

	for (int i = 0; i < 5; i++) {

		switch (i % 3) {
		case 0:
			red_color(piramid.color[i]);
			break;
		case 1:
			green_color(piramid.color[i]);
			break;
		case 2:
			blue_color(piramid.color[i]);
			break;
		}
	}

	piramid.vertex = 18;
}

GLvoid red_color(GLfloat* color) {
	color[0] = 1.0f;
	color[1] = 0.0f;
	color[2] = 0.0f;
}

GLvoid green_color(GLfloat* color) {
	color[0] = 0.0f;
	color[1] = 1.0f;
	color[2] = 0.0f;
}

GLvoid blue_color(GLfloat* color) {
	color[0] = 0.0f;
	color[1] = 0.0f;
	color[2] = 1.0f;
}

GLvoid trans_shape(GLfloat* degree, GLfloat* trans, GLuint shaderProgramID) {
	glm::mat4 rotateMatrix_x(1.0f);
	rotateMatrix_x = glm::rotate(rotateMatrix_x, glm::radians(degree[0]), glm::vec3(1.0f, 0.0f, 0.0f)); //--- X축 회전

	glm::mat4 rotateMatrix_y(1.0f);
	rotateMatrix_y = glm::rotate(rotateMatrix_y, glm::radians(degree[1]), glm::vec3(0.0f, 1.0f, 0.0f)); //--- Y축 회전

	glm::mat4 rotateMatrix_z(1.0f);
	rotateMatrix_z = glm::rotate(rotateMatrix_z, glm::radians(degree[2]), glm::vec3(0.0f, 0.0f, 1.0f)); //--- Z축 회전

	glm::mat4 rotateMatrix(1.0f);
	rotateMatrix = rotateMatrix_z * rotateMatrix_y * rotateMatrix_x;

	glm::mat4 transMatrix(1.0f);
	transMatrix = glm::translate(transMatrix, glm::vec3(trans[0], trans[1], trans[2])); //--- 이동

	glm::mat4 Matrix(1.0f);
	Matrix = transMatrix * rotateMatrix;

	unsigned int transformLocation = glGetUniformLocation(shaderProgramID, "transform");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(Matrix));
}
#include "GL_My_header.h"

#define LENGTH 0.1f

typedef struct {
	GLfloat point[8][3];
	GLuint index[12][3];
	GLfloat color[8][3];
	GLsizei vertex;
}CUBE;

GLfloat line[3][2][3] = {
	{{1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}}, // x축
	{{0.0f, 1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}}, // y축
	{{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}} // z축
};
GLfloat line_color[1][3];
GLuint line_index[1][3] = { 0, 1 };
GLsizei line_vertex = 2;

CUBE cube;
GLUquadric* qobj;

GLfloat cube_x_point[4] = {
	-LENGTH, -LENGTH,
	LENGTH, LENGTH
};
GLfloat cube_y_point[2] = {
	LENGTH, -LENGTH
};
GLfloat cube_z_point[4] = {
	-LENGTH, LENGTH,
	LENGTH, -LENGTH
};
GLint cube_index[6][6] = {
	{0, 1, 2, 0, 2, 3},
	{1, 5, 6, 1, 6, 2},
	{2, 6, 3, 3, 6, 7},
	{4, 5, 1, 4, 1, 0},
	{4, 0, 3, 4, 3, 7},
	{7, 6, 5, 7, 5, 4}
};

char vertex[] = { "vertex.glsl" };
char fragment[] = { "fragment.glsl" };
GLuint VAO, VBO, VBO_pos, EBO;
GLuint vao, vbo[2];

GLfloat line_rotate[3] = { 30.0f, 30.0f, 0.0f }; // x, y, z

GLfloat rotate_1[3] = { 30.0f, 30.0f, 0.0f }; // x, y, z
GLfloat rotate_rev1[3] = { 30.0f, 30.0f, 0.0f }; // x, y, z
GLfloat translate_1[3] = { -0.5f, 0.0f, 0.0f }; // x, y, z

GLfloat rotate_2[3] = { 30.0f, 30.0f, 0.0f }; // x, y, z
GLfloat rotate_rev2[3] = { 30.0f, 30.0f, 0.0f }; // x, y, z
GLfloat translate_2[3] = { 0.5f, 0.0f, 0.0f }; // x, y, z

GLfloat degree_value[2] = { 1.0f, -1.0f };

GLboolean time_switch = FALSE;
GLboolean rotate_switch = FALSE; // FALSE 자전 | TRUE 공전
GLboolean x_y = FALSE;
GLboolean select[2] = { TRUE, TRUE };

GLint dValue;
GLint number[2] = { 0, 1 };

GLuint shaderProgramID;

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid TimerFunction(int value);

GLvoid make_shaderProgram();
GLvoid InitBuffer_EBO(GLfloat vPositionList[][3], GLuint index[][3], GLfloat color[][3], GLuint vSize, GLuint iSize, GLuint cSize);

GLvoid draw_shape(GLint number);
GLvoid make_cube();
GLvoid make_piramid();

GLvoid red_color(GLfloat* color);
GLvoid green_color(GLfloat* color);
GLvoid blue_color(GLfloat* color);

GLvoid trans_shape(GLfloat* degree, GLfloat* trans, GLfloat* degree_rev, GLuint shaderProgramID);

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
	glutMainLoop();
}

GLvoid drawScene()
{
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgramID);

	for (int i = 0; i < 3; i++) {
		rotate_shape(line_rotate[0], line_rotate[1], line_rotate[2], shaderProgramID);

		InitBuffer_EBO(line[i], line_index, line_color, sizeof(line[i]), sizeof(line_index), sizeof(line_color));
		glBindVertexArray(VAO);

		glDrawElements(GL_LINES, line_vertex, GL_UNSIGNED_INT, 0);
	}

	trans_shape(rotate_1, translate_1, rotate_rev1, shaderProgramID);
	draw_shape(number[0]);

	trans_shape(rotate_2, translate_2, rotate_rev2, shaderProgramID);
	draw_shape(number[1]);
	

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
	case 'x':
		dValue = 0;
		rotate_switch = FALSE;
		x_y = FALSE;
		if (!time_switch) {
			time_switch = TRUE;
			glutTimerFunc(10, TimerFunction, 1);
		}
		break;
	case 'X':
		dValue = 1;
		rotate_switch = FALSE;
		x_y = FALSE;
		if (!time_switch) {
			time_switch = TRUE;
			glutTimerFunc(10, TimerFunction, 1);
		}
		break;
	case 'y':
		dValue = 0;
		rotate_switch = FALSE;
		x_y = TRUE;
		if (!time_switch) {
			time_switch = TRUE;
			glutTimerFunc(10, TimerFunction, 1);
		}
		break;
	case 'Y':
		dValue = 1;
		rotate_switch = FALSE;
		x_y = TRUE;
		if (!time_switch) {
			time_switch = TRUE;
			glutTimerFunc(10, TimerFunction, 1);
		}
		break;
	case 'r':
		dValue = 0;
		rotate_switch = TRUE;
		if (!time_switch) {
			time_switch = TRUE;
			glutTimerFunc(10, TimerFunction, 1);
		}
		break;
	case 'R':
		dValue = 1;
		rotate_switch = TRUE;
		if (!time_switch) {
			time_switch = TRUE;
			glutTimerFunc(10, TimerFunction, 1);
		}
		break;
	case 'c':
		number[0] = (number[0] + 2) % 5;
		number[1] = (number[1] + 2) % 5;
		break;
	case 's':
		number[0] = 0;
		number[1] = 1;
		time_switch = FALSE;
		select[0] = TRUE;
		select[1] = TRUE;

		rotate_1[0] = rotate_1[0] = rotate_2[0] = rotate_2[0] = 30.0f; // x, y, z
		rotate_rev1[1] = rotate_rev2[1] = 30.0f;
		break;
	case '1':
		select[0] = TRUE;
		select[1] = FALSE;
		break;
	case '2':
		select[0] = FALSE;
		select[1] = TRUE;
		break;
	case '3':
		select[0] = TRUE;
		select[1] = TRUE;
		break;
	}
	glutPostRedisplay();
}

GLvoid TimerFunction(int value) {
	glutPostRedisplay();

	if (rotate_switch) {
		rotate_rev1[1] += degree_value[dValue];
		rotate_rev2[1] += degree_value[dValue];
	}
	else {
		if (x_y) {
			if(select[0]) rotate_1[1] += degree_value[dValue];
			if(select[1]) rotate_2[1] += degree_value[dValue];
		}
		else {
			if (select[0]) rotate_1[0] += degree_value[dValue];
			if (select[1]) rotate_2[0] += degree_value[dValue];
		}
	}

	

	if(time_switch) glutTimerFunc(10, TimerFunction, 1);
}

GLvoid draw_shape(GLint number) {
	switch (number) {
	case 0: // cube
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
		 
		make_cube(); 

		InitBuffer_EBO(cube.point, cube.index, cube.color, sizeof(cube.point), sizeof(cube.index), sizeof(cube.color)); 
		glBindVertexArray(VAO); 
		glDrawElements(GL_TRIANGLES, cube.vertex, GL_UNSIGNED_INT, 0); 
		break;
	case 1: // ball
		qobj = gluNewQuadric();
		gluQuadricDrawStyle(qobj, GLU_LINE);
		gluSphere(qobj, LENGTH, 50, 50);
		break;
	case 2: // cylinder
		qobj = gluNewQuadric();
		gluQuadricDrawStyle(qobj, GLU_LINE);
		gluCylinder(qobj, LENGTH, LENGTH, LENGTH*2, 20, 8);
		break;
	case 3: // cone
		qobj = gluNewQuadric();
		gluQuadricDrawStyle(qobj, GLU_LINE);
		gluCylinder(qobj, LENGTH, 0.0, LENGTH*2, 20, 8);
		break;
	case 4: // polyhedron 오각형?
		qobj = gluNewQuadric();
		gluQuadricDrawStyle(qobj, GLU_LINE);
		gluSphere(qobj, LENGTH, 5, 5);
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

// 원점에서 회전 변환
GLvoid trans_shape(GLfloat* degree, GLfloat* trans, GLfloat* degree_rev, GLuint shaderProgramID) {
	glm::mat4 rotateMatrix_x(1.0f);
	rotateMatrix_x = glm::rotate(rotateMatrix_x, glm::radians(degree[0]), glm::vec3(1.0f, 0.0f, 0.0f)); //--- X축 회전

	glm::mat4 rotateMatrix_y(1.0f);
	rotateMatrix_y = glm::rotate(rotateMatrix_y, glm::radians(degree[1]), glm::vec3(0.0f, 1.0f, 0.0f)); //--- Y축 회전

	glm::mat4 rotateMatrix_z(1.0f);
	rotateMatrix_z = glm::rotate(rotateMatrix_z, glm::radians(degree[2]), glm::vec3(0.0f, 0.0f, 1.0f)); //--- Z축 회전

	glm::mat4 rotateMatrix(1.0f);
	rotateMatrix = rotateMatrix_z * rotateMatrix_x * rotateMatrix_y;

	glm::mat4 transMatrix(1.0f);
	transMatrix = glm::translate(transMatrix, glm::vec3(trans[0], trans[1], trans[2])); //--- 이동

	glm::mat4 rotateMatrix_rev_x(1.0f);
	rotateMatrix_rev_x = glm::rotate(rotateMatrix_rev_x, glm::radians(degree_rev[0]), glm::vec3(1.0f, 0.0f, 0.0f)); //--- X축 회전

	glm::mat4 rotateMatrix_rev_y(1.0f);
	rotateMatrix_rev_y = glm::rotate(rotateMatrix_rev_y, glm::radians(degree_rev[1]), glm::vec3(0.0f, 1.0f, 0.0f)); //--- Y축 회전

	glm::mat4 rotateMatrix_rev_z(1.0f);
	rotateMatrix_rev_z = glm::rotate(rotateMatrix_rev_z, glm::radians(degree_rev[2]), glm::vec3(0.0f, 0.0f, 1.0f)); //--- Z축 회전

	glm::mat4 rotateMatrix_rev(1.0f);
	rotateMatrix_rev = rotateMatrix_rev_z * rotateMatrix_rev_x * rotateMatrix_rev_y;

	glm::mat4 Matrix(1.0f);
	Matrix = rotateMatrix_rev * transMatrix * rotateMatrix;

	unsigned int transformLocation = glGetUniformLocation(shaderProgramID, "transform");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(Matrix));
}
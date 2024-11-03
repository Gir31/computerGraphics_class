#include "GL_My_header.h"

#define LENGTH 0.1f
#define PI 3.141592f

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
GLfloat line_color[3][2][3] = {
	{{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}}, // x축
	{{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}}, // y축
	{{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}} // z축
};
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

GLfloat line_rotate[3] = { 30.0f, 30.0f, 30.0f }; // x, y, z

GLfloat shape_rotate[3] = { 0.0f, 0.0f, 0.0f };
GLfloat shape_trans[3] = { 0.5f, 0.0f, 0.0f };
GLfloat shape_rotate_rev[3] = { 30.0f, 30.0f, 30.0f };
GLfloat shape_revolution = { 0.0f };
GLfloat shape_scale[2] = {1.0f, 1.0f};
GLfloat move = { 0.01f };

GLfloat shape_rotate2[3] = { 0.0f, 0.0f, 0.0f };
GLfloat shape_trans2[3] = { -0.5f, 0.0f, 0.0f };
GLfloat shape_rotate_rev2[3] = { 30.0f, 30.0f, 30.0f };
GLfloat shape_revolution2 = { 0.0f };
GLfloat shape_scale2[2] = {1.0f, 1.0f};
GLfloat move2 = { -0.01f };

GLboolean time_switch = FALSE;
GLboolean spiral_move_switch = FALSE;
GLboolean swap_position_switch = FALSE;
GLboolean revolution_swap_switch = FALSE;
GLboolean revolution_switch = FALSE;
GLboolean up_down_swap_switch = FALSE;
GLboolean x = FALSE;
GLboolean y = FALSE;
GLboolean z = FALSE;

GLboolean move_switch[8];

GLuint shaderProgramID;

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid TimerFunction(int value);

GLvoid make_shaderProgram();
GLvoid InitBuffer_EBO(GLfloat vPositionList[][3], GLuint index[][3], GLfloat color[][3], GLuint vSize, GLuint iSize, GLuint cSize);

GLvoid draw_shape(GLint number);
GLvoid make_cube();

GLvoid red_color(GLfloat* color);
GLvoid green_color(GLfloat* color);
GLvoid blue_color(GLfloat* color);

GLvoid trans_shape(GLfloat scale, GLfloat* degree, GLfloat* trans, GLfloat revolution, GLfloat* degree_rev, GLfloat scale2, GLuint shaderProgramID);

GLvoid reset_shape();

GLvoid spiral_move();
GLvoid swap_position();
GLvoid revolution_swap_move();
GLvoid up_down_swap_move();
GLvoid revolution_move();
GLvoid x_move();
GLvoid y_move();
GLvoid z_move();

GLvoid reset_switch(GLint on);
void circle_spiral();

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



	rotate_shape(line_rotate[0], line_rotate[1], line_rotate[2], shaderProgramID);

	if (move_switch[0]) circle_spiral();

	for (int i = 0; i < 3; i++) {
		InitBuffer_EBO(line[i], line_index, line_color[i], sizeof(line[i]), sizeof(line_index), sizeof(line_color[i]));
		glBindVertexArray(VAO);

		glDrawElements(GL_LINES, line_vertex, GL_UNSIGNED_INT, 0);
	}

	trans_shape(shape_scale[0], shape_rotate, shape_trans, shape_revolution, shape_rotate_rev, shape_scale[1], shaderProgramID);
	draw_shape(0);

	trans_shape(shape_scale2[0], shape_rotate2, shape_trans2, shape_revolution2, shape_rotate_rev2, shape_scale2[1], shaderProgramID);
	draw_shape(1);

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
	case '[':
		if (shape_scale[0] > 0.1f) {
			shape_scale[0] -= 0.1f;
			shape_scale2[0] -= 0.1f;
		}
		break;
	case ']':
		shape_scale[0] += 0.1f;
		shape_scale2[0] += 0.1f;
		break;
	case 'o':
		if (shape_scale[1] > 0.1f) {
			shape_scale[1] -= 0.1f;
			shape_scale2[1] -= 0.1f;
		}
		break;
	case 'p':
		shape_scale[1] += 0.1f;
		shape_scale2[1] += 0.1f;
		break;
	case 'q':
		shape_trans[0] += 0.1f;
		shape_trans2[0] += 0.1f;
		break;
	case 'Q':
		shape_trans[0] -= 0.1f;
		shape_trans2[0] -= 0.1f;
		break;
	case 'w':
		shape_trans[1] += 0.1f;
		shape_trans2[1] += 0.1f;
		break;
	case 'W':
		shape_trans[1] -= 0.1f;
		shape_trans2[1] -= 0.1f;
		break;
	case 'e':
		shape_trans[2] += 0.1f;
		break;
	case 'E':
		shape_trans[2] -= 0.1f;
		break;
	case 'a':
		shape_trans[0] += 0.1f;
		break;
	case 'A':
		shape_trans[0] -= 0.1f;
		break;
	case 's':
		shape_trans[1] += 0.1f;
		break;
	case 'S':
		shape_trans[1] -= 0.1f;
		break;
	case 'd':
		shape_trans[2] += 0.1f;
		break;
	case 'D':
		shape_trans[2] -= 0.1f;
		break;
	case 'z':
		shape_trans2[0] += 0.1f;
		break;
	case 'Z':
		shape_trans2[0] -= 0.1f;
		break;
	case 'x':
		shape_trans2[1] += 0.1f;
		break;
	case 'X':
		shape_trans2[1] -= 0.1f;
		break;
	case 'c':
		shape_trans2[2] += 0.1f;
		break;
	case 'C':
		shape_trans2[2] -= 0.1f;
		break;
	case '1':
		reset_shape();
		shape_trans[0] = shape_trans2[0] = 0.0f;
		reset_switch(key - '1');
		break;
	case '2':
		reset_shape();

		shape_trans[0] = 0.5f;
		shape_trans[1] = 0.0f;
		shape_trans[2] = 0.0f;
		shape_revolution = 0.0f;

		shape_trans2[0] = -0.5f;
		shape_trans2[1] = 0.0f;
		shape_trans2[2] = 0.0f;
		shape_revolution2 = 0.0f;

		reset_switch(key - '1');
		break;
	case '3':
		reset_shape();

		reset_switch(key - '1');
		break;
	case '4':
		reset_shape();

		reset_switch(key - '1');
		break;
	case '5':
		reset_shape();

		reset_switch(key - '1');
		break;
	case '6':
		reset_shape();
		shape_trans[0] = shape_trans2[0] = 0.0f;
		reset_switch(key - '1');
		break;
	case '7':
		reset_shape();
		shape_trans[0] = shape_trans2[0] = 0.0f;
		reset_switch(key - '1');
		break;
	case '8':
		reset_shape();
		shape_trans[0] = shape_trans2[0] = 0.0f;
		reset_switch(key - '1');
		break;
	case '9':
		reset_shape();
		reset_switch(key - '1');
		break;
	}
	glutPostRedisplay();
}

GLvoid TimerFunction(int value) {
	glutPostRedisplay();
	
	if (move_switch[0]) spiral_move();
	else if (move_switch[1]) swap_position();
	else if (move_switch[2]) revolution_swap_move();
	else if (move_switch[3]) up_down_swap_move();
	else if (move_switch[4]) revolution_move();
	else if (move_switch[5]) x_move();
	else if (move_switch[6]) y_move();
	else if (move_switch[7]) z_move();

	if(time_switch) glutTimerFunc(10, TimerFunction, 1);
}

GLvoid draw_shape(GLint number) {
	switch (number) {
	case 0: // cube
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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

GLvoid trans_shape(GLfloat scale, GLfloat* degree, GLfloat* trans, GLfloat revolution, GLfloat* degree_rev, GLfloat scale2, GLuint shaderProgramID) { 

	// 원점에서 신축
	glm::mat4 scaleMatrix(1.0f);
	scaleMatrix = glm::scale(scaleMatrix, glm::vec3(scale, scale, scale)); //--- 크기 변환

	// 자전
	glm::mat4 rotateMatrix_x(1.0f);
	rotateMatrix_x = glm::rotate(rotateMatrix_x, glm::radians(degree[0]), glm::vec3(1.0f, 0.0f, 0.0f)); //--- X축 회전

	glm::mat4 rotateMatrix_y(1.0f);
	rotateMatrix_y = glm::rotate(rotateMatrix_y, glm::radians(degree[1]), glm::vec3(0.0f, 1.0f, 0.0f)); //--- Y축 회전

	glm::mat4 rotateMatrix_z(1.0f);
	rotateMatrix_z = glm::rotate(rotateMatrix_z, glm::radians(degree[2]), glm::vec3(0.0f, 0.0f, 1.0f)); //--- Z축 회전

	glm::mat4 rotateMatrix(1.0f);
	rotateMatrix = rotateMatrix_z * rotateMatrix_y * rotateMatrix_x;

	// 원점에 대한 이동
	glm::mat4 transMatrix(1.0f);
	transMatrix = glm::translate(transMatrix, glm::vec3(trans[0], trans[1], trans[2])); //--- 이동

	// 공전
	glm::mat4 rotateMatrix_revolution(1.0f);
	rotateMatrix_revolution = glm::rotate(rotateMatrix_revolution, glm::radians(revolution), glm::vec3(0.0f, 1.0f, 0.0f)); //--- Y축 회전

	// 이동 후 회전
	glm::mat4 rotateMatrix_rev_x(1.0f);
	rotateMatrix_rev_x = glm::rotate(rotateMatrix_rev_x, glm::radians(degree_rev[0]), glm::vec3(1.0f, 0.0f, 0.0f)); //--- X축 회전

	glm::mat4 rotateMatrix_rev_y(1.0f);
	rotateMatrix_rev_y = glm::rotate(rotateMatrix_rev_y, glm::radians(degree_rev[1]), glm::vec3(0.0f, 1.0f, 0.0f)); //--- Y축 회전

	glm::mat4 rotateMatrix_rev_z(1.0f);
	rotateMatrix_rev_z = glm::rotate(rotateMatrix_rev_z, glm::radians(degree_rev[2]), glm::vec3(0.0f, 0.0f, 1.0f)); //--- Z축 회전

	glm::mat4 rotateMatrix_rev(1.0f);
	rotateMatrix_rev = rotateMatrix_rev_z * rotateMatrix_rev_y * rotateMatrix_rev_x;

	// 원점에서 이동 후 신축
	glm::mat4 scaleMatrix2(1.0f);
	scaleMatrix2 = glm::scale(scaleMatrix2, glm::vec3(scale2, scale2, scale2)); //--- 크기 변환

	glm::mat4 Matrix(1.0f);
	Matrix = scaleMatrix2 * rotateMatrix_rev * rotateMatrix_revolution * transMatrix * rotateMatrix * scaleMatrix;

	//--- 변환 행렬 값을 버텍스 세이더로 보내기
	unsigned int transformLocation = glGetUniformLocation(shaderProgramID, "transform");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(Matrix));
}

GLvoid reset_shape() {

	for (int i = 0; i < 3; i++) {
		shape_rotate[i] = shape_rotate2[i] = 0.0f;
		shape_rotate_rev[i] = shape_rotate_rev2[i] = 30.0f;
	}

	for (int i = 0; i < 2; i++) {
		shape_scale[i] = shape_scale2[i] = 1.0f;
		shape_trans[i + 1] = shape_trans2[i + 1] = 0.0f;
	}
	shape_trans[0] = 0.5f;
	shape_trans2[0] = -0.5f;
	shape_revolution = shape_revolution2 = 0.0f;
	move = 0.01f;
	move2 = -0.01f;
}

GLvoid spiral_move() {
	shape_revolution = shape_revolution + 1;

	shape_trans[0] += 0.0005f;
	if (shape_trans[0] >= 1.0f) shape_trans[0] = 0.0f;

	shape_revolution2 = shape_revolution2 + 1;

	shape_trans2[0] -= 0.0005f;
	if (shape_trans2[0] <= -1.0f) shape_trans2[0] = 0.0f;
}

GLvoid swap_position() {
	shape_trans[0] -= 0.01f;
	shape_trans2[0] += 0.01f;

	if (shape_trans[0] <= -0.5f) {
		shape_trans[0] = 0.5f;
		shape_trans2[0] = -0.5f;
	}
}

GLvoid revolution_swap_move() {
	shape_revolution = shape_revolution + 1;
	if (shape_revolution >= 180.0f)
		shape_revolution = 0.0f;

	shape_revolution2 = shape_revolution2 + 1;
	if (shape_revolution2 >= 180.0f)
		shape_revolution2 = 0.0f;
}

GLvoid up_down_swap_move() {
	if (shape_trans[0] > 0.0f) {
		shape_trans[0] -= 0.01f;
		shape_trans[1] += 0.0075f;
	}
	else if(shape_trans[0] < -0.5f) {
		shape_trans[0] = 0.5f;
		shape_trans[1] = 0.0f;
	}
	else {
		shape_trans[0] -= 0.01f;
		shape_trans[1] -= 0.0075f;
	}

	if (shape_trans2[0] < 0.0f) {
		shape_trans2[0] += 0.01f;
		shape_trans2[1] -= 0.0075f;
	}
	else if (shape_trans2[0] > 0.5f) {
		shape_trans2[0] = -0.5f;
		shape_trans2[1] = 0.0f;
	}
	else {
		shape_trans2[0] += 0.01f;
		shape_trans2[1] += 0.0075f;
	}
}

GLvoid revolution_move() {
	shape_revolution = shape_revolution + 1;
	if (shape_revolution == 360.0f) shape_revolution = 0.0f;
	if (shape_revolution < 180.0f)
		shape_scale[0] -= 0.0025f;
	else
		shape_scale[0] += 0.0025f;

	shape_revolution2 = shape_revolution2 + 1;
	if (shape_revolution2 == 360.0f) shape_revolution2 = 0.0f;
	if (shape_revolution2 < 180.0f)
		shape_scale2[0] += 0.0025f;
	else
		shape_scale2[0] -= 0.0025f;

	shape_rotate[1] += 2.0f;
	shape_rotate2[1] += 2.0f;
}

GLvoid x_move() {
	shape_trans[0] += move;

	if (shape_trans[0] < -1.0f || shape_trans[0] > 1.0f) {
		move *= -1;
	}

	shape_trans2[0] += move2;
	if (shape_trans2[0] < -1.0f || shape_trans2[0] > 1.0f) {
		move2 *= -1;
	}
}

GLvoid y_move() {
	shape_trans[1] += move;

	if (shape_trans[1] < -1.0f || shape_trans[1] > 1.0f) {
		move *= -1;
	}

	shape_trans2[1] += move2;
	if (shape_trans2[1] < -1.0f || shape_trans2[1] > 1.0f) {
		move2 *= -1;
	}
}

GLvoid z_move() {
	shape_trans[2] += move;

	if (shape_trans[2] < -1.0f || shape_trans[2] > 1.0f) {
		move *= -1;
	}

	shape_trans2[2] += move2;
	if (shape_trans2[2] < -1.0f || shape_trans2[2] > 1.0f) {
		move2 *= -1;
	}
}

GLvoid reset_switch(GLint on) {
	for (int i = 0; i < 8; i++) move_switch[i] = FALSE;

	if(on != 8)move_switch[on] = TRUE;

	if (!time_switch) {
		time_switch = TRUE;
		glutTimerFunc(10, TimerFunction, 1);
	}
}

void circle_spiral() {
	GLfloat x, z;
	GLfloat r = 0.0f;
	GLint degree = 0;

	glBegin(GL_LINE_STRIP);
	while (r <= 1.0f) {
		x = r * sinf(degree * PI / 180);
		z = r* cosf(degree * PI / 180);
		r += 0.0005f;
		degree = (degree + 1) % 360;

		glPointSize(10);
		glVertex3f(x, 0, z);
	}
	glEnd();
}
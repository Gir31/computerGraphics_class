#include "GL_My_header.h"

#define CUBE_LENGTH 0.2f

typedef struct {
	GLfloat point[8][3];
	GLuint index[12][3];
	GLfloat color[8][3];
	GLsizei vertex;
}CUBE;

CUBE cube;
PROJECTION projection = { 45.0f, 0.0f, 0.1f, 100.0f };

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

glm::vec3 cameraPos = glm::vec3{ 0.0f, 0.0f, 2.0f };
glm::vec3 camera_trans = glm::vec3{ 0.0f, 0.0f, 0.0f };
glm::vec3 camera_degree = glm::vec3{ 0.0f, 0.0f, 0.0f };
glm::vec3 camera_rev_degree = glm::vec3{ 0.0f, 0.0f, 0.0f };

char vertex[] = { "vertex.glsl" };
char fragment[] = { "fragment.glsl" };
GLuint VAO, VBO, VBO_pos, EBO;

GLuint shaderProgramID;

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid SpecialKeyboard(int key, int x, int y);
GLvoid TimerFunction(int value);

void make_shaderProgram();
void InitBuffer_EBO(GLfloat vPositionList[][3], GLuint index[][3], GLfloat color[][3], GLuint vSize, GLuint iSize, GLuint cSize);

void Draw_Coordinate_Plane();

void make_cube();

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
	glutSpecialFunc(SpecialKeyboard);
	glutMainLoop();
}

GLvoid drawScene()
{
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgramID);

	Perspective_Projection_Transformation(projection, shaderProgramID);

	Viewing_Transformation(camera_rev_degree, cameraPos, camera_trans, camera_degree,shaderProgramID);

	Draw_Coordinate_Plane();

	make_cube();

	

	InitBuffer_EBO(cube.point, cube.index, cube.color, sizeof(cube.point), sizeof(cube.index), sizeof(cube.color));
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, cube.vertex, GL_UNSIGNED_INT, 0);

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	projection.Aspect = w/h;
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
	case 'q':
		camera_trans.x += 0.1f;
		break;
	case 'w':
		camera_trans.x -= 0.1f;
		break;
	}
	glutPostRedisplay();
}

GLvoid SpecialKeyboard(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		camera_rev_degree.y -= 2.0f;
		break;
	case GLUT_KEY_RIGHT:
		camera_rev_degree.y += 2.0f;
		break;
	case GLUT_KEY_UP:
		camera_degree.y += 2.0f;
		break;
	case GLUT_KEY_DOWN:
		camera_degree.y -= 2.0f;
		break;
	}
	glutPostRedisplay();
}

GLvoid TimerFunction(int value) {
	glutPostRedisplay();

	glutTimerFunc(10, TimerFunction, 1);
}
// a coordinate plane
void Draw_Coordinate_Plane() {
	for (int i = 0; i < 3; i++) {
		rotate_shape(30.0f, 30.0f, 30.0f, shaderProgramID);

		InitBuffer_EBO(line[i], line_index, line_color, sizeof(line[i]), sizeof(line_index), sizeof(line_color));
		glBindVertexArray(VAO);

		glDrawElements(GL_LINES, line_vertex, GL_UNSIGNED_INT, 0);
	}
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
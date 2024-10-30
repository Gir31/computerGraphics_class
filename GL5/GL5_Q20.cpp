#include "GL_My_header.h"
//========================================================

//========================================================
// 원근 투영
PROJECTION projection = { 45.0f, 0.0f, 0.1f, 50.0f };
glm::vec3 spaceTrans = glm::vec3(0.0f, 0.0f, -5.0f);
// 좌표 평면 회전
glm::vec3 shapeDegree = glm::vec3(30.0f, 0.0f, 0.0f);
//========================================================
char vertex[] = { "vertex.glsl" };
char fragment[] = { "fragment.glsl" };
BUFFER buffer;
GLuint shaderProgramID;
//========================================================
// 사용자 지정 변수
GLboolean timeSwitch = FALSE;
//========================================================
GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid SpecialKeyboard(int key, int x, int y);
GLvoid TimerFunction(int value);
void make_shaderProgram();
//========================================================
// 사용자 지정 함수
GLvoid Draw_Coordinate_Plane();
GLvoid DrawFloor();
//========================================================

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

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgramID);

	Perspective_Projection_Transformation(projection, spaceTrans, shaderProgramID);

	Draw_Coordinate_Plane();
	DrawFloor();

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	projection.Aspect = w / h;
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

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	
	case 'q': case 'Q':
		glutLeaveMainLoop();
	}
	glutPostRedisplay();
}

GLvoid SpecialKeyboard(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		break;
	case GLUT_KEY_RIGHT:
		break;
	case GLUT_KEY_UP:
		break;
	case GLUT_KEY_DOWN:
		break;
	}
	glutPostRedisplay();
}

GLvoid TimerFunction(int value) {
	glutPostRedisplay();

	if (timeSwitch) glutTimerFunc(10, TimerFunction, 1);
}

GLvoid Draw_Coordinate_Plane() {
	glm::vec3 line[3][2] = {
		{glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f)}, // x축
		{glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)}, // y축
		{glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f)} // z축
	};
	glm::vec3 lineColor[3][2] = {
		{glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)}, // x축
		{glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)}, // y축
		{glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f)} // z축
	};
	GLuint line_index[1][3] = { 0, 1 };
	GLsizei line_vertex = 2;

	for (int i = 0; i < 3; i++) {
		unsigned int transformLocation = glGetUniformLocation(shaderProgramID, "model");
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(rotate_shape(shapeDegree)));

		InitBuffer(&buffer, line[i], line_index, lineColor[i], 
			sizeof(line[i]), sizeof(line_index), sizeof(lineColor[i]));
		glBindVertexArray(buffer.VAO); 

		glDrawElements(GL_LINES, line_vertex, GL_UNSIGNED_INT, 0);
	}
}

GLvoid DrawFloor() {
	glm::vec3 floorPoint[4] = {
		{glm::vec3(-1.0f, 0.0f, -1.0f)}, {glm::vec3(1.0f, 0.0f, -1.0f)},
		{glm::vec3(-1.0f, 0.0f, 1.0f)}, {glm::vec3(1.0f, 0.0f, 1.0f)}
	};
	glm::vec3 floorColor[4] = {
		{ glm::vec3(0.0f, 1.0f, 0.0f) },{ glm::vec3(0.0f, 1.0f, 0.0f) },
		{ glm::vec3(0.0f, 1.0f, 0.0f) },{ glm::vec3(0.0f, 1.0f, 0.0f) }
	};
	GLuint floorIndex[2][3] = {
		{0, 2, 1}, {1, 2, 3}
	};
	GLsizei floorVertex = 6;

	unsigned int transformLocation = glGetUniformLocation(shaderProgramID, "model");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(rotate_shape(shapeDegree)));

	InitBuffer(&buffer, floorPoint, floorIndex, floorColor, 
		sizeof(floorPoint), sizeof(floorIndex), sizeof(floorColor));
	glBindVertexArray(buffer.VAO);

	glDrawElements(GL_TRIANGLES, floorVertex, GL_UNSIGNED_INT, 0);
}
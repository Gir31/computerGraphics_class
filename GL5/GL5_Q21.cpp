#include "GL_My_header.h"
//========================================================

//========================================================
// 원근 투영
PROJECTION projection = { 45.0f, 0.0f, 0.1f, 50.0f };
glm::vec3 spaceTrans = glm::vec3(0.0f, 0.0f, -5.0f);
//========================================================
char vertex[] = { "vertex.glsl" };
char fragment[] = { "fragment.glsl" };
BUFFER buffer;
GLuint shaderProgramID;
//========================================================
// 사용자 지정 변수
glm::vec3 facePoint[8] = {
	glm::vec3(-1.f, 1.f, -1.f), glm::vec3(1.f, 1.f, -1.f), 
	glm::vec3(-1.f, 1.f, 1.f), glm::vec3(1.f, 1.f, 1.f),
	glm::vec3(-1.f, -1.f, -1.f), glm::vec3(1.f, -1.f, -1.f),
	glm::vec3(-1.f, -1.f, 1.f), glm::vec3(1.f, -1.f, 1.f)
};
glm::vec3 faceColor[5][4] = {
	{glm::vec3(1.f, 1.f, 0.f), glm::vec3(1.f, 1.f, 0.f), 
	glm::vec3(1.f, 1.f, 0.f),glm::vec3(1.f, 1.f, 0.f)}, // yellow
	{glm::vec3(0.f, 1.f, 1.f), glm::vec3(0.f, 1.f, 1.f),
	glm::vec3(0.f, 1.f, 1.f),glm::vec3(0.f, 1.f, 1.f)}, // cyan
	{glm::vec3(1.f, 0.f, 1.f), glm::vec3(1.f, 0.f, 1.f),
	glm::vec3(1.f, 0.f, 1.f),glm::vec3(1.f, 0.f, 1.f)}, // magenta
	{glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.f, 1.f, 1.f),
	glm::vec3(1.f, 1.f, 1.f),glm::vec3(1.f, 1.f, 1.f)}, //white
	{glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f),
	glm::vec3(0.5f, 0.5f, 0.5f),glm::vec3(0.5f, 0.5f, 0.5f)}, // grey

};
GLfloat faceIndex[6][6] = {

};
GLsizei faceVertex = 6;

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
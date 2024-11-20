#include "GL_My_header.h"
//========================================================
#define PI 3.141592f
//========================================================
// 원근 투영
PROJECTION projection = { 45.0f, 0.0f, 0.1f, 50.0f };
glm::vec3 spaceTrans = glm::vec3(0.0f, 0.0f, -2.0f);
//========================================================
char vertex[] = { "vertex.glsl" };
char fragment[] = { "fragment.glsl" };
GLuint vao, vbo[2];
GLuint shaderProgramID;
//========================================================
// 사용자 지정 변수
float points[] = {
	-1.f, 1.f, -1.f,
	1.f, 1.f, -1.f,
	1.f, 1.f, 1.f,
	-1.f, 1.f, 1.f, // 윗

	-1.f, 1.f, -1.f,
	-1.f, -1.f, -1.f,
	1.f, -1.f, -1.f,
	1.f, 1.f, -1.f, // 뒷

	-1.f, 1.f, -1.f,
	-1.f, 1.f, 1.f,
	-1.f, -1.f, 1.f,
	-1.f, -1.f, -1.f, // 왼

	1.f, -1.f, -1.f,
	-1.f, -1.f, -1.f,
	-1.f, -1.f, 1.f,
	1.f, -1.f, 1.f, // 아래

	1.f, 1.f, 1.f,
	1.f, 1.f, -1.f,
	1.f, -1.f, -1.f,
	1.f, -1.f, 1.f, // 오

	-0.2f, 0.2f, -0.2f,
	-0.2f, 0.2f, 0.2f,
	0.2f, 0.2f, 0.2f,
	0.2f, 0.2f, -0.2f, // 장애물 1 위

	-0.2f, 0.2f, -0.2f,
	-0.2f, -0.2f, -0.2f,
	-0.2f, -0.2f, 0.2f,
	-0.2f, 0.2f, 0.2f, // 장애물 1 왼

	-0.2f, 0.2f, -0.2f, 
	0.2f, 0.2f, -0.2f,
	0.2f, -0.2f, -0.2f,
	-0.2f, -0.2f, -0.2f, // 장애물 1 뒤

	0.2f, 0.2f, -0.2f,
	0.2f, 0.2f, 0.2f,
	0.2f, -0.2f, 0.2f,
	0.2f, -0.2f, -0.2f, // 장애물 1 오

	-0.2f, 0.2f, 0.2f,
	-0.2f, -0.2f, 0.2f,
	0.2f, -0.2f, 0.2f,
	0.2f, 0.2f, 0.2f, // 장애물 1 앞

	0.2f, -0.2f, 0.2f,
	-0.2f, -0.2f, 0.2f,
	-0.2f, -0.2f, -0.2f,
	0.2f, -0.2f, -0.2f, // 장애물 1 밑

	-0.15f, 0.15f, -0.15f,
	-0.15f, 0.15f, 0.15f,
	0.15f, 0.15f, 0.15f,
	0.15f, 0.15f, -0.15f, // 장애물 2 위

	-0.15f, 0.15f, -0.15f,
	-0.15f, -0.15f, -0.15f,
	-0.15f, -0.15f, 0.15f,
	-0.15f, 0.15f, 0.15f, // 장애물 2 왼

	-0.15f, 0.15f, -0.15f,
	0.15f, 0.15f, -0.15f,
	0.15f, -0.15f, -0.15f,
	-0.15f, -0.15f, -0.15f, // 장애물 2 뒤

	0.15f, 0.15f, -0.15f,
	0.15f, 0.15f, 0.15f,
	0.15f, -0.15f, 0.15f,
	0.15f, -0.15f, -0.15f, // 장애물 2 오

	-0.15f, 0.15f, 0.15f,
	-0.15f, -0.15f, 0.15f,
	0.15f, -0.15f, 0.15f,
	0.15f, 0.15f, 0.15f, // 장애물 2 앞

	0.15f, -0.15f, 0.15f,
	-0.15f, -0.15f, 0.15f,
	-0.15f, -0.15f, -0.15f,
	0.15f, -0.15f, -0.15f, // 장애물 2 밑

	-0.1f, 0.1f, -0.1f,
	-0.1f, 0.1f, 0.1f,
	0.1f, 0.1f, 0.1f,
	0.1f, 0.1f, -0.1f, // 장애물 3 위

	-0.1f, 0.1f, -0.1f,
	-0.1f, -0.1f, -0.1f,
	-0.1f, -0.1f, 0.1f,
	-0.1f, 0.1f, 0.1f, // 장애물 3 왼

	-0.1f, 0.1f, -0.1f,
	0.1f, 0.1f, -0.1f,
	0.1f, -0.1f, -0.1f,
	-0.1f, -0.1f, -0.1f, // 장애물 3 뒤

	0.1f, 0.1f, -0.1f,
	0.1f, 0.1f, 0.1f,
	0.1f, -0.1f, 0.1f,
	0.1f, -0.1f, -0.1f, // 장애물 3 오

	-0.1f, 0.1f, 0.1f,
	-0.1f, -0.1f, 0.1f,
	0.1f, -0.1f, 0.1f,
	0.1f, 0.1f, 0.1f, // 장애물 3 앞

	0.1f, -0.1f, 0.1f,
	-0.1f, -0.1f, 0.1f,
	-0.1f, -0.1f, -0.1f,
	0.1f, -0.1f, -0.1f, // 장애물 3 밑
};
float color[] = {
	0.3f, 0.3f, 0.3f,
	0.3f, 0.3f, 0.3f,
	0.3f, 0.3f, 0.3f,
	0.3f, 0.3f, 0.3f,

	0.7f, 0.7f, 0.7f,
	0.7f, 0.7f, 0.7f,
	0.7f, 0.7f, 0.7f,
	0.7f, 0.7f, 0.7f,

	0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,

	0.3f, 0.3f, 0.3f,
	0.3f, 0.3f, 0.3f,
	0.3f, 0.3f, 0.3f,
	0.3f, 0.3f, 0.3f,

	0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,

	0.0f, 1.f, 1.f,
	0.0f, 1.f, 1.f,
	0.0f, 1.f, 1.f,
	0.0f, 1.f, 1.f,

	0.0f, 1.f, 1.f,
	0.0f, 1.f, 1.f,
	0.0f, 1.f, 1.f,
	0.0f, 1.f, 1.f,

	0.0f, 1.f, 1.f,
	0.0f, 1.f, 1.f,
	0.0f, 1.f, 1.f,
	0.0f, 1.f, 1.f,

	0.0f, 1.f, 1.f,
	0.0f, 1.f, 1.f,
	0.0f, 1.f, 1.f,
	0.0f, 1.f, 1.f,

	0.0f, 1.f, 1.f,
	0.0f, 1.f, 1.f,
	0.0f, 1.f, 1.f,
	0.0f, 1.f, 1.f,

	0.0f, 1.f, 1.f,
	0.0f, 1.f, 1.f,
	0.0f, 1.f, 1.f,
	0.0f, 1.f, 1.f,

	1.f, 0.f, 1.f,
	1.f, 0.f, 1.f,
	1.f, 0.f, 1.f,
	1.f, 0.f, 1.f,

	1.f, 0.f, 1.f,
	1.f, 0.f, 1.f,
	1.f, 0.f, 1.f,
	1.f, 0.f, 1.f,

	1.f, 0.f, 1.f,
	1.f, 0.f, 1.f,
	1.f, 0.f, 1.f,
	1.f, 0.f, 1.f,

	1.f, 0.f, 1.f,
	1.f, 0.f, 1.f,
	1.f, 0.f, 1.f,
	1.f, 0.f, 1.f,

	1.f, 0.f, 1.f,
	1.f, 0.f, 1.f,
	1.f, 0.f, 1.f,
	1.f, 0.f, 1.f,

	1.f, 0.f, 1.f,
	1.f, 0.f, 1.f,
	1.f, 0.f, 1.f,
	1.f, 0.f, 1.f,

	1.f, 1.f, 0.f,
	1.f, 1.f, 0.f,
	1.f, 1.f, 0.f,
	1.f, 1.f, 0.f,

	1.f, 1.f, 0.f,
	1.f, 1.f, 0.f,
	1.f, 1.f, 0.f,
	1.f, 1.f, 0.f,

	1.f, 1.f, 0.f,
	1.f, 1.f, 0.f,
	1.f, 1.f, 0.f,
	1.f, 1.f, 0.f,

	1.f, 1.f, 0.f,
	1.f, 1.f, 0.f,
	1.f, 1.f, 0.f,
	1.f, 1.f, 0.f,

	1.f, 1.f, 0.f,
	1.f, 1.f, 0.f,
	1.f, 1.f, 0.f,
	1.f, 1.f, 0.f,

	1.f, 1.f, 0.f,
	1.f, 1.f, 0.f,
	1.f, 1.f, 0.f,
	1.f, 1.f, 0.f,
};

glm::vec3 rotateBox = glm::vec3(0.f, 0.f, 0.f);

glm::vec3 transObstacle1 = glm::vec3(0.f, -0.8f, -0.35f);
glm::vec3 transObstacle2 = glm::vec3(0.f, -0.85f, 0.f);
glm::vec3 transObstacle3 = glm::vec3(0.f, -0.9f, 0.25f);

glm::vec3 transCamera = glm::vec3(0.0f, 0.0f, 2.0f);
glm::vec3 rotateCamera = glm::vec3(0, 0, 0);

GLfloat initMouseDegree; // 마우스의 초기 각도

GLboolean left_button = FALSE;
GLboolean timeSwitch = FALSE;
//========================================================
GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Motion(int x, int y);
GLvoid Keyboard(unsigned char key, int x, int y); 
GLvoid SpecialKeyboard(int key, int x, int y);
GLvoid TimerFunction(int value);
void make_shaderProgram();
void InitBuffer_();
//========================================================
// 사용자 지정 함수
GLvoid cameraTranslation(glm::vec3 cameraTrans, glm::vec3 cameraRotate);
GLvoid fallObstacle();
//========================================================

int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800.0f, 800.0f);
	glutCreateWindow("Example1");

	glewExperimental = GL_TRUE;
	glewInit();

	make_shaderProgram();
	InitBuffer_();
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKeyboard);
	glutTimerFunc(10, TimerFunction, 1);
	glutMainLoop();
}

GLvoid drawScene()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Perspective_Projection_Transformation(projection, spaceTrans, shaderProgramID);
	cameraTranslation(transCamera, rotateCamera);

	unsigned int transformLocation = glGetUniformLocation(shaderProgramID, "model");

	glm::mat4 box = glm::mat4(1.0f);
	box = translation_shape(glm::vec3(0.f, 0.f, 0.f)) * rotate_shape(rotateBox);
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(box));
	glDrawArrays(GL_QUADS, 0, 20);

	glm::mat4 obsltacle1 = glm::mat4(1.0f);
	obsltacle1 = rotate_shape(rotateBox) * translation_shape(transObstacle1);
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(obsltacle1));
	glDrawArrays(GL_QUADS, 20, 24);

	glm::mat4 obsltacle2 = glm::mat4(1.0f);
	obsltacle2 = rotate_shape(rotateBox) * translation_shape(transObstacle2);
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(obsltacle2));
	glDrawArrays(GL_QUADS, 44, 24);

	glm::mat4 obsltacle3 = glm::mat4(1.0f);
	obsltacle3 = rotate_shape(rotateBox) * translation_shape(transObstacle3);
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(obsltacle3));
	glDrawArrays(GL_QUADS, 68, 24);

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

void InitBuffer_() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(2, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(points), points, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(color), color, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
}

GLvoid Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		left_button = TRUE;

		initMouseDegree = atan2f(((y / 400.f) - 1.f) * -1.f, (x / 400.f) - 1.f) / PI * 180.f; // 마우스 클릭시 초기 각도 초기화
	}else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		left_button = FALSE;
	}
	glutPostRedisplay();
}

void Motion(int x, int y) {
	if (left_button) {
		GLfloat curMouseDegree; // 마우스 이동 시 각도  

		curMouseDegree = atan2f(((y / 400.f) - 1.f) * -1.f, (x / 400.f) - 1.f) / PI * 180.f; // 현재 각도 초기화

		rotateBox.z += curMouseDegree - initMouseDegree; // 초기 각도와 현재 각도의 차를 rotate.z에 더해줌 

		initMouseDegree = curMouseDegree; // 초기 각도를 현재 각도로 초기화

		if (rotateBox.z < 0.f) rotateBox.z += 360.f;
		if (rotateBox.z >= 360.f) rotateBox.z = 0.f;
	}
	glutPostRedisplay();
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
		transCamera.x -= 0.1f;
		break;
	case GLUT_KEY_RIGHT:
		transCamera.x += 0.1f;
		break;
	case GLUT_KEY_UP:
		transCamera.z += 0.1f;
		break;
	case GLUT_KEY_DOWN:
		transCamera.z -= 0.1f;
		std::cout << transCamera.z << std::endl;
		break;
	}
	glutPostRedisplay();
}

GLvoid TimerFunction(int value) {
	glutPostRedisplay();

	fallObstacle();

	glutTimerFunc(10, TimerFunction, 1);
}

GLvoid cameraTranslation(glm::vec3 cameraTrans, glm::vec3 cameraRotate) {
	glm::vec3 zeroPoint = glm::vec3(0, 0, 0);
	glm::mat4 view = glm::mat4(1.0f);
	view = camera_locate(cameraTrans, zeroPoint) * rotate_camera(cameraRotate);

	unsigned int viewLocation = glGetUniformLocation(shaderProgramID, "view");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
}

GLvoid fallObstacle() {

	transObstacle1.x += sinf(rotateBox.z / PI * 180.f) * 0.01f;

	if (!(transObstacle1.x > -0.8f && transObstacle1.x < 0.8f)) {
		
		if (transObstacle1.x < 0.f) transObstacle1.x = -0.8f;
		else transObstacle1.x = 0.8f;
	}

	transObstacle2.x += sinf(rotateBox.z / PI * 180.f) * 0.01f;

	if (!(transObstacle2.x > -0.85f && transObstacle2.x < 0.85f)) {
		if (transObstacle2.x < 0.f) transObstacle2.x = -0.85f;
		else transObstacle2.x = 0.85f;
	}

	transObstacle3.x += sinf(rotateBox.z / PI * 180.f) * 0.01f;

	if(!(transObstacle3.x > -0.9f && transObstacle3.x < 0.9f)) {
		if (transObstacle3.x < 0.f) transObstacle3.x = -0.9f;
		else transObstacle3.x = 0.9f;
	}
}
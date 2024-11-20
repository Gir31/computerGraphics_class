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

glm::vec3 bounceBall = glm::vec3(0, 0, 0);
glm::vec3 touchWall = glm::vec3(0, 0, 0);
GLfloat bounceValue = 0.01f;

glm::vec2 hitBox[4] = { 
	glm::vec2(-0.8f, 0.8f) ,glm::vec2(0.8f, 0.8f),
	glm::vec2(-0.8f, -0.8f) ,glm::vec2(0.8f, -0.8f)
};

GLUquadric* qobj;

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
GLboolean touch(GLfloat bx, GLfloat by); 
GLvoid bounce();
//========================================================

int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(100.0f, 100.0f);
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


	glm::mat4 ball = glm::mat4(1.0f);
	ball = translation_shape(bounceBall);
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(ball));
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluSphere(qobj, 0.05f, 50, 50);

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
	bounce();

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

	// 장애물 1 자유 낙하
	transObstacle1.x += sinf(rotateBox.z * PI / 180.f) * -0.05f;

	if (!(transObstacle1.x > -0.8f && transObstacle1.x < 0.8f)) {
		
		if (transObstacle1.x < 0.f) transObstacle1.x = -0.8f;
		else transObstacle1.x = 0.8f;
	}

	transObstacle1.y += cosf(rotateBox.z * PI / 180.f) * -0.05f;

	if (!(transObstacle1.y > -0.8f && transObstacle1.y < 0.8f)) {

		if (transObstacle1.y < 0.f) transObstacle1.y = -0.8f;
		else transObstacle1.y = 0.8f;
	}

	// 장애물 2 자유 낙하
	transObstacle2.x += sinf(rotateBox.z * PI / 180.f) * -0.05f;

	if (!(transObstacle2.x > -0.85f && transObstacle2.x < 0.85f)) {
		if (transObstacle2.x < 0.f) transObstacle2.x = -0.85f;
		else transObstacle2.x = 0.85f;
	}

	transObstacle2.y += cosf(rotateBox.z * PI / 180.f) * -0.05f;

	if (!(transObstacle2.y > -0.8f && transObstacle2.y < 0.8f)) {

		if (transObstacle2.y < 0.f) transObstacle2.y = -0.8f;
		else transObstacle2.y = 0.8f;
	}

	// 장애물 3 자유 낙하
	transObstacle3.x += sinf(rotateBox.z * PI / 180.f) * -0.05f;

	if(!(transObstacle3.x > -0.9f && transObstacle3.x < 0.9f)) {
		if (transObstacle3.x < 0.f) transObstacle3.x = -0.9f;
		else transObstacle3.x = 0.9f;
	}

	transObstacle3.y += cosf(rotateBox.z * PI / 180.f) * -0.05f;

	if (!(transObstacle3.y > -0.8f && transObstacle3.y < 0.8f)) {

		if (transObstacle3.y < 0.f) transObstacle3.y = -0.8f;
		else transObstacle3.y = 0.8f;
	}

}

GLboolean touch(GLfloat bx, GLfloat by) {
	GLfloat x[4], y[4];
	GLfloat m[4], c[4];
	GLfloat R = 0.9 * sqrtf(2);

	// 오른쪽 위 점
	x[0] = R * cosf((135.f - rotateBox.z) * PI / 180.f) * -1; y[0] = R * sinf((135.f - rotateBox.z) * PI / 180.f);

	// 오른쪽 아래 점
	x[1] = R * cosf((225.f - rotateBox.z) * PI / 180.f) * -1; y[1] = R * sinf((225.f - rotateBox.z) * PI / 180.f);

	// 왼쪽 아래 점
	x[2] = R * cosf((315.f - rotateBox.z) * PI / 180.f) * -1; y[2] = R * sinf((315.f - rotateBox.z) * PI / 180.f);

	// 왼쪽 위 점
	x[3] = R * cosf((45.f - rotateBox.z) * PI / 180.f) * -1; y[3] = R * sinf((45.f - rotateBox.z) * PI / 180.f);

	m[0] = (y[1] - y[0]) / (x[1] - x[0]); c[0] = y[0] - (m[0] * x[0]);
	m[1] = (y[2] - y[1]) / (x[2] - x[1]); c[1] = y[1] - (m[1] * x[1]);
	m[2] = (y[3] - y[2]) / (x[3] - x[2]); c[2] = y[2] - (m[2] * x[2]);
	m[3] = (y[0] - y[3]) / (x[0] - x[3]); c[3] = y[3] - (m[3] * x[3]);

	// y = mx + c
	GLfloat bm = by / bx;
	GLfloat bl = sqrtf((powf(bx, 2) + powf(by, 2))); // 공과 원점 사이의 거리 
	GLfloat tx, ty, tl; // 원점과 각 변 사이의 거리

	for (int i = 0; i < 4; i++) {
		if (m[i] != bm) {
			tx = (c[i] / (m[i] - bm)) * -1;
			ty = m[i] * tx + c[i];
			tl = sqrtf((powf(tx, 2) + powf(ty, 2)));
			if (bl >= tl) return TRUE;
		}
	}

	return FALSE;
}

GLvoid bounce() {
	bounceBall.y += bounceValue * cosf(touchWall.z * PI / 180.f);
	bounceBall.x += bounceValue * sinf(touchWall.z * PI / 180.f);

	if (touch(bounceBall.x, bounceBall.y)) {
		bounceValue *= -1;
		touchWall.z -= rotateBox.z;
	}
}
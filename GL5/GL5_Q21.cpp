#include "GL_My_header.h"
//========================================================
typedef struct {
	glm::vec3 move;
	glm::vec3 dir;
	glm::vec3 left_motion, right_motion;

	GLint moveDir; // | 0 : up | 1 : right | 2 : down | 3 : left |
}ROBOT;

ROBOT robot;
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

	0.f, 1.f, 0.f,
	0.f, -1.f, 0.f,
	1.f, -1.f, 0.f,
	1.f, 1.f, 0.f, // 좌 문

	-1.f, 1.f, 0.f,
	-1.f, -1.f, 0.f,
	0.f, -1.f, 0.f,
	0.f, 1.f, 0.f, // 우 문

	-0.05f, -0.7f, -0.05f,
	-0.05f, -0.7f, 0.05f,
	0.05f, -0.7f, 0.05f,
	0.05f, -0.7f, -0.05f, // 머리 위

	-0.05f, -0.7f, -0.05f,
	-0.05f, -0.8f, -0.05f,
	-0.05f, -0.8f, 0.05f,
	-0.05f, -0.7f, 0.05f, // 머리 왼쪽

	-0.05f, -0.8f, -0.05f,
	-0.05f, -0.7f, -0.05f,
	0.05f, -0.7f, -0.05f,
	0.05f, -0.8f, -0.05f, // 머리 뒤쪽

	0.05f, -0.7f, 0.05f,
	0.05f, -0.8f, 0.05f,
	0.05f, -0.8f, -0.05f,
	0.05f, -0.7f, -0.05f, // 머리 오른쪽

	-0.05f, -0.7f, 0.05f,
	-0.05f, -0.8f, 0.05f,
	0.05f, -0.8f, 0.05f,
	0.05f, -0.7f, 0.05f, // 머리 앞쪽

	-0.05f, -0.8f, 0.05f,
	-0.05f, -0.8f, -0.05f,
	0.05f, -0.8f, -0.05f,
	0.05f, -0.8f, 0.05f, // 머리 아래쪽

	-0.05f, -0.8f, -0.025f,
	-0.05f, -0.8f, 0.025f,
	0.05f, -0.8f, 0.025f,
	0.05f, -0.8f, -0.025f, // 몸통 위

	-0.05f, -0.8f, -0.025f,
	-0.05f, -0.9f, -0.025f,
	-0.05f, -0.9f, 0.025f,
	-0.05f, -0.8f, 0.025f, // 몸통 왼쪽

	-0.05f, -0.9f, -0.025f,
	-0.05f, -0.8f, -0.025f,
	0.05f, -0.8f, -0.025f,
	0.05f, -0.9f, -0.025f, // 몸통 뒤쪽

	0.05f, -0.8f, 0.025f,
	0.05f, -0.9f, 0.025f,
	0.05f, -0.9f, -0.025f,
	0.05f, -0.8f, -0.025f, // 몸통 오른쪽

	-0.05f, -0.8f, 0.025f,
	-0.05f, -0.9f, 0.025f,
	0.05f, -0.9f, 0.025f,
	0.05f, -0.8f, 0.025f, // 몸통 앞쪽

	-0.05f, -0.9f, 0.025f,
	-0.05f, -0.9f, -0.025f,
	0.05f, -0.9f, -0.025f,
	0.05f, -0.9f, 0.025f, // 몸통 아래쪽

	-0.07f, -0.8f, -0.025f,
	-0.07f, -0.8f, 0.025f,
	-0.05f, -0.8f, 0.025f,
	-0.05f, -0.8f, -0.025f, // 오른팔 위

	-0.07f, -0.8f, -0.025f,
	-0.07f, -0.9f, -0.025f,
	-0.07f, -0.9f, 0.025f,
	-0.07f, -0.8f, 0.025f, // 오른팔 왼쪽

	-0.07f, -0.9f, -0.025f,
	-0.07f, -0.8f, -0.025f,
	-0.05f, -0.8f, -0.025f,
	-0.05f, -0.9f, -0.025f, // 오른팔 뒤쪽

	-0.05f, -0.8f, 0.025f,
	-0.05f, -0.9f, 0.025f,
	-0.05f, -0.9f, -0.025f,
	-0.05f, -0.8f, -0.025f, // 오른팔 오른쪽

	-0.07f, -0.8f, 0.025f,
	-0.07f, -0.9f, 0.025f,
	-0.05f, -0.9f, 0.025f,
	-0.05f, -0.8f, 0.025f, // 오른팔 앞쪽

	-0.07f, -0.9f, 0.025f,
	-0.05f, -0.9f, -0.025f,
	-0.05f, -0.9f, -0.025f,
	-0.07f, -0.9f, 0.025f, // 오른팔 아래쪽

	0.07f, -0.8f, -0.025f,
	0.07f, -0.8f, 0.025f,
	0.05f, -0.8f, 0.025f,
	0.05f, -0.8f, -0.025f, // 왼팔 위

	0.07f, -0.8f, -0.025f,
	0.07f, -0.9f, -0.025f,
	0.07f, -0.9f, 0.025f,
	0.07f, -0.8f, 0.025f, // 왼팔 왼쪽

	0.07f, -0.9f, -0.025f,
	0.07f, -0.8f, -0.025f,
	0.05f, -0.8f, -0.025f,
	0.05f, -0.9f, -0.025f, // 왼팔 뒤쪽

	0.05f, -0.8f, 0.025f,
	0.05f, -0.9f, 0.025f,
	0.05f, -0.9f, -0.025f,
	0.05f, -0.8f, -0.025f, // 왼팔 오른쪽

	0.07f, -0.8f, 0.025f,
	0.07f, -0.9f, 0.025f,
	0.05f, -0.9f, 0.025f,
	0.05f, -0.8f, 0.025f, // 왼팔 앞쪽

	0.07f, -0.9f, 0.025f,
	0.05f, -0.9f, -0.025f,
	0.05f, -0.9f, -0.025f,
	0.07f, -0.9f, 0.025f, // 왼팔 아래쪽

	-0.05f, -0.9f, -0.025f,
	-0.05f, -0.9f, 0.025f,
	0.f, -0.9f, 0.025f,
	0.f, -0.9f, -0.025f, // 오른다리 위

	-0.05f, -0.9f, -0.025f,
	-0.05f, -1.f, -0.025f,
	-0.05f, -1.f, 0.025f,
	-0.05f, -0.9f, 0.025f, // 오른다리 왼쪽

	-0.05f, -1.f, -0.025f,
	-0.05f, -0.9f, -0.025f,
	0.f, -0.9f, -0.025f,
	0.f, -1.f, -0.025f, // 오른다리 뒤쪽

	0.f, -0.9f, 0.025f,
	0.f, -1.f, 0.025f,
	0.f, -1.f, -0.025f,
	0.f, -0.9f, -0.025f, // 오른다리 오른쪽

	-0.05f, -0.9f, 0.025f,
	-0.05f, -1.f, 0.025f,
	0.f, -1.f, 0.025f,
	0.f, -0.9f, 0.025f, // 오른다리 앞쪽

	-0.05f, -1.f, 0.025f,
	-0.05f, -1.f, -0.025f,
	0.f, -1.f, -0.025f,
	0.f, -1.f, 0.025f, // 오른다리 아래쪽

	0.f, -0.9f, -0.025f,
	0.f, -0.9f, 0.025f,
	0.05f, -0.9f, 0.025f,
	0.05f, -0.9f, -0.025f, // 왼다리 위

	0.f, -0.9f, -0.025f,
	0.f, -1.f, -0.025f,
	0.f, -1.f, 0.025f,
	0.f, -0.9f, 0.025f, // 왼다리 왼쪽

	0.f, -1.f, -0.025f,
	0.f, -0.9f, -0.025f,
	0.05f, -0.9f, -0.025f,
	0.05f, -1.f, -0.025f, // 왼다리 뒤쪽

	0.05f, -0.9f, 0.025f,
	0.05f, -1.f, 0.025f,
	0.05f, -1.f, -0.025f,
	0.05f, -0.9f, -0.025f, // 왼다리 오른쪽

	0.f, -0.9f, 0.025f,
	0.f, -1.f, 0.025f,
	0.05f, -1.f, 0.025f,
	0.05f, -0.9f, 0.025f, // 왼다리 앞쪽

	0.f, -1.f, 0.025f,
	0.f, -1.f, -0.025f,
	0.05f, -1.f, -0.025f,
	0.05f, -1.f, 0.025f, // 왼다리 아래쪽

	0.f, -0.75f, 0.05f,
	-0.01f, -0.775f, 0.05f,
	0.f, -0.775f, 0.07f, // 코 왼쪽

	0.f, -0.75f, 0.05f,
	0.01f, -0.775f, 0.05f,
	0.f, -0.775f, 0.07f, // 코 오른쪽

	-0.01f, -0.775f, 0.05f,
	0.01f, -0.775f, 0.05f,
	0.f, -0.775f, 0.07f, // 코 밑

	-0.2f, 0.2f, -0.2f,
	-0.2f, 0.2f, 0.2f,
	0.2f, 0.2f, 0.2f,
	0.2f, 0.2f, -0.2f,

	-0.2f, 0.2f, -0.2f,
	0.2f, 0.2f, -0.2f,
	0.2f, 0.f, -0.2f,
	-0.2f, 0.f, -0.2f,

	-0.2f, 0.2f, 0.2f,
	-0.2f, 0.2f, -0.2f,
	-0.2f, 0.f, -0.2f,
	-0.2f, 0.f, 0.2f,

	-0.2f, 0.2f, 0.2f,
	-0.2f, 0.f, 0.2f,
	0.2f, 0.f, 0.2f,
	0.2f, 0.2f, 0.2f,

	0.2f, 0.2f, 0.2f,
	0.2f, 0.f, 0.2f,
	0.2f, 0.f, -0.2f,
	0.2f, 0.2f, -0.2f
};
float color[] = {
	0, 1, 1, 
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,

	0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,

	1, 0, 1,
	1, 0, 1,
	1, 0, 1,
	1, 0, 1,

	1, 1, 1,
	1, 1, 1,
	1, 1, 1,
	1, 1, 1,

	1, 1, 0,
	1, 1, 0,
	1, 1, 0,
	1, 1, 0,

	1, 0, 0, 
	0, 1, 0, 
	0, 0, 1, 
	0, 1, 1, 

	0, 1, 1,
	0, 0, 1,
	0, 1, 0,
	1, 0, 0,

	0, 0, 1,
	0, 0, 1,
	0, 0, 1,
	0, 0, 1,

	0, 0, 1,
	0, 0, 1,
	0, 0, 1,
	0, 0, 1,

	0, 0, 1,
	0, 0, 1,
	0, 0, 1,
	0, 0, 1,

	0, 0, 1,
	0, 0, 1,
	0, 0, 1,
	0, 0, 1,

	0, 0, 1,
	0, 0, 1,
	0, 0, 1,
	0, 0, 1,

	0, 0, 1,
	0, 0, 1,
	0, 0, 1,
	0, 0, 1,

	0, 1, 0,
	0, 1, 0,
	0, 1, 0,
	0, 1, 0,

	0, 1, 0,
	0, 1, 0,
	0, 1, 0,
	0, 1, 0,

	0, 1, 0,
	0, 1, 0,
	0, 1, 0,
	0, 1, 0,

	0, 1, 0,
	0, 1, 0,
	0, 1, 0,
	0, 1, 0,

	0, 1, 0,
	0, 1, 0,
	0, 1, 0,
	0, 1, 0,

	0, 1, 0,
	0, 1, 0,
	0, 1, 0,
	0, 1, 0,

	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,

	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,

	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,

	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,

	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,

	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
		
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,

	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,

	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,

	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,

	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,

	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
		
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,

	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,

	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,

	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,

	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,

	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
		
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,

	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,

	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,

	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,

	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,

	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,

	0, 0, 0,
	0, 0, 0,
	0, 0, 0,

	0, 0, 0,
	0, 0, 0,
	0, 0, 0,

	0, 0, 0,
	0, 0, 0,
	0, 0, 0,

	0.75f, 0.75f, 0.75f,
	0.75f, 0.75f, 0.75f,
	0.75f, 0.75f, 0.75f,
	0.75f, 0.75f, 0.75f,

	0.75f, 0.75f, 0.75f,
	0.75f, 0.75f, 0.75f,
	0.75f, 0.75f, 0.75f,
	0.75f, 0.75f, 0.75f,

	0.75f, 0.75f, 0.75f,
	0.75f, 0.75f, 0.75f,
	0.75f, 0.75f, 0.75f,
	0.75f, 0.75f, 0.75f,

	0.75f, 0.75f, 0.75f,
	0.75f, 0.75f, 0.75f,
	0.75f, 0.75f, 0.75f,
	0.75f, 0.75f, 0.75f,

	0.75f, 0.75f, 0.75f,
	0.75f, 0.75f, 0.75f,
	0.75f, 0.75f, 0.75f,
	0.75f, 0.75f, 0.75f
};

float obstacle1[4] = {-1.f, -0.2f, -0.6f, 0.2f}; // 장애물 xz좌표
float obstacle2[4] = {0.3f, 0.3f, 0.7f, 0.7f};
float obstacle3[4] = { -0.4f, 0.6f, 0.f, 1.f };

glm::vec3 transObstacle = glm::vec3(-0.8f, -1.f, 0.f);
glm::vec3 transObstacle1 = glm::vec3(0.5f, -1.f, 0.5f);
glm::vec3 transObstacle2 = glm::vec3(-0.2f, -1.f, 0.8f);

glm::vec3 transCamera = glm::vec3(0.0f, 0.0f, 2.0f);
glm::vec3 rotateCamera = glm::vec3(0, 0, 0);
glm::vec3 open = glm::vec3(0, 0, 0);

GLfloat rotateValue = 1.f;
GLfloat openValue;
GLfloat armLegMotionValue = 5.f;
GLfloat robotJumpValue = 0.05f;
GLfloat speedValue = 0.01f;

GLboolean timeSwitch = FALSE;
GLboolean rFlag = FALSE;
GLboolean oFlag = FALSE;
GLboolean jFlag = FALSE;
GLboolean alFlag = TRUE;
//========================================================
GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid SpecialKeyboard(int key, int x, int y);
GLvoid TimerFunction(int value);
void make_shaderProgram();
void InitBuffer_();
//========================================================
// 사용자 지정 함수
GLvoid cameraTranslation(glm::vec3 cameraTrans, glm::vec3 cameraRotate);

GLvoid armLegMotion();
glm::mat4 armMotion(glm::vec3 motionValue);
glm::mat4 legMotion(glm::vec3 motionValue);

GLvoid robotJump();
GLvoid touchWall();
GLboolean touchObstacle(float obstacle[]);
GLvoid robotMove();
GLvoid resetAll();
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

	robot.moveDir = 2;

	make_shaderProgram();
	InitBuffer_();
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKeyboard);
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
	box = translation_shape(glm::vec3(0.f, 0.f, 0.f));
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(box));
	glDrawArrays(GL_QUADS, 0, 20);

	glm::mat4 openLeftDoor = glm::mat4(1.0f);
	openLeftDoor = translation_shape(glm::vec3(-1.f, 0.f, 1.f)) * rotate_shape(open);
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(openLeftDoor));
	glDrawArrays(GL_QUADS, 20, 4);

	glm::mat4 openRightDoor = glm::mat4(1.0f);
	openRightDoor = translation_shape(glm::vec3(1.f, 0.f, 1.f)) * rotate_shape(-open);
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(openRightDoor));
	glDrawArrays(GL_QUADS, 24, 4);

	glm::mat4 robotHead = glm::mat4(1.0f);
	robotHead = translation_shape(robot.move) * rotate_shape(robot.dir);
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(robotHead));
	glDrawArrays(GL_QUADS, 28, 24);

	glm::mat4 robotBody = glm::mat4(1.0f);
	robotBody = translation_shape(robot.move) * rotate_shape(robot.dir);
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(robotBody));
	glDrawArrays(GL_QUADS, 52, 24);

	glm::mat4 robotRightArm = glm::mat4(1.0f);
	robotRightArm = translation_shape(robot.move) * rotate_shape(robot.dir) * armMotion(robot.right_motion);
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(robotRightArm));
	glDrawArrays(GL_QUADS, 76, 24);

	glm::mat4 robotLeftArm = glm::mat4(1.0f);
	robotLeftArm = translation_shape(robot.move) * rotate_shape(robot.dir) * armMotion(robot.left_motion);
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(robotLeftArm));
	glDrawArrays(GL_QUADS, 100, 24);

	glm::mat4 robotRightLeg = glm::mat4(1.0f);
	robotRightLeg = translation_shape(robot.move) * rotate_shape(robot.dir) * legMotion(robot.right_motion);
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(robotRightLeg));
	glDrawArrays(GL_QUADS, 124, 24);

	glm::mat4 robotLeftLeg = glm::mat4(1.0f);
	robotLeftLeg = translation_shape(robot.move) * rotate_shape(robot.dir) * legMotion(robot.left_motion);
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(robotLeftLeg));
	glDrawArrays(GL_QUADS, 148, 24);

	glm::mat4 nose = glm::mat4(1.0f);
	nose = translation_shape(robot.move) * rotate_shape(robot.dir);
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(nose));
	glDrawArrays(GL_TRIANGLES, 172, 9);

	glm::mat4 obstacle1 = glm::mat4(1.0f);
	obstacle1 = translation_shape(transObstacle);
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(obstacle1));
	glDrawArrays(GL_QUADS, 181, 20);

	glm::mat4 obstacle2 = glm::mat4(1.0f);
	obstacle2 = translation_shape(transObstacle1);
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(obstacle2));
	glDrawArrays(GL_QUADS, 181, 20);

	glm::mat4 obstacle3 = glm::mat4(1.0f);
	obstacle3 = translation_shape(transObstacle2);
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(obstacle3));
	glDrawArrays(GL_QUADS, 181, 20);

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

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'j':
		if (timeSwitch) 
			if (!jFlag) jFlag = TRUE;
		break;
	case 'w': case 'W':
		robot.dir.y = 180.f;
		robot.moveDir = 0;
		break;
	case 'a': case 'A':
		robot.dir.y = -90.f;
		robot.moveDir = 3;
		break;
	case 's': case 'S':
		robot.dir.y = 0.f;
		robot.moveDir = 2;
		break;
	case 'd': case 'D':
		robot.dir.y = 90.f;
		robot.moveDir = 1;
		break;
	case 'i':
		resetAll();
		break;
	case '+':
		if (speedValue < 0.3f) {
			speedValue += 0.001f;
			if(armLegMotionValue < 10.0f)armLegMotionValue += 0.01f;
		}
		break;
	case '-':
		if (speedValue > 0.002f) {
			speedValue -= 0.001f;
			if(armLegMotionValue > 0.02f)armLegMotionValue -= 0.01f;
		}
		break;
	case 'o':
		if (open.y > -90.f) {
			openValue = -1.f;

			if (!oFlag) oFlag = TRUE;


			if (!timeSwitch) {
				timeSwitch = TRUE;
				glutTimerFunc(10, TimerFunction, 1);
			}
		}
		break;
	case 'O':
		if (open.y < 0.f) {
			openValue = 1.f;

			if (!oFlag) oFlag = TRUE;


			if (!timeSwitch) {
				timeSwitch = TRUE;
				glutTimerFunc(10, TimerFunction, 1);
			}
		}
		break;
	case 'y':
		rotateValue = 1;

		if (!rFlag) rFlag = TRUE;

		if (!timeSwitch) {
			timeSwitch = TRUE;
			glutTimerFunc(10, TimerFunction, 1);
		}
		break;
	case 'Y':
		rotateValue = -1;

		if (!rFlag) rFlag = TRUE;

		if (!timeSwitch) {
			timeSwitch = TRUE;
			glutTimerFunc(10, TimerFunction, 1);
		}
		break;
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

	armLegMotion();
	robotMove();

	if (jFlag) robotJump();

	if (oFlag) {
		open.y += openValue;
		if (open.y == 0.f || open.y == -90.f) oFlag = FALSE;
	}
	if(rFlag) rotateCamera.y += rotateValue;

	if (timeSwitch) glutTimerFunc(10, TimerFunction, 1);
}

GLvoid cameraTranslation(glm::vec3 cameraTrans, glm::vec3 cameraRotate) {
	glm::vec3 zeroPoint = glm::vec3(0, 0, 0);
	glm::mat4 view = glm::mat4(1.0f);
	view = camera_locate(cameraTrans, zeroPoint) * rotate_camera(cameraRotate);

	unsigned int viewLocation = glGetUniformLocation(shaderProgramID, "view");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
}

GLvoid armLegMotion() {

	robot.left_motion.x += armLegMotionValue;
	robot.right_motion.x -= armLegMotionValue;

	if ((robot.left_motion.x > speedValue * 4500.f && alFlag) || (robot.left_motion.x < speedValue * -4500.f && !alFlag)) {
		armLegMotionValue *= -1;

		alFlag = (alFlag + 1) % 2;
	}
}

glm::mat4 armMotion(glm::vec3 motionValue) {
	glm::mat4 motion = glm::mat4(1.0f);
	motion = translation_shape(glm::vec3(0.f, -0.8f, 0.0f)) * rotate_shape(motionValue) * translation_shape(glm::vec3(0.f, 0.8f, 0.0f));
	return motion;
}

glm::mat4 legMotion(glm::vec3 motionValue) {
	glm::mat4 motion = glm::mat4(1.0f);
	motion = translation_shape(glm::vec3(0.f, -0.9f, 0.0f)) * rotate_shape(motionValue) * translation_shape(glm::vec3(0.f, 0.9f, 0.0f));
	return motion;
}

GLvoid robotJump() {
	robot.move.y += robotJumpValue;

	if (robot.move.y > 1.f)
		robotJumpValue *= -1;
	else if (robot.move.y < 0.f || ((touchObstacle(obstacle1) || touchObstacle(obstacle2) || touchObstacle(obstacle3)) && robot.move.y <= 0.2f)) {
		robot.move.y = 0.2f;
		robotJumpValue *= -1;
		jFlag = FALSE;
	}
}

GLvoid touchWall() {
	if (robot.move.x > 1.0f || robot.move.x < -1.0f || robot.move.z > 1.0f || robot.move.z < -1.0f) {
		robot.moveDir = (robot.moveDir + 2) % 4;
		robot.dir.y += 180.f;
		return;
	}
	else if (robot.move.y < 0.2f && (touchObstacle(obstacle1) || touchObstacle(obstacle2) || touchObstacle(obstacle3))) {
		robot.moveDir = (robot.moveDir + 2) % 4;
		robot.dir.y += 180.f;
		return;
	}

}

GLboolean touchObstacle(float obstacle[]) {
	if (robot.move.x > obstacle[2]) return FALSE;
	if (robot.move.x < obstacle[0]) return FALSE;
	if (robot.move.z > obstacle[3]) return FALSE;
	if (robot.move.z < obstacle[1]) return FALSE;

	return TRUE;
}

GLvoid robotMove() {
	switch (robot.moveDir) {
	case 0: // up
		robot.move.z -= speedValue;
		break;
	case 1: // right
		robot.move.x += speedValue;
		break;
	case 2: // down
		robot.move.z += speedValue;
		break;
	case 3: // left
		robot.move.x -= speedValue;
		break;
	}
	if(!jFlag && !(touchObstacle(obstacle1) || touchObstacle(obstacle2) || touchObstacle(obstacle3)))robot.move.y = 0;
	touchWall();
}

GLvoid resetAll() {
	robot.move = glm::vec3(0.f, 0.f, 0.f);
	robot.dir = glm::vec3(0.f, 0.f, 0.f);
	robot.moveDir = 2;
	robot.left_motion = glm::vec3(0.f, 0.f, 0.f);
	robot.right_motion = glm::vec3(0.f, 0.f, 0.f);

	transCamera = glm::vec3(0.0f, 0.0f, 2.0f);
	rotateCamera = glm::vec3(0, 0, 0);

	speedValue = 0.01f;
	armLegMotionValue = 5.0f;

	alFlag = TRUE;
}
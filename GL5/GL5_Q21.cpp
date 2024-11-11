#include "GL_My_header.h"
//========================================================
typedef struct {
	glm::vec3 move;
	glm::vec3 dir;
	glm::vec3 left_motion, right_motion;
}ROBOT;

ROBOT robot;
//========================================================
// ¿ø±Ù Åõ¿µ
PROJECTION projection = { 45.0f, 0.0f, 0.1f, 50.0f };
glm::vec3 spaceTrans = glm::vec3(0.0f, 0.0f, -2.0f);
//========================================================
char vertex[] = { "vertex.glsl" };
char fragment[] = { "fragment.glsl" };
GLuint vao, vbo[2]; 
GLuint shaderProgramID;
//========================================================
// »ç¿ëÀÚ ÁöÁ¤ º¯¼ö
float points[] = {
	-1.f, 1.f, -1.f,
	1.f, 1.f, -1.f,
	1.f, 1.f, 1.f,
	-1.f, 1.f, 1.f, // À­

	-1.f, 1.f, -1.f,
	-1.f, -1.f, -1.f,
	1.f, -1.f, -1.f,
	1.f, 1.f, -1.f, // µÞ

	-1.f, 1.f, -1.f,
	-1.f, 1.f, 1.f,
	-1.f, -1.f, 1.f,
	-1.f, -1.f, -1.f, // ¿Þ

	1.f, -1.f, -1.f,
	-1.f, -1.f, -1.f,
	-1.f, -1.f, 1.f, 
	1.f, -1.f, 1.f, // ¾Æ·¡

	1.f, 1.f, 1.f,
	1.f, 1.f, -1.f,
	1.f, -1.f, -1.f, 
	1.f, -1.f, 1.f, // ¿À

	0.f, 1.f, 0.f,
	0.f, -1.f, 0.f,
	1.f, -1.f, 0.f,
	1.f, 1.f, 0.f, // ÁÂ ¹®

	-1.f, 1.f, 0.f,
	-1.f, -1.f, 0.f,
	0.f, -1.f, 0.f,
	0.f, 1.f, 0.f, // ¿ì ¹®

	-0.05f, -0.7f, -0.05f,
	-0.05f, -0.7f, 0.05f,
	0.05f, -0.7f, 0.05f,
	0.05f, -0.7f, -0.05f, // ¸Ó¸® À§

	-0.05f, -0.7f, -0.05f,
	-0.05f, -0.8f, -0.05f,
	-0.05f, -0.8f, 0.05f,
	-0.05f, -0.7f, 0.05f, // ¸Ó¸® ¿ÞÂÊ

	-0.05f, -0.8f, -0.05f,
	-0.05f, -0.7f, -0.05f,
	0.05f, -0.7f, -0.05f,
	0.05f, -0.8f, -0.05f, // ¸Ó¸® µÚÂÊ

	0.05f, -0.7f, 0.05f,
	0.05f, -0.8f, 0.05f,
	0.05f, -0.8f, -0.05f,
	0.05f, -0.7f, -0.05f, // ¸Ó¸® ¿À¸¥ÂÊ

	-0.05f, -0.7f, 0.05f,
	-0.05f, -0.8f, 0.05f,
	0.05f, -0.8f, 0.05f,
	0.05f, -0.7f, 0.05f, // ¸Ó¸® ¾ÕÂÊ

	-0.05f, -0.8f, 0.05f,
	-0.05f, -0.8f, -0.05f,
	0.05f, -0.8f, -0.05f,
	0.05f, -0.8f, 0.05f, // ¸Ó¸® ¾Æ·¡ÂÊ

	-0.05f, -0.8f, -0.025f,
	-0.05f, -0.8f, 0.025f,
	0.05f, -0.8f, 0.025f,
	0.05f, -0.8f, -0.025f, // ¸öÅë À§

	-0.05f, -0.8f, -0.025f,
	-0.05f, -0.9f, -0.025f,
	-0.05f, -0.9f, 0.025f,
	-0.05f, -0.8f, 0.025f, // ¸öÅë ¿ÞÂÊ

	-0.05f, -0.9f, -0.025f,
	-0.05f, -0.8f, -0.025f,
	0.05f, -0.8f, -0.025f,
	0.05f, -0.9f, -0.025f, // ¸öÅë µÚÂÊ

	0.05f, -0.8f, 0.025f,
	0.05f, -0.9f, 0.025f,
	0.05f, -0.9f, -0.025f,
	0.05f, -0.8f, -0.025f, // ¸öÅë ¿À¸¥ÂÊ

	-0.05f, -0.8f, 0.025f,
	-0.05f, -0.9f, 0.025f,
	0.05f, -0.9f, 0.025f,
	0.05f, -0.8f, 0.025f, // ¸öÅë ¾ÕÂÊ

	-0.05f, -0.9f, 0.025f,
	-0.05f, -0.9f, -0.025f,
	0.05f, -0.9f, -0.025f,
	0.05f, -0.9f, 0.025f, // ¸öÅë ¾Æ·¡ÂÊ

	-0.07f, -0.8f, -0.025f,
	-0.07f, -0.8f, 0.025f,
	-0.05f, -0.8f, 0.025f,
	-0.05f, -0.8f, -0.025f, // ¿À¸¥ÆÈ À§

	-0.07f, -0.8f, -0.025f,
	-0.07f, -0.9f, -0.025f,
	-0.07f, -0.9f, 0.025f,
	-0.07f, -0.8f, 0.025f, // ¿À¸¥ÆÈ ¿ÞÂÊ

	-0.07f, -0.9f, -0.025f,
	-0.07f, -0.8f, -0.025f,
	-0.05f, -0.8f, -0.025f,
	-0.05f, -0.9f, -0.025f, // ¿À¸¥ÆÈ µÚÂÊ

	-0.05f, -0.8f, 0.025f,
	-0.05f, -0.9f, 0.025f,
	-0.05f, -0.9f, -0.025f,
	-0.05f, -0.8f, -0.025f, // ¿À¸¥ÆÈ ¿À¸¥ÂÊ

	-0.07f, -0.8f, 0.025f,
	-0.07f, -0.9f, 0.025f,
	-0.05f, -0.9f, 0.025f,
	-0.05f, -0.8f, 0.025f, // ¿À¸¥ÆÈ ¾ÕÂÊ

	-0.07f, -0.9f, 0.025f,
	-0.05f, -0.9f, -0.025f,
	-0.05f, -0.9f, -0.025f,
	-0.07f, -0.9f, 0.025f, // ¿À¸¥ÆÈ ¾Æ·¡ÂÊ

	0.07f, -0.8f, -0.025f,
	0.07f, -0.8f, 0.025f,
	0.05f, -0.8f, 0.025f,
	0.05f, -0.8f, -0.025f, // ¿ÞÆÈ À§

	0.07f, -0.8f, -0.025f,
	0.07f, -0.9f, -0.025f,
	0.07f, -0.9f, 0.025f,
	0.07f, -0.8f, 0.025f, // ¿ÞÆÈ ¿ÞÂÊ

	0.07f, -0.9f, -0.025f,
	0.07f, -0.8f, -0.025f,
	0.05f, -0.8f, -0.025f,
	0.05f, -0.9f, -0.025f, // ¿ÞÆÈ µÚÂÊ

	0.05f, -0.8f, 0.025f,
	0.05f, -0.9f, 0.025f,
	0.05f, -0.9f, -0.025f,
	0.05f, -0.8f, -0.025f, // ¿ÞÆÈ ¿À¸¥ÂÊ

	0.07f, -0.8f, 0.025f,
	0.07f, -0.9f, 0.025f,
	0.05f, -0.9f, 0.025f,
	0.05f, -0.8f, 0.025f, // ¿ÞÆÈ ¾ÕÂÊ

	0.07f, -0.9f, 0.025f,
	0.05f, -0.9f, -0.025f,
	0.05f, -0.9f, -0.025f,
	0.07f, -0.9f, 0.025f, // ¿ÞÆÈ ¾Æ·¡ÂÊ

	-0.05f, -0.9f, -0.025f,
	-0.05f, -0.9f, 0.025f,
	0.f, -0.9f, 0.025f,
	0.f, -0.9f, -0.025f, // ¿À¸¥´Ù¸® À§

	-0.05f, -0.9f, -0.025f,
	-0.05f, -1.f, -0.025f,
	-0.05f, -1.f, 0.025f,
	-0.05f, -0.9f, 0.025f, // ¿À¸¥´Ù¸® ¿ÞÂÊ

	-0.05f, -1.f, -0.025f,
	-0.05f, -0.9f, -0.025f,
	0.f, -0.9f, -0.025f,
	0.f, -1.f, -0.025f, // ¿À¸¥´Ù¸® µÚÂÊ

	0.f, -0.9f, 0.025f,
	0.f, -1.f, 0.025f,
	0.f, -1.f, -0.025f,
	0.f, -0.9f, -0.025f, // ¿À¸¥´Ù¸® ¿À¸¥ÂÊ

	-0.05f, -0.9f, 0.025f,
	-0.05f, -1.f, 0.025f,
	0.f, -1.f, 0.025f,
	0.f, -0.9f, 0.025f, // ¿À¸¥´Ù¸® ¾ÕÂÊ

	-0.05f, -1.f, 0.025f,
	-0.05f, -1.f, -0.025f,
	0.f, -1.f, -0.025f,
	0.f, -1.f, 0.025f, // ¿À¸¥´Ù¸® ¾Æ·¡ÂÊ

	0.f, -0.9f, -0.025f,
	0.f, -0.9f, 0.025f,
	0.05f, -0.9f, 0.025f,
	0.05f, -0.9f, -0.025f, // ¿Þ´Ù¸® À§

	0.f, -0.9f, -0.025f,
	0.f, -1.f, -0.025f,
	0.f, -1.f, 0.025f,
	0.f, -0.9f, 0.025f, // ¿Þ´Ù¸® ¿ÞÂÊ

	0.f, -1.f, -0.025f,
	0.f, -0.9f, -0.025f,
	0.05f, -0.9f, -0.025f,
	0.05f, -1.f, -0.025f, // ¿Þ´Ù¸® µÚÂÊ

	0.05f, -0.9f, 0.025f,
	0.05f, -1.f, 0.025f,
	0.05f, -1.f, -0.025f,
	0.05f, -0.9f, -0.025f, // ¿Þ´Ù¸® ¿À¸¥ÂÊ

	0.f, -0.9f, 0.025f,
	0.f, -1.f, 0.025f,
	0.05f, -1.f, 0.025f,
	0.05f, -0.9f, 0.025f, // ¿Þ´Ù¸® ¾ÕÂÊ

	0.f, -1.f, 0.025f,
	0.f, -1.f, -0.025f,
	0.05f, -1.f, -0.025f,
	0.05f, -1.f, 0.025f, // ¿Þ´Ù¸® ¾Æ·¡ÂÊ
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
	0, 1, 1
};

glm::vec3 transCamera = glm::vec3(0.0f, 0.0f, 2.0f);
glm::vec3 rotateCamera = glm::vec3(0, 0, 0);
glm::vec3 open = glm::vec3(0, 0, 0);

GLfloat rotateValue = 1.f;
GLfloat openValue;
GLfloat armLegMotionValue = 5.f;
GLfloat robotJumpValue = 0.05f;

GLboolean timeSwitch = FALSE;
GLboolean rFlag = FALSE;
GLboolean oFlag = FALSE;
GLboolean jFlag = FALSE;
//========================================================
GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid SpecialKeyboard(int key, int x, int y);
GLvoid TimerFunction(int value);
void make_shaderProgram();
void InitBuffer_();
//========================================================
// »ç¿ëÀÚ ÁöÁ¤ ÇÔ¼ö
GLvoid cameraTranslation(glm::vec3 cameraTrans, glm::vec3 cameraRotate);

GLvoid armLegMotion();
glm::mat4 armMotion(glm::vec3 motionValue);
glm::mat4 legMotion(glm::vec3 motionValue);

GLvoid robotJump();
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
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKeyboard);
	glutMainLoop();
}

GLvoid drawScene()
{
	glEnable(GL_DEPTH_TEST);

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

	if (robot.left_motion.x > 45.f || robot.left_motion.x < -45.f) armLegMotionValue *= -1;
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
	else if (robot.move.y < 0.f) {
		robotJumpValue *= -1;
		jFlag = FALSE;
	}

	std::cout << robot.move.y << std::endl;
}

GLvoid robotMove() {

}
#include "GL_My_header.h"
//========================================================
#define LENGTH 0.2f

typedef struct {
	glm::vec3 scale;
	glm::vec3 trans;
	glm::vec3 rotate;
}TOP;

typedef struct {
	glm::vec3 trans;
}BOTTOM;

typedef struct {
	glm::vec3 scale;
	glm::vec3 trans;
	glm::vec3 rotate;
}BARREL;

typedef struct {
	glm::vec3 scale;
	glm::vec3 trans;
	glm::vec3 rotate;
}ARM;

TOP top;
BOTTOM bottom;
BARREL barrel[2];
ARM arm[2];
//========================================================
// 원근 투영
PROJECTION projection = { 45.0f, 0.0f, 0.1f, 50.0f };
glm::vec3 spaceTrans = glm::vec3(0.0f, 0.0f, -5.0f); 
// 좌표 평면
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
// 바닥
glm::vec3 floorPoint[4] = {
	glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(-1.0f, 0.0f, 1.0f),
	glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, -1.0f)
};
glm::vec3 floorColor[4] = {
	glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), 
	glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f)
};
GLuint floorIndex[2][3] = { {0, 1, 2}, {0, 2, 3}};
GLsizei floorVertex = 6;
// 큐브
glm::vec3 cubePoint[8] = {
	{glm::vec3(-LENGTH, LENGTH, -LENGTH)},{glm::vec3(LENGTH, LENGTH, -LENGTH)},
	{glm::vec3(-LENGTH, LENGTH, LENGTH)},{glm::vec3(LENGTH, LENGTH, LENGTH)},
	{glm::vec3(-LENGTH, 0.0f, -LENGTH)},{glm::vec3(-LENGTH, 0.0f, LENGTH)},
	{glm::vec3(LENGTH, 0.0f, LENGTH)},{glm::vec3(LENGTH, 0.0f, -LENGTH)},
};
glm::vec3 cubeColor[8] = {
	{glm::vec3(1.0f, 0.0f, 0.0f)},{glm::vec3(1.0f, 1.0f, 0.0f)},{glm::vec3(0.0f, 1.0f, 0.0f)},{glm::vec3(0.0f, 1.0f, 1.0f)},
	{glm::vec3(0.0f, 0.0f, 1.0f)},{glm::vec3(1.0f, 0.0f, 1.0f)},{glm::vec3(1.0f, 0.0f, 0.0f)},{glm::vec3(1.0f, 1.0f, 0.0f)}
};
GLuint cubeIndex[12][3] = {
	{ 0, 2, 1 }, { 1, 2, 3 },
	{ 4, 5, 0 }, { 0, 5, 2 },
	{ 4, 0, 7 }, { 7, 0, 1 },
	{ 2, 5, 3 }, { 3, 5, 6 },
	{ 1, 3, 7 }, { 7, 3, 6 },
	{ 7, 6, 4 }, { 4, 6, 5 }
};
GLsizei cubeVertex = 36;
// 좌표 평면 회전
glm::vec3 shapeDegree = glm::vec3(30.0f, 0.0f, 0.0f);
//========================================================
char vertex[] = { "vertex.glsl" };
char fragment[] = { "fragment.glsl" };
GLuint VAO, VBO, VBO_pos, EBO;
GLuint shaderProgramID;
//========================================================
// 사용자 지정 변수
GLfloat bottomTransValue = 0.025f;
GLfloat topDegreeValue = 1.0f;
GLfloat barrelDegreeValue = 1.0f;
GLfloat barrelTransValue = 0.01f;
GLfloat armDegreeValue = 1.0f;
GLfloat cameraRotValue, cameraRevValue;

GLboolean timeSwitch = FALSE; 
GLboolean bottomTrans = FALSE;
GLboolean topRotate = FALSE;
GLboolean barrelRotate = FALSE;
GLboolean barrelTrans = FALSE;
GLboolean armRotate = FALSE;
GLboolean armPo, armNe;
GLboolean bottomPo, bottomNe;
GLboolean barrelPo;
GLboolean viewRotate, viewRevolution;

glm::vec3 cameraMove = glm::vec3(0.0f, 0.0f, 2.0f);
glm::vec3 cameraRotate = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraRevolution = glm::vec3(0.0f, 0.0f, 0.0f);
//========================================================
GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid SpecialKeyboard(int key, int x, int y);
GLvoid TimerFunction(int value);
void make_shaderProgram();
void InitBuffer_EBO_VEC(glm::vec3* vPositionList, GLuint index[][3], glm::vec3* color,
	GLuint vSize, GLuint iSize, GLuint cSize);
//========================================================
// 사용자 지정 함수
GLvoid Draw_Coordinate_Plane();
GLvoid DrawFloor();
GLvoid DrawCube();
GLvoid DrawTop();
GLvoid DrawArm(ARM arm); 
GLvoid DrawBarrel(BARREL barrel); 

GLvoid initialState(); 

GLvoid transBottom(); 
GLvoid rotateTop(); 
GLvoid rotateBarrel(); 
GLvoid transBarrel();
GLvoid rotateArm(); 

glm::mat4 initialLocate(glm::vec3 scale, glm::vec3 trans);
GLvoid cameraTranslation(glm::vec3 cameraMove, glm::vec3 cameraRotate, glm::vec3 cameraRevolution); 

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

	initialState();

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

	cameraTranslation(cameraMove, cameraRotate, cameraRevolution);

	Perspective_Projection_Transformation(projection, spaceTrans, shaderProgramID);

	Draw_Coordinate_Plane();
	DrawFloor();
	DrawCube();
	DrawTop();
	for (int i = 0; i < 2; i++) {
		DrawArm(arm[i]);
		DrawBarrel(barrel[i]);
	}

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

void InitBuffer_EBO_VEC(glm::vec3* vPositionList, GLuint index[][3], glm::vec3* color, GLuint vSize, GLuint iSize, GLuint cSize)
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
	case 'b':
		if (!bottomTrans || !bottomPo) {
			bottomTransValue = 0.025f;
			bottomPo = TRUE;
			bottomNe = FALSE;
			bottomTrans = TRUE;

			if (!timeSwitch) {
				timeSwitch = TRUE;
				glutTimerFunc(10, TimerFunction, 1);
			}
		}
		else {
			bottomTrans = FALSE;
		}


		break;
	case 'B':
		if (!bottomTrans || !bottomNe) {
			bottomTransValue = -0.025f;
			bottomPo = FALSE;
			bottomNe = TRUE;
			bottomTrans = TRUE;

			if (!timeSwitch) {
				timeSwitch = TRUE;
				glutTimerFunc(10, TimerFunction, 1);
			}
		}
		else {
			bottomTrans = FALSE;
		}
		break;
	case 'm':
		if (!topRotate || topDegreeValue < 0) {
			topDegreeValue = 1.0f;

			topRotate = TRUE;

			if (!timeSwitch) {
				timeSwitch = TRUE;
				glutTimerFunc(10, TimerFunction, 1);
			}
		}
		else {
			topRotate = FALSE;
		}

			
		break;
	case 'M':
		if (!topRotate || topDegreeValue > 0) {
			topDegreeValue = -1.0f;

			topRotate = TRUE;

			if (!timeSwitch) {
				timeSwitch = TRUE;
				glutTimerFunc(10, TimerFunction, 1);
			}
		}
		else {
			topRotate = FALSE;
		}
		break;
	case 'f':
		if (!barrelRotate || barrelDegreeValue < 0) {
			barrelDegreeValue = 1.0f;

			barrelRotate = TRUE;

			if (!timeSwitch) {
				timeSwitch = TRUE;
				glutTimerFunc(10, TimerFunction, 1);
			}
		}
		else {
			barrelRotate = FALSE;
		}


		break;
	case 'F':
		if (!barrelRotate || barrelDegreeValue > 0) {
			barrelDegreeValue = -1.0f;

			barrelRotate = TRUE;

			if (!timeSwitch) {
				timeSwitch = TRUE;
				glutTimerFunc(10, TimerFunction, 1);
			}
		}
		else {
			barrelRotate = FALSE;
		}
		break;
	case 'e':
		if (!barrelTrans || barrelTransValue < 0) {
			barrelRotate = FALSE;
			barrelTrans = TRUE;
			barrelPo = TRUE;
			if (!timeSwitch) {
				timeSwitch = TRUE;
				glutTimerFunc(10, TimerFunction, 1);
			}
		}


		break;
	case 'E':
		if (!barrelTrans || barrelTransValue > 0) {
			barrelRotate = FALSE;
			barrelTrans = TRUE;
			barrelPo = FALSE;
			if (!timeSwitch) {
				timeSwitch = TRUE;
				glutTimerFunc(10, TimerFunction, 1);
			}
		}

		break;
	case 't':
		if (!armRotate || !armPo) {
			armDegreeValue = 1.0f;
			armPo = TRUE;
			armNe = FALSE;
			armRotate = TRUE;

			if (!timeSwitch) {
				timeSwitch = TRUE;
				glutTimerFunc(10, TimerFunction, 1);
			}
		}
		else {
			armRotate = FALSE;
		}


		break;
	case 'T':
		if (!armRotate || !armNe) {
			armDegreeValue = -1.0f;
			armPo = FALSE;
			armNe = TRUE;
			armRotate = TRUE;

			if (!timeSwitch) {
				timeSwitch = TRUE;
				glutTimerFunc(10, TimerFunction, 1);
			}
		}
		else {
			armRotate = FALSE;
		}
		break;
	case 's': case 'S':
		timeSwitch = FALSE;
		bottomTrans = FALSE;
		topRotate = FALSE;
		barrelRotate = FALSE;
		barrelTrans = FALSE;
		armRotate = FALSE;
		armPo = armNe = FALSE;
		bottomPo = bottomNe = FALSE;
		barrelPo = FALSE;
		break;
	case 'c' : case 'C':
		timeSwitch = FALSE;
		bottomTrans = FALSE;
		topRotate = FALSE;
		barrelRotate = FALSE;
		barrelTrans = FALSE;
		armRotate = FALSE;
		armPo = armNe = FALSE;
		bottomPo = bottomNe = FALSE;
		barrelPo = FALSE;

		initialState();
		break;
	case 'z':
		cameraMove.z += 1.0f;
		break;
	case 'Z':
		cameraMove.z -= 1.0f;
		break;
	case 'x':
		cameraMove.x += 1.0f;
		break;
	case 'X':
		cameraMove.x -= 1.0f;
		break;
	case 'y':
		cameraRotValue = 1.0f;
		viewRotate = TRUE;

		if (!timeSwitch) {
			timeSwitch = TRUE;
			glutTimerFunc(10, TimerFunction, 1);
		}
		break;
	case 'Y':
		cameraRotValue = -1.0f;
		viewRotate = TRUE;

		if (!timeSwitch) {
			timeSwitch = TRUE;
			glutTimerFunc(10, TimerFunction, 1);
		}
		break;
	case 'r':
		cameraRevValue = 1.0f;
		viewRevolution = TRUE;

		if (!timeSwitch) {
			timeSwitch = TRUE;
			glutTimerFunc(10, TimerFunction, 1);
		}
		break;
	case 'R':
		cameraRevValue = -1.0f;
		viewRevolution = TRUE;

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

	if (bottomTrans) transBottom();
	if(topRotate) rotateTop();
	if (barrelRotate) rotateBarrel();
	if (barrelTrans) transBarrel();
	if (armRotate) rotateArm();

	if (viewRotate) cameraRotate.y += cameraRotValue;
	if (viewRevolution)cameraRevolution.y += cameraRevValue;

	if (timeSwitch) glutTimerFunc(10, TimerFunction, 1);
}

GLvoid Draw_Coordinate_Plane() {
	for (int i = 0; i < 3; i++) {
		unsigned int transformLocation = glGetUniformLocation(shaderProgramID, "model");
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(rotate_shape(shapeDegree)));

		InitBuffer_EBO_VEC(line[i], line_index, lineColor[i], sizeof(line[i]), sizeof(line_index), sizeof(lineColor[i]));
		glBindVertexArray(VAO);

		glDrawElements(GL_LINES, line_vertex, GL_UNSIGNED_INT, 0);
	}
}

GLvoid DrawFloor() {
	unsigned int transformLocation = glGetUniformLocation(shaderProgramID, "model");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(rotate_shape(shapeDegree)));

	InitBuffer_EBO_VEC(floorPoint, floorIndex, floorColor, sizeof(floorPoint), sizeof(floorIndex), sizeof(floorColor));
	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES, floorVertex, GL_UNSIGNED_INT, 0); 
}

GLvoid DrawCube() {
	unsigned int transformLocation = glGetUniformLocation(shaderProgramID, "model");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(rotate_shape(shapeDegree) * translation_shape(bottom.trans)));

	InitBuffer_EBO_VEC(cubePoint, cubeIndex, cubeColor, sizeof(cubePoint), sizeof(cubeIndex), sizeof(cubeColor));
	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES, cubeVertex, GL_UNSIGNED_INT, 0);
}

GLvoid DrawTop() {
	glm::mat4 Matrix = glm::mat4(1.0f);
	Matrix = rotate_shape(shapeDegree) * translation_shape(bottom.trans) * initialLocate(top.scale, top.trans) * rotate_shape(top.rotate);

	unsigned int transformLocation = glGetUniformLocation(shaderProgramID, "model");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(Matrix));

	InitBuffer_EBO_VEC(cubePoint, cubeIndex, cubeColor, sizeof(cubePoint), sizeof(cubeIndex), sizeof(cubeColor));
	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES, cubeVertex, GL_UNSIGNED_INT, 0);
}

GLvoid DrawArm(ARM arm) {
	glm::mat4 Matrix = glm::mat4(1.0f);
	Matrix = rotate_shape(shapeDegree) * translation_shape(bottom.trans) * rotate_shape(top.rotate) * translation_shape(arm.trans) * rotate_shape(arm.rotate) * scaling_shape(arm.scale);

	unsigned int transformLocation = glGetUniformLocation(shaderProgramID, "model");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(Matrix));

	InitBuffer_EBO_VEC(cubePoint, cubeIndex, cubeColor, sizeof(cubePoint), sizeof(cubeIndex), sizeof(cubeColor));
	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES, cubeVertex, GL_UNSIGNED_INT, 0);
}

GLvoid DrawBarrel(BARREL barrel) {
	glm::mat4 Matrix = glm::mat4(1.0f);
	Matrix = rotate_shape(shapeDegree) * translation_shape(bottom.trans) * rotate_shape(barrel.rotate) * translation_shape(barrel.trans) * scaling_shape(barrel.scale);

	unsigned int transformLocation = glGetUniformLocation(shaderProgramID, "model");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(Matrix));

	InitBuffer_EBO_VEC(cubePoint, cubeIndex, cubeColor, sizeof(cubePoint), sizeof(cubeIndex), sizeof(cubeColor));
	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES, cubeVertex, GL_UNSIGNED_INT, 0);
}

GLvoid initialState() {
	glm::vec3 zero = glm::vec3(0.0f, 0.0f, 0.0f);

	top.scale = glm::vec3(0.7f, 0.7f, 0.7f);
	top.trans = glm::vec3(0.0f, LENGTH, 0.0f);
	top.rotate = zero;

	bottom.trans = zero;

	arm[0].scale = arm[1].scale = glm::vec3(0.15f, 0.8f, 0.15f);
	arm[0].trans = glm::vec3(0.1f, LENGTH * 1.5, 0.0f);
	arm[1].trans = glm::vec3(-0.1f, LENGTH * 1.5, 0.0f);
	arm[0].rotate = arm[1].rotate = zero;

	barrel[0].scale = barrel[1].scale = glm::vec3(0.15f, 0.1f, 1.0f);
	barrel[0].trans = glm::vec3(-0.15f, 0.0f, LENGTH);
	barrel[1].trans = glm::vec3(0.15f, 0.0f, LENGTH);
	barrel[0].rotate = barrel[1].rotate = zero;
}

GLvoid transBottom() {
	bottom.trans.x += bottomTransValue;

	if (bottom.trans.x > 0.8f || bottom.trans.x < -0.8f)
		bottomTransValue *= -1;
}

GLvoid rotateTop() {
	top.rotate.y += topDegreeValue;
}

GLvoid rotateBarrel() {
	barrel[0].rotate.y += barrelDegreeValue;
	barrel[1].rotate.y -= barrelDegreeValue;

	if (barrel[0].rotate.y > 359.0f) {
		barrel[0].rotate.y = 0.0f;
		barrel[1].rotate.y = 0.0f;
	}
}

GLvoid transBarrel() {
	if (barrel[0].rotate.y != 0.0f) {
		rotateBarrel();
	}
	else {
		if (barrelPo) {
			barrel[0].trans.x += barrelTransValue;
			barrel[1].trans.x -= barrelTransValue;
			if (barrel[0].trans.x > 0.0f) {
				barrel[0].trans.x = 0.0f;
				barrel[1].trans.x = 0.0f;
				barrelTrans = FALSE;
				barrelPo = FALSE;
			}
		}
		else {
			barrel[0].trans.x -= barrelTransValue;
			barrel[1].trans.x += barrelTransValue;
			if (barrel[0].trans.x < -0.15f) {
				barrel[0].trans.x = -0.15f;
				barrel[1].trans.x = 0.15f;
				barrelTrans = FALSE;
				barrelPo = TRUE;
			}
		}
	}
}

GLvoid rotateArm() {
	arm[0].rotate.z += armDegreeValue;
	arm[1].rotate.z -= armDegreeValue;

	if (arm[0].rotate.z > 90.0f || arm[0].rotate.z < -90.0f)
		armDegreeValue *= -1;
}

glm::mat4 initialLocate(glm::vec3 scale, glm::vec3 trans) {
	glm::mat4 Matrix = glm::mat4(1.0f);
	Matrix = translation_shape(trans) * scaling_shape(scale);

	return Matrix;
}

GLvoid cameraTranslation(glm::vec3 cameraMove, glm::vec3 cameraRotate, glm::vec3 cameraRevolution) {
	glm::vec3 zero = glm::vec3(0.0f, 0.0f, -5.0f) - cameraMove;
	glm::vec3 pos = glm::vec3(0.0f, 0.0f, 5.0f) + cameraMove;
	
	glm::mat4 view = glm::mat4(1.0f);
	view = camera_locate(cameraMove, glm::vec3(0.f, 0.f, 0.f)) *  rotate_camera(cameraRevolution)
		* trans_camera(pos) * rotate_camera(cameraRotate) * trans_camera(zero);

	unsigned int viewLocation = glGetUniformLocation(shaderProgramID, "view");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]); 
}
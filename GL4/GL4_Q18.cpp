#include "GL_My_header.h"
//========================================================
#define STAR_SIZE 0.4f
#define PLANET_SIZE 0.15f
#define MOON_SIZE 0.05f
#define PI 3.141592f


typedef struct {
	glm::vec3 trans; // 행성으로부터의 거리
	glm::vec3 revolution; // 공전
	glm::vec3 obitalOrbit; // 달 공전궤도
	glm::vec3 color; // 달의 색
}MOON;

typedef struct {
	glm::vec3 trans; // 행성 위치
	glm::vec3 revolution; // 행성 공전
	glm::vec3 obitalOrbit; // 행성 공전궤도
	glm::vec3 color; // 행성 색

	MOON moon;
}PLANET;

typedef struct {
	glm::vec3 trans; // 항성 위치
	glm::vec3 rotation; // 항성 자전
	glm::vec3 color; // 항성 색

	PLANET planet[3];
}STAR;

//========================================================
// 원근 투영
PROJECTION projection = { 45.0f, 0.0f, 0.1f, 50.0f };
glm::vec3 spaceTrans = glm::vec3(0.0f, 0.0f, -5.0f);
//========================================================
char vertex[] = { "vertex_Q18.glsl" };
char fragment[] = { "fragment.glsl" };
GLuint VAO, VBO, VBO_pos, EBO;
GLuint shaderProgramID;
//========================================================
// 사용자 지정 변수
GLUquadric* qobj;

STAR star;

GLboolean timeSwitch = FALSE;
GLboolean projectionSwitch = TRUE;
GLboolean LineOrFace = FALSE; 
GLboolean galacticSystem_y = FALSE;
GLboolean galacticSystem_z = FALSE;

// 항성을 제외한 은하계
glm::vec3 galacticSystemTrans = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 galacticSystemRotate = glm::vec3(0.0f, 0.0f, 0.0f);
// 항성
glm::vec3 starTrans = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 starRotate = glm::vec3(0.0f, 0.0f, 0.0f);

GLfloat yValue = 1.0f;
GLfloat zValue = 1.0f;
//========================================================
GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid SpecialKeyboard(int key, int x, int y);
GLvoid TimerFunction(int value);
void make_shaderProgram();
void InitBuffer_EBO_VEC(glm::vec3* color, GLuint cSize);
//========================================================
// 사용자 지정 함수
GLvoid resetStar();
GLvoid drawObitalOrbit(PLANET planet, GLfloat distance, GLboolean use); 
GLvoid drawStar();
GLvoid drawPlanet(PLANET* planet);
GLvoid drawMoon(PLANET* planet); 

glm::mat4 rotation(glm::vec3 rotatePlanet);
glm::mat4 revolution(glm::vec3 revolutionPlanet, glm::vec3 obitalOrbit, glm::vec3 locate); 
glm::mat4 obitalOrbit_rotate(glm::vec3 degree);
glm::mat4 galacticSystem(glm::vec3 trans, glm::vec3 rotate); 
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

	star.color = glm::vec3(1.0f, 0.5f, 0.5f);
	resetStar();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKeyboard);
	glutTimerFunc(10, TimerFunction, 1);
	glutMainLoop();
}

GLvoid drawScene()
{
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgramID);

	if (projectionSwitch) Perspective_Projection_Transformation(projection, spaceTrans, shaderProgramID);
	else Orthogoanl_Projection_Transformation(2.0f, shaderProgramID);

	drawStar(); 

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
	case 'p':
		projectionSwitch = FALSE;
		break;
	case 'P':
		projectionSwitch = TRUE;
		break;
	case 'm':
		LineOrFace = FALSE;
		break;
	case 'M':
		LineOrFace = TRUE;
		break;
	case 'w':
		galacticSystemTrans.y += 0.2f;
		starTrans.y += 0.2f;
		break;
	case 'a':
		galacticSystemTrans.x -= 0.2f;
		starTrans.x -= 0.2f;
		break;
	case 's':
		galacticSystemTrans.y -= 0.2f;
		starTrans.y -= 0.2f;
		break;
	case 'd':
		galacticSystemTrans.x += 0.2f;
		starTrans.x += 0.2f;
		break;
	case '+':
		galacticSystemTrans.z += 0.2f;
		starTrans.z += 0.2f;
		break;
	case '-':
		galacticSystemTrans.z -= 0.2f;
		starTrans.z -= 0.2f;
		break;
	case 'y':
		if (!galacticSystem_y) galacticSystem_y = TRUE;
		yValue = 1.0f;
		break;
	case 'Y':
		if (!galacticSystem_y) galacticSystem_y = TRUE;
		yValue = -1.0f;
		break;
	case 'z':
		if (!galacticSystem_z) galacticSystem_z = TRUE;
		zValue = 1.0f;
		break;
	case 'Z':
		if (!galacticSystem_z) galacticSystem_z = TRUE;
		zValue = -1.0f;
		break;
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

	star.planet[0].revolution.y += 1.0f;
	star.planet[0].moon.revolution.y += 1.0f;
	star.planet[0].moon.obitalOrbit.y -= 1.0f;

	star.planet[1].revolution.y -= 1.2f;
	star.planet[1].moon.revolution.y += 5.0f;
	star.planet[1].moon.obitalOrbit.y += 1.2f;

	star.planet[2].revolution.y += 0.7f;
	star.planet[2].moon.revolution.y -= 3.0f;

	if (galacticSystem_y) {
		galacticSystemRotate.y += yValue;
		starRotate.y += yValue;
	}
	if (galacticSystem_z) galacticSystemRotate.z += zValue;

	glutTimerFunc(10, TimerFunction, 1);
}

void InitBuffer_EBO_VEC(glm::vec3* color, GLuint cSize)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO_pos);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_pos);
	glBufferData(GL_ARRAY_BUFFER, NULL, NULL, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, NULL, NULL, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, cSize, color, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
}

GLvoid drawObitalOrbit(PLANET planet, GLfloat distance, GLboolean use) {
	GLfloat x, z;
	GLint degree = 0;

	if (use) {
		GLuint transformLocation = glGetUniformLocation(shaderProgramID, "model");
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, 
			glm::value_ptr(galacticSystem(galacticSystemTrans, galacticSystemRotate) * rotation(planet.obitalOrbit))
		);
	}
	else {
		GLuint transformLocation = glGetUniformLocation(shaderProgramID, "model");
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, 
			glm::value_ptr(galacticSystem(galacticSystemTrans, galacticSystemRotate) * revolution(planet.revolution, planet.obitalOrbit, planet.trans) * obitalOrbit_rotate(planet.moon.obitalOrbit))
		);
	}

	GLuint colorLocation = glGetUniformLocation(shaderProgramID, "color");
	glUniform3f(colorLocation, 0.0f, 0.0f, 0.0f);

	glBegin(GL_LINE_STRIP);
	while (degree < 361) {
		x = distance * sinf(degree * PI / 180);
		z = distance * cosf(degree * PI / 180);

		degree += 1;

		glPointSize(10);
		glVertex3f(x, 0, z);
	}
	glEnd();
}

GLvoid drawStar() {
	GLuint transformLocation = glGetUniformLocation(shaderProgramID, "model"); 
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, 
		glm::value_ptr(galacticSystem(starTrans, starRotate) * rotation(star.rotation))
	);

	GLuint colorLocation = glGetUniformLocation(shaderProgramID, "color");
	glUniform3f(colorLocation, star.color.x, star.color.y, star.color.z);

	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, LineOrFace ? GLU_LINE : GLU_FILL);
	gluSphere(qobj, STAR_SIZE, 50, 50); 

	for (int i = 0; i < 3; i++) {
		drawPlanet(&star.planet[i]);
		drawObitalOrbit(star.planet[i], 1.5f, TRUE);
	}
}

GLvoid drawPlanet(PLANET* planet) { 
	glm::mat4 Matrix = glm::mat4(1.0f);
	Matrix = galacticSystem(galacticSystemTrans, galacticSystemRotate) * revolution(planet->revolution, planet->obitalOrbit, planet->trans) * rotation(star.rotation); 

	GLuint transformLocation = glGetUniformLocation(shaderProgramID, "model");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(Matrix));

	GLuint colorLocation = glGetUniformLocation(shaderProgramID, "color");
	glUniform3f(colorLocation, planet->color.x, planet->color.y, planet->color.z);

	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, LineOrFace ? GLU_LINE : GLU_FILL); 
	gluSphere(qobj, PLANET_SIZE, 50, 50);

	drawObitalOrbit(*planet, 0.5f, FALSE); 

	drawMoon(planet);
}

GLvoid drawMoon(PLANET* planet) {
	glm::mat4 Matrix = glm::mat4(1.0f); 
	Matrix = galacticSystem(galacticSystemTrans, galacticSystemRotate) * revolution(planet->revolution, planet->obitalOrbit, planet->trans) 
		* revolution(planet->moon.revolution, planet->moon.obitalOrbit, planet->moon.trans) * rotation(star.rotation); 

	GLuint transformLocation = glGetUniformLocation(shaderProgramID, "model");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(Matrix));

	GLuint colorLocation = glGetUniformLocation(shaderProgramID, "color");
	glUniform3f(colorLocation, planet->moon.color.x, planet->moon.color.y, planet->moon.color.z);

	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, LineOrFace ? GLU_LINE : GLU_FILL); 
	gluSphere(qobj, MOON_SIZE, 50, 50);
}

GLvoid resetStar() {
	star.color = glm::vec3(1.0f, 0.0f, 0.0f);
	star.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	star.trans = glm::vec3(0.0f, 0.0f, 0.0f);

	green_color(&star.planet[0].color); 
	star.planet[0].trans = glm::vec3(1.5f, 0.0f, 0.0f); 
	star.planet[0].obitalOrbit = glm::vec3(10.0f, 0.0f, 45.0f);
	yellow_color(&star.planet[0].moon.color);
	star.planet[0].moon.trans = glm::vec3(0.5f, 0.0f, 0.0f);
	star.planet[0].moon.revolution = glm::vec3(0.0f, 0.0f, 0.0f);
	star.planet[0].moon.obitalOrbit = glm::vec3(0.0f, 0.0f, -45.0f);

	blue_color(&star.planet[1].color); 
	star.planet[1].trans = glm::vec3(-1.5f, 0.0f, 0.0f);
	star.planet[1].obitalOrbit = glm::vec3(10.0f, 0.0f, -45.0f);
	yellow_color(&star.planet[1].moon.color);
	star.planet[1].moon.trans = glm::vec3(0.5f, 0.0f, 0.0f);
	star.planet[1].moon.revolution = glm::vec3(0.0f, 0.0f, 0.0f);
	star.planet[1].moon.obitalOrbit = glm::vec3(0.0f, 0.0f, 45.0f);

	red_color(&star.planet[2].color);
	star.planet[2].trans = glm::vec3(0.0f, 0.0f, 1.5f);
	star.planet[2].obitalOrbit = glm::vec3(10.0f, 0.0f, 0.0f);
	yellow_color(&star.planet[2].moon.color);
	star.planet[2].moon.trans = glm::vec3(0.5f, 0.0f, 0.0f);
	star.planet[2].moon.revolution = glm::vec3(0.0f, 0.0f, 0.0f);
	star.planet[2].moon.obitalOrbit = glm::vec3(0.0f, 0.0f, 0.0f);
}

// 항성/행성/달 자전
glm::mat4 rotation(glm::vec3 rotatePlanet) {
	glm::mat4 Matrix = glm::mat4(1.0f);
	Matrix = rotate_shape(rotatePlanet);

	return Matrix;
}

// y축 기준 행성 공전
glm::mat4 revolution(glm::vec3 revolutionPlanet, glm::vec3 obitalOrbit, glm::vec3 locate) {
	glm::mat4 Matrix = glm::mat4(1.0f);
	// 공전 궤도, 공전
	Matrix = obitalOrbit_rotate(obitalOrbit) * rotate_shape(revolutionPlanet) * translation_shape(locate);

	return Matrix;
}

// 공전 궤도 조정
glm::mat4 obitalOrbit_rotate(glm::vec3 degree) {
	glm::mat4 rotateMatrix_x(1.0f);
	rotateMatrix_x = glm::rotate(rotateMatrix_x, glm::radians(degree.x), glm::vec3(1.0f, 0.0f, 0.0f)); //--- X축 회전

	glm::mat4 rotateMatrix_y(1.0f);
	rotateMatrix_y = glm::rotate(rotateMatrix_y, glm::radians(degree.y), glm::vec3(0.0f, 1.0f, 0.0f)); //--- Y축 회전

	glm::mat4 rotateMatrix_z(1.0f);
	rotateMatrix_z = glm::rotate(rotateMatrix_z, glm::radians(degree.z), glm::vec3(0.0f, 0.0f, 1.0f)); //--- Z축 회전

	glm::mat4 rotateMatrix(1.0f);
	rotateMatrix = rotateMatrix_y * rotateMatrix_z * rotateMatrix_x;

	return rotateMatrix;
}

glm::mat4 galacticSystem(glm::vec3 trans, glm::vec3 rotate) {
	glm::mat4 Matrix = glm::mat4(1.0f);
	Matrix = translation_shape(trans) * rotate_shape(rotate);

	return Matrix;
}
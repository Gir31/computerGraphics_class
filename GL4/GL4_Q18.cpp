#include "GL_My_header.h"
//========================================================
#define STAR_SIZE 0.4f
#define PLANET_SIZE 0.15f
#define MOON_SIZE 0.05f


typedef struct {
	glm::vec3 trans; // �༺���κ����� �Ÿ�
	glm::vec3 revolution; // ����
	glm::vec3 obitalOrbit; // �� �����˵�
	glm::vec3 color; // ���� ��
}MOON;

typedef struct {
	glm::vec3 trans; // �༺ ��ġ
	glm::vec3 revolution; // �༺ ����
	glm::vec3 obitalOrbit; // �༺ �����˵�
	glm::vec3 color; // �༺ ��

	MOON moon;
}PLANET;

typedef struct {
	glm::vec3 trans; // �׼� ��ġ
	glm::vec3 rotation; // �׼� ����
	glm::vec3 color; // �׼� ��

	PLANET planet[3];
}STAR;

STAR star;
//========================================================
// ���� ����
PROJECTION projection = { 45.0f, 0.0f, 0.1f, 50.0f };
glm::vec3 spaceTrans = glm::vec3(0.0f, 0.0f, -5.0f);
//========================================================
char vertex[] = { "vertex_Q18.glsl" };
char fragment[] = { "fragment.glsl" };
GLuint VAO, VBO, VBO_pos, EBO;
GLuint shaderProgramID;
//========================================================
// ����� ���� ����
GLUquadric* qobj;


GLboolean timeSwitch = FALSE;
GLboolean projectionSwitch = TRUE;
//========================================================
GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid SpecialKeyboard(int key, int x, int y);
GLvoid TimerFunction(int value);
void make_shaderProgram();
void InitBuffer_EBO_VEC(glm::vec3* color, GLuint cSize);
//========================================================
// ����� ���� �Լ�
GLvoid resetStar();
GLvoid drawStar();
GLvoid drawPlanet(PLANET* planet);
GLvoid drawMoon(PLANET* planet); 

glm::mat4 rotation(glm::vec3 transPlanet, glm::vec3 rotatePlanet);
glm::mat4 revolution(glm::vec3 revolutionPlanet, glm::vec3 obitalOrbit, glm::vec3 locate); 
glm::mat4 revolutionMoon(glm::vec3 centerDistance, glm::vec3 revolutionMoon); 
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
	star.planet[0].moon.revolution.y += 1.0;

	star.planet[1].revolution.y += 2.0f;
	star.planet[2].revolution.y += 3.0f;

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

GLvoid drawStar() {
	GLuint transformLocation = glGetUniformLocation(shaderProgramID, "model"); 
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(rotation(star.trans, star.rotation)));

	GLuint colorLocation = glGetUniformLocation(shaderProgramID, "color");
	glUniform3f(colorLocation, star.color.x, star.color.y, star.color.z);

	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluSphere(qobj, STAR_SIZE, 50, 50); 

	for (int i = 0; i < 3; i++) drawPlanet(&star.planet[i]);
}

GLvoid drawPlanet(PLANET* planet) { 
	glm::mat4 Matrix = glm::mat4(1.0f);
	Matrix = revolution(planet->revolution, planet->obitalOrbit, planet->trans) * rotation(star.trans, star.rotation);

	GLuint transformLocation = glGetUniformLocation(shaderProgramID, "model");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(Matrix));

	GLuint colorLocation = glGetUniformLocation(shaderProgramID, "color");
	glUniform3f(colorLocation, planet->color.x, planet->color.y, planet->color.z);

	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluSphere(qobj, PLANET_SIZE, 50, 50);

	drawMoon(planet);
}

GLvoid drawMoon(PLANET* planet) {
	glm::mat4 Matrix = glm::mat4(1.0f);
	Matrix = revolution(planet->revolution, planet->obitalOrbit, planet->trans) * revolutionMoon(planet->moon.trans, planet->moon.revolution) * rotation(star.trans, star.rotation);

	GLuint transformLocation = glGetUniformLocation(shaderProgramID, "model");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(Matrix));

	GLuint colorLocation = glGetUniformLocation(shaderProgramID, "color");
	glUniform3f(colorLocation, planet->moon.color.x, planet->moon.color.y, planet->moon.color.z);

	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
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

	blue_color(&star.planet[1].color); 
	star.planet[1].trans = glm::vec3(-1.5f, 0.0f, 0.0f);
	star.planet[1].obitalOrbit = glm::vec3(10.0f, 0.0f, -45.0f);

	red_color(&star.planet[2].color);
	star.planet[2].trans = glm::vec3(0.0f, 0.0f, 1.5f);
	star.planet[2].obitalOrbit = glm::vec3(10.0f, 0.0f, 0.0f);
}

// �׼�/�༺/�� ����
glm::mat4 rotation(glm::vec3 transPlanet, glm::vec3 rotatePlanet) {
	glm::mat4 Matrix = glm::mat4(1.0f);
	Matrix = rotate_shape(rotatePlanet);

	return Matrix;
}

// y�� ���� �༺ ����
glm::mat4 revolution(glm::vec3 revolutionPlanet, glm::vec3 obitalOrbit, glm::vec3 locate) {
	glm::mat4 Matrix = glm::mat4(1.0f);
	// ���� �˵�, ����
	Matrix = rotate_shape(obitalOrbit) * rotate_shape(revolutionPlanet) * translation_shape(locate);

	return Matrix;
}

// ���� ����
glm::mat4 revolutionMoon(glm::vec3 centerDistance, glm::vec3 revolutionMoon) {
	glm::mat4 Matrix = glm::mat4(1.0f);
	Matrix = rotate_shape(revolutionMoon) * translation_shape(centerDistance);

	return Matrix;
}
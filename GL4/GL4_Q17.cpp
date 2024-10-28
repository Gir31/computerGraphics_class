#include "GL_My_header.h"
//========================================================
#define LENGTH 0.5f

typedef struct {
	glm::vec3 trans;
	glm::vec3 rotate;
	glm::vec3 rotation;
}MODEL;

typedef struct {
	glm::vec3 point[4];
	glm::vec3 color[4];
	GLuint index[2][3];
	GLsizei vertex; 

	MODEL model;
}RECTANGLE;

typedef struct {
	glm::vec3 point[3];
	glm::vec3 color[3];
	GLuint index[1][3];
	GLsizei vertex;

	MODEL model;
	glm::vec3 inclination;
}TRIANGLE;

//========================================================
// 원근 투영
PROJECTION projection = { 45.0f, 0.0f, 0.1f, 50.0f };
glm::vec3 spaceTrans = glm::vec3(0.0f, 0.0f, -5.0f);
// 좌표 평면
GLfloat line[3][2][3] = {
	{{1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}}, // x축
	{{0.0f, 1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}}, // y축
	{{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}} // z축
};
GLfloat line_color[3][2][3] = {
	{{1.0f, 0.0f, 0.0f},{1.0f, 0.0f, 0.0f}},
	{{ 0.0f, 1.0f, 0.0f },{0.0f, 1.0f, 0.0f}},
	{{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}}
};
GLuint line_index[1][3] = { 0, 1 };
GLsizei line_vertex = 2;
// 좌표 평면 회전
glm::vec3 shapeDegree = glm::vec3(30.0f, 30.0f, 30.0f);
//========================================================
char vertex[] = { "vertex.glsl" };
char fragment[] = { "fragment.glsl" };
GLuint VAO, VBO, VBO_pos, EBO;
GLuint shaderProgramID;
//========================================================
// 사용자 지정 변수
RECTANGLE rec[6];
RECTANGLE rec_pir;
TRIANGLE tri[4]; 

glm::vec3 trans_cube[6] = {
	{glm::vec3(0.0f, 0.0f, LENGTH)},
	{glm::vec3(LENGTH, 0.0f, 0.0f)},
	{glm::vec3(0.0f, LENGTH, 0.0f)},
	{glm::vec3(0.0f, 0.0f, -LENGTH)},
	{glm::vec3(-LENGTH, 0.0f, 0.0f)},
	{glm::vec3(0.0f, -LENGTH, 0.0f)}
};
glm::vec3 trans_pir[4] = {
	{glm::vec3(0.0f, 0.0f, LENGTH)},
	{glm::vec3(LENGTH, 0.0f, 0.0f)},
	{glm::vec3(0.0f, 0.0f, -LENGTH)},
	{glm::vec3(-LENGTH, 0.0f, 0.0f)}
};
glm::vec3 spin_surface = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 open_surface[3] = {
	{glm::vec3(0.0f, LENGTH, 0.0f)}, // trans
	{glm::vec3(0.0f, 0.0f, 0.0f)}, // degree
	{glm::vec3(0.0f, -LENGTH, 0.0f)} //trans
};
glm::vec3 scale_surface = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec2 surfaceValue[4] = {
	{glm::vec2(-1.0f, 1.0f)},{glm::vec2(-1.0f, -1.0f)},
	{glm::vec2(1.0f, -1.0f)},{glm::vec2(1.0f, 1.0f)}
};
glm::vec2 surfaceValueTri[3] = {
	{glm::vec2(0.0f, 1.0f)},
	{glm::vec2(-1.0f, 0.0f)},
	{glm::vec2(1.0f, 0.0f)}
};
GLuint xyIndex[6] = { 0, 1, 3, 1, 2, 3 };
GLuint yzIndex[6] = { 0, 1, 2, 0, 2, 3 };
GLuint zxIndex[6] = { 0, 2, 1, 0, 3, 2 };
GLint openValue = 1;
GLfloat openSideValue = 0.1f;
GLfloat scaleValue = -0.1f;
GLfloat scaleTransValue = -0.05f;
GLfloat allSideOpenValue = 1.0f;
GLfloat oneSideOpenValue[4] = {1.0f, 1.0f, 1.0f, 1.0f};
GLint number = 0;

GLboolean hide = FALSE;
GLboolean timeSwitch = FALSE;
GLboolean rotationSwitch = FALSE;
GLboolean spinSwitch = FALSE;
GLboolean openSwitch = FALSE;
GLboolean openSideSwitch = FALSE;
GLboolean openBackSwitch = FALSE;
GLboolean allOpenSwitch = FALSE;
GLboolean oneOpenSwitch = FALSE;
GLboolean projectionSwitch = TRUE;
GLboolean shape = TRUE;
//========================================================
GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid SpecialKeyboard(int key, int x, int y);
GLvoid TimerFunction(int value);
void make_shaderProgram();
void InitBuffer_EBO(GLfloat vPositionList[][3], GLuint index[][3],
	GLfloat color[][3], GLuint vSize, GLuint iSize, GLuint cSize);
void InitBuffer_EBO_VEC(glm::vec3* vPositionList, GLuint index[][3], glm::vec3* color,
	GLuint vSize, GLuint iSize, GLuint cSize);
//========================================================
// 사용자 지정 함수
GLvoid Draw_Coordinate_Plane();
GLvoid Create_Rectangle(RECTANGLE* rec, GLint surface);
GLvoid Draw_Rectangle(RECTANGLE* rec, GLint surface);
GLvoid Create_Triangle(TRIANGLE* tri, GLint surface);
GLvoid Draw_Triangle(TRIANGLE* tri, GLint surface);
GLvoid MT_cubeValue();
GLvoid MT_cube(RECTANGLE* rec, GLint surface);
GLvoid MT_pirValue();
GLvoid MT_pir(TRIANGLE* tri, GLint surface);
GLvoid cubeRotation();
GLvoid triRotation(); 
GLvoid surfaceSpin();   
GLvoid surfaceOpen_cube(); 
GLvoid surfaceSideOpen_cube(); 
GLvoid scaleBack_cube(); 
GLvoid allSideOpen_tri();
GLvoid oneSideOpen_tri(TRIANGLE* tri, GLint surface); 
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

	for (int i = 0; i < 6; i++) Create_Rectangle(&rec[i], i % 3);
	MT_cubeValue();

	for (int i = 0; i < 4; i++) Create_Triangle(&tri[i], i % 2);
	MT_pirValue();
	Create_Rectangle(&rec_pir, 2);
	rec_pir.model.rotate = shapeDegree;

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKeyboard);
	glutMainLoop();
}

GLvoid drawScene()
{
	glEnable(GL_DEPTH_TEST);
	if(hide) glDisable(GL_DEPTH_TEST);

	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgramID);

	if (projectionSwitch) Perspective_Projection_Transformation(projection, spaceTrans, shaderProgramID);
	else Orthogoanl_Projection_Transformation(2.0f, shaderProgramID);

	Draw_Coordinate_Plane();

	if(shape)
		for (int i = 0; i < 6; i++) Draw_Rectangle(&rec[i], i);
	else {
		for (int i = 0; i < 4; i++) Draw_Triangle(&tri[i], i);
		Draw_Rectangle(&rec_pir, 6);
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
	case 'h':
		if (hide) hide = FALSE;
		else hide = TRUE;
		break;
	case 'y':
		if (rotationSwitch) rotationSwitch = FALSE;
		else rotationSwitch = TRUE;
		if (!timeSwitch) {
			timeSwitch = TRUE;
			glutTimerFunc(10, TimerFunction, 1);
		}
		break;
	case 't':
		if (spinSwitch) spinSwitch = FALSE;
		else spinSwitch = TRUE;
		if (!timeSwitch) {
			timeSwitch = TRUE;
			glutTimerFunc(10, TimerFunction, 1);
		}
		break;
	case 'f':
		if (openSwitch) openSwitch = FALSE;
		else openSwitch = TRUE;
		if (!timeSwitch) {
			timeSwitch = TRUE;
			glutTimerFunc(10, TimerFunction, 1);
		}
		break;
	case 's':
		if (openSideSwitch) openSideSwitch = FALSE;
		else openSideSwitch = TRUE;
		if (!timeSwitch) {
			timeSwitch = TRUE;
			glutTimerFunc(10, TimerFunction, 1);
		}
		break;
	case 'b':
		if (openBackSwitch) openBackSwitch = FALSE;
		else openBackSwitch = TRUE;
		if (!timeSwitch) {
			timeSwitch = TRUE;
			glutTimerFunc(10, TimerFunction, 1);
		}
		break;
	case 'o':
		if (allOpenSwitch) allOpenSwitch = FALSE;
		else allOpenSwitch = TRUE;
		if (!timeSwitch) {
			timeSwitch = TRUE;
			glutTimerFunc(10, TimerFunction, 1);
		}
		break;
	case 'R':
		if (oneOpenSwitch) oneOpenSwitch = FALSE;
		else {
			oneOpenSwitch = TRUE;
			number = (number + 1) % 4;
		}
		if (!timeSwitch) {
			timeSwitch = TRUE;
			glutTimerFunc(10, TimerFunction, 1);
		}
		break;
	case 'p':
		if (projectionSwitch) projectionSwitch = FALSE;
		else projectionSwitch = TRUE;
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
		if (shape) {
			shape = FALSE;
		}
		else {
			shape = TRUE;
		}
		break;
	case GLUT_KEY_DOWN:
		break;
	}
	glutPostRedisplay();
}

GLvoid TimerFunction(int value) {
	glutPostRedisplay();

	if (rotationSwitch) {
		cubeRotation();
		triRotation();
	}
	if (spinSwitch) surfaceSpin();
	if (openSwitch) surfaceOpen_cube();
	if (openSideSwitch) surfaceSideOpen_cube();
	if (openBackSwitch) scaleBack_cube();

	if (allOpenSwitch) allSideOpen_tri();
	else if (oneOpenSwitch) oneSideOpen_tri(&tri[number], number);

	if (timeSwitch) glutTimerFunc(10, TimerFunction, 1);
}

void Draw_Coordinate_Plane() {
	for (int i = 0; i < 3; i++) {
		unsigned int transformLocation = glGetUniformLocation(shaderProgramID, "model");
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(rotate_shape(shapeDegree)));

		InitBuffer_EBO(line[i], line_index, line_color[i], sizeof(line[i]), sizeof(line_index), sizeof(line_color[i]));
		glBindVertexArray(VAO);

		glDrawElements(GL_LINES, line_vertex, GL_UNSIGNED_INT, 0);
	}
}

GLvoid Create_Rectangle(RECTANGLE* rec, GLint surface) {
	switch (surface) {
	case 0: // xy
		for (int i = 0; i < 4; i++) {
			rec->point[i].x = LENGTH * surfaceValue[i].x;
			rec->point[i].y = LENGTH * surfaceValue[i].y;
			rec->point[i].z = 0.0f;

			yellow_color(&rec->color[i]);
		}

		for (int i = 0; i < 6; i++) rec->index[i / 3][i % 3] = xyIndex[i];

		rec->vertex = 6;
	
		break;
	case 1: // yz
		for (int i = 0; i < 4; i++) {
			rec->point[i].x = 0.0f;
			rec->point[i].y = LENGTH * surfaceValue[i].x;
			rec->point[i].z = LENGTH * surfaceValue[i].y;

			cyan_color(&rec->color[i]);
		}

		for (int i = 0; i < 6; i++) rec->index[i / 3][i % 3] = yzIndex[i];

		rec->vertex = 6;
		break;
	case 2: // zx
		for (int i = 0; i < 4; i++) {
			rec->point[i].x = LENGTH * surfaceValue[i].x;
			rec->point[i].y = 0.0f;
			rec->point[i].z = LENGTH * surfaceValue[i].y;

			magenta_color(&rec->color[i]);
		}

		for (int i = 0; i < 6; i++) rec->index[i / 3][i % 3] = zxIndex[i];

		rec->vertex = 6;
		break;
	}
}

GLvoid Draw_Rectangle(RECTANGLE* rec, GLint surface) {
	MT_cube(rec, surface);

	InitBuffer_EBO_VEC(rec->point, rec->index, rec->color, sizeof(rec->point), sizeof(rec->index), sizeof(rec->color));
	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES, rec->vertex, GL_UNSIGNED_INT, 0);
}

GLvoid Create_Triangle(TRIANGLE* tri, GLint surface) {
	switch (surface) {
	case 0: // xy
		for (int i = 0; i < 3; i++) {
			tri->point[i].x = surfaceValueTri[i].x * LENGTH;
			tri->point[i].y = sqrtf(2) * surfaceValueTri[i].y * LENGTH;
			tri->point[i].z = 0.0f;

			tri->index[0][i] = i;

			yellow_color(&tri->color[i]);
		}

		tri->vertex = 3;
		break;
	case 1: // yz
		for (int i = 0; i < 3; i++) {
			tri->point[i].x = 0.0f;
			tri->point[i].y = sqrtf(2) * surfaceValueTri[i].y * LENGTH;
			tri->point[i].z = surfaceValueTri[i].x * LENGTH;

			tri->index[0][i] = i;

			cyan_color(&tri->color[i]);
		}

		tri->vertex = 3;
		break;
	}
}

GLvoid Draw_Triangle(TRIANGLE* tri, GLint surface) {
	MT_pir(tri, surface);

	InitBuffer_EBO_VEC(tri->point, tri->index, tri->color, sizeof(tri->point), sizeof(tri->index), sizeof(tri->color));
	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES, tri->vertex, GL_UNSIGNED_INT, 0);
}

GLvoid MT_cubeValue() {
	for (int i = 0; i < 6; i++) {
		rec[i].model.trans.x = trans_cube[i].x;
		rec[i].model.trans.y = trans_cube[i].y;
		rec[i].model.trans.z = trans_cube[i].z;

		rec[i].model.rotate.x = shapeDegree.x;
		rec[i].model.rotate.y = shapeDegree.y;
		rec[i].model.rotate.z = shapeDegree.z;
	}
}

GLvoid MT_pirValue() {
	for (int i = 0; i < 4; i++) {
		tri[i].model.trans.x = trans_pir[i].x;
		tri[i].model.trans.y = trans_pir[i].y;
		tri[i].model.trans.z = trans_pir[i].z;

		tri[i].model.rotate.x = shapeDegree.x;
		tri[i].model.rotate.y = shapeDegree.y;
		tri[i].model.rotate.z = shapeDegree.z;
	}
	tri[1].inclination.z = tri[2].inclination.x = 45.0f;
	tri[0].inclination.x = tri[3].inclination.z = -45.0f;
}

GLvoid MT_cube(RECTANGLE* rec, GLint surface) {
	glm::mat4 Matrix;
	if (surface == 0) Matrix = rotate_shape(rec->model.rotate) * rotate_shape(rec->model.rotation) * translation_shape(rec->model.trans) 
		* translation_shape(open_surface[2]) * rotate_shape(open_surface[1]) * translation_shape(open_surface[0]);
	else if (surface == 2) Matrix = rotate_shape(rec->model.rotate) * rotate_shape(rec->model.rotation) 
		* translation_shape(rec->model.trans) * rotate_shape(spin_surface);
	else if (surface == 3) Matrix = rotate_shape(rec->model.rotate) * rotate_shape(rec->model.rotation) * translation_shape(rec->model.trans) * scaling_shape(scale_surface);
	else Matrix = rotate_shape(rec->model.rotate) * rotate_shape(rec->model.rotation) * translation_shape(rec->model.trans);

	unsigned int transformLocation = glGetUniformLocation(shaderProgramID, "model");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(Matrix));
}

GLvoid MT_pir(TRIANGLE* tri, GLint surface) {
	glm::mat4 Matrix;
	Matrix = rotate_shape(tri->model.rotate) * rotate_shape(tri->model.rotation) * translation_shape(tri->model.trans) * rotate_shape(tri->inclination);

	unsigned int transformLocation = glGetUniformLocation(shaderProgramID, "model");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(Matrix));
}

GLvoid cubeRotation() {
	for (int i = 0; i < 6; i++) rec[i].model.rotation.y += 2.0f;
}

GLvoid triRotation() {
	for (int i = 0; i < 4; i++) tri[i].model.rotation.y += 2.0f;
	rec_pir.model.rotation.y += 2.0f;
}

GLvoid surfaceSpin() {
	spin_surface.x += 2.0f;
}

GLvoid surfaceOpen_cube() {
	open_surface[1].x += openValue;

	if (open_surface[1].x > 89.0f || open_surface[1].x < 1.0f) {
		openValue *= -1;
		openSwitch = FALSE;
	}
}

GLvoid surfaceSideOpen_cube(){
	rec[1].model.trans.y += openSideValue;
	rec[4].model.trans.y += openSideValue;

	if (rec[1].model.trans.y > LENGTH * 2 || rec[1].model.trans.y < 0.1f) {
		openSideValue *= -1;
		openSideSwitch = FALSE;
	}
}

GLvoid scaleBack_cube() {
	scale_surface.x += scaleValue;
	scale_surface.y += scaleValue;
	rec[3].model.trans.y += scaleTransValue;

	if (scale_surface.x < 0.0f || scale_surface.x > 0.9f) {
		scaleValue *= -1;
		scaleTransValue *= -1;
		openBackSwitch = FALSE;
	}
}

GLvoid allSideOpen_tri() {
	tri[0].inclination.x += allSideOpenValue;
	tri[1].inclination.z -= allSideOpenValue;
	tri[2].inclination.x -= allSideOpenValue;
	tri[3].inclination.z += allSideOpenValue;

	if (tri[0].inclination.x > 224.0f || tri[0].inclination.x < -44.0f) {
		allSideOpenValue *= -1;
		allOpenSwitch = FALSE;
	}
}

GLvoid oneSideOpen_tri(TRIANGLE* tri, GLint surface) {
	switch (surface) {
	case 0:
		tri->inclination.x += oneSideOpenValue[0];
		if (tri->inclination.x > 89.0f || tri->inclination.x < -44.0f) {
			oneSideOpenValue[0] *= -1;
			oneOpenSwitch = FALSE;
		}
		break;
	case 1:
		tri->inclination.z -= oneSideOpenValue[1];
		if (tri->inclination.z < -89.0f || tri->inclination.z > 44.0f) {
			oneSideOpenValue[1] *= -1;
			oneOpenSwitch = FALSE;
		}
		break;
	case 2:
		tri->inclination.x -= oneSideOpenValue[2];
		if (tri->inclination.x < -89.0f || tri->inclination.x > 44.0f) {
			oneSideOpenValue[2] *= -1;
			oneOpenSwitch = FALSE;
		}
		break;
	case 3:
		tri->inclination.z += oneSideOpenValue[3];
		if (tri->inclination.z > 89.0f || tri->inclination.z < -44.0f) {
			oneSideOpenValue[3] *= -1;
			oneOpenSwitch = FALSE;
		}
		break;
	}
}
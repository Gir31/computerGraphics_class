#include "GL_My_header.h"
#include "obj_reader.h"
//#include "prac.h"
//========================================================
typedef struct {
	Model model;
	glm::vec3 color[8];
}MODEL;

MODEL cube;
MODEL light_cube;
//CModel m;
//========================================================
// 원근 투영
PROJECTION projection = { 45.0f, 0.0f, 0.1f, 50.0f };
glm::vec3 spaceTrans = glm::vec3(0.0f, 0.0f, -2.0f);
//========================================================
char vertex[] = { "vertex.glsl" };
char fragment[] = { "fragment.glsl" };
GLuint shaderProgramID;

unsigned int VBO[3], VAO, EBO;
//========================================================
// 사용자 지정 변수
glm::vec3 center = glm::vec3(0.f, 0.f, 0.f);
glm::vec3 origin_pos = glm::vec3(0.f, 0.f, 5.f);

glm::vec3 cameraPos = glm::vec3(0.f, 0.f, 20.f);
glm::vec3 cameraRotate = glm::vec3(0.f);

glm::vec3 lightPos = glm::vec3(0.f, 0.f, 5.f);
glm::vec3 lightColor = glm::vec3(1.f, 1.f, 1.f);
glm::vec3 light_revolution = glm::vec3(0.f, 30.f, 0.f);
glm::vec3 light_rotate = glm::vec3(0.f, 0.f, 0.f);

glm::vec3 rotate = glm::vec3(0.f, 0.f, 0.f);

GLUquadric* qobj;

GLfloat angle = 0.f;

GLboolean timeSwitch = FALSE;
GLboolean rFlag = FALSE;
GLboolean lFlag = FALSE;
GLboolean light_switch = TRUE;
//========================================================
GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid SpecialKeyboard(int key, int x, int y);
GLvoid TimerFunction(int value);
void make_shaderProgram();
void InitBuffer_(MODEL* model);
//========================================================
// 사용자 지정 함수

GLvoid cameraTranslation(glm::vec3 cameraTrans);

//========================================================

int main(int argc, char** argv)
{
	srand(time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800.0f, 800.0f);
	glutCreateWindow("Example1");

	glewExperimental = GL_TRUE;
	glewInit();

	read_obj_file("cube.obj", &cube.model);
	read_obj_file("cube.obj", &light_cube.model);

	cube.color[0] = glm::vec3(0.f, 1.f, 1.f);
	cube.color[1] = glm::vec3(0.f, 1.f, 1.f);
	cube.color[2] = glm::vec3(0.f, 1.f, 1.f);
	cube.color[3] = glm::vec3(0.f, 1.f, 1.f);
	cube.color[4] = glm::vec3(0.f, 1.f, 1.f);
	cube.color[5] = glm::vec3(0.f, 1.f, 1.f);
	cube.color[6] = glm::vec3(0.f, 1.f, 1.f);
	cube.color[7] = glm::vec3(0.f, 1.f, 1.f);

	light_cube.color[0] = glm::vec3(1.f, 1.f, 1.f);
	light_cube.color[1] = glm::vec3(1.f, 1.f, 1.f);
	light_cube.color[2] = glm::vec3(1.f, 1.f, 1.f);
	light_cube.color[3] = glm::vec3(1.f, 1.f, 1.f);
	light_cube.color[4] = glm::vec3(1.f, 1.f, 1.f);
	light_cube.color[5] = glm::vec3(1.f, 1.f, 1.f);
	light_cube.color[6] = glm::vec3(1.f, 1.f, 1.f);
	light_cube.color[7] = glm::vec3(1.f, 1.f, 1.f);

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
	glEnable(GL_CULL_FACE);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Perspective_Projection_Transformation(projection, spaceTrans, shaderProgramID);
	cameraTranslation(cameraPos);

	glUseProgram(shaderProgramID);

	InitBuffer_(&cube);

	unsigned int transformLocation = glGetUniformLocation(shaderProgramID, "model");

	glm::mat4 rotateCube = glm::mat4(1.f);
	rotateCube = rotate_shape(glm::vec3(30.f, 0.f, 30.f))* rotate_shape(rotate);
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(rotateCube));

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, cube.model.face_count * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	InitBuffer_(&light_cube);

	glm::mat4 light_ = glm::mat4(1.f);
	light_ = translation_shape(lightPos) * rotate_shape(light_rotate);
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(light_));

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, light_cube.model.face_count * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

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

void InitBuffer_(MODEL* model) {
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Vertex Buffer (정점 위치)
	glGenBuffers(1, &VBO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, model->model.vertex_count * sizeof(Vertex), model->model.vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0); // location = 0

	// Normal Buffer (법선)
	glGenBuffers(1, &VBO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, model->model.normal_count * sizeof(Normal), model->model.normals, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Normal), (void*)0);
	glEnableVertexAttribArray(1); // location = 1

	// Color Buffer (색상)
	glGenBuffers(1, &VBO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model->color), model->color, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2); // location = 2

	// Element Buffer
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->model.face_count * sizeof(Face), model->model.faces, GL_STATIC_DRAW);

	glBindVertexArray(0);

	// 조명
	glUseProgram(shaderProgramID);

	if (light_switch) {
		unsigned int lightPosLocation = glGetUniformLocation(shaderProgramID, "lightPos"); //--- lightPos 값 전달: (0.0, 0.0, 5.0);
		glUniform3f(lightPosLocation, lightPos.x, lightPos.y, lightPos.z);
		unsigned int lightColorLocation = glGetUniformLocation(shaderProgramID, "lightColor"); //--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
		glUniform3f(lightColorLocation, lightColor.x, lightColor.y, lightColor.z);
		unsigned int viewPosLocation = glGetUniformLocation(shaderProgramID, "viewPos"); //--- viewPos 값 전달: 카메라 위치
		glUniform3f(viewPosLocation, cameraPos.x, cameraPos.y, cameraPos.z);
	}
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'y': case 'Y':
		if (rFlag) rFlag = FALSE;
		else rFlag = TRUE;

		if (!timeSwitch) {
			timeSwitch = TRUE;
			glutTimerFunc(10, TimerFunction, 1);
		}
		break;
	case 'r': case 'R':
		if (lFlag) lFlag = FALSE;
		else lFlag = TRUE;

		if (!timeSwitch) {
			timeSwitch = TRUE;
			glutTimerFunc(10, TimerFunction, 1);
		}
		break;
	case 'm':
		if (light_switch) {
			light_switch = FALSE;
			lightColor = glm::vec3(1.f, 1.f, 1.f);

			unsigned int lightColorLocation = glGetUniformLocation(shaderProgramID, "lightColor"); //--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
			glUniform3f(lightColorLocation, lightColor.x, lightColor.y, lightColor.z);
		}
		else {
			light_switch = TRUE;
			lightColor = glm::vec3(0.f, 0.f, 0.f);

			unsigned int lightColorLocation = glGetUniformLocation(shaderProgramID, "lightColor"); //--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
			glUniform3f(lightColorLocation, lightColor.x, lightColor.y, lightColor.z);
		}
		break;
	case 'z':
		if(origin_pos.z > 1.f) origin_pos.z -= 0.5f;
		// 회전 변환 적용
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f)); // Y축 기준 회전
		glm::vec4 rotatedLightPos = rotation * glm::vec4(origin_pos, 1.0f);

		// 공전 결과 조명 위치
		lightPos = glm::vec3(rotatedLightPos) + center;
		break;
	case 'Z':
		origin_pos.z += 0.5f;
		// 회전 변환 적용
		rotation = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f)); // Y축 기준 회전
		rotatedLightPos = rotation * glm::vec4(origin_pos, 1.0f);

		// 공전 결과 조명 위치
		lightPos = glm::vec3(rotatedLightPos) + center;
		break;
	case 'q': case 'Q':
		glutLeaveMainLoop();
	}
	glutPostRedisplay();
}

GLvoid SpecialKeyboard(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		cameraRotate.y -= 1.f;
		break;
	case GLUT_KEY_RIGHT:
		cameraRotate.y += 1.f;
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

	if (rFlag) rotate.y += 1.f;
	if (lFlag) {
		angle += 0.01f;

		// 회전 변환 적용
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f)); // Y축 기준 회전
		glm::vec4 rotatedLightPos = rotation * glm::vec4(origin_pos, 1.0f);

		// 공전 결과 조명 위치
		lightPos = glm::vec3(rotatedLightPos) + center;
	}

	if (timeSwitch) glutTimerFunc(10, TimerFunction, 1);
}

GLvoid cameraTranslation(glm::vec3 cameraTrans) {
	glm::vec3 zeroPoint = glm::vec3(0, 0, 0);
	glm::mat4 view = glm::mat4(1.0f);
	view = camera_locate(cameraTrans, zeroPoint) * rotate_camera(cameraRotate);

	unsigned int viewLocation = glGetUniformLocation(shaderProgramID, "view");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
}
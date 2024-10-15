#include "GL_My_header.h"

char vertex[] = { "vertex1.glsl" };
char fragment[] = { "fragment1.glsl" };

GLuint shaderProgramID;

float vPositionList[4][3] = {
	{0.5f, 0.5f, 0.0f}, // 우측 상단
	{0.5f, -0.5f, 0.0f}, // 우측 하단
	{-0.5f, -0.5f, 0.0f}, // 좌측 하단
	{-0.5f, 0.5f, 0.0f} // 좌측 상단
};
unsigned int index[2][3] = {
	{0, 1, 3}, // 첫 번째 삼각형
	{1, 2, 3} // 두 번째 삼각형
};

GLfloat color[3][3] = { {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0} };;
GLuint VAO, VBO_pos, EBO;

GLvoid drawScene();
GLvoid Reshape(int w, int h);

void make_shaderProgram();
void InitBuffer_ebo();

int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Example1");

	glewExperimental = GL_TRUE;
	glewInit();

	make_shaderProgram();
	InitBuffer_ebo();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}


void InitBuffer_ebo()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO_pos);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_pos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vPositionList), vPositionList, GL_STATIC_DRAW);
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //--- GL_ELEMENT_ARRAY_BUFFER 버퍼 유형으로 바인딩
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
}
void drawScene()
{
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgramID);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //--- 0번부터 6개의 꼭지점을 사용하여 삼각형을 그린다.
	
	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
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
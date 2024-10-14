#include "GL_My_header.h"

#define MAX_LENGTH 0.5

typedef struct {

	GLfloat point[5][3];
	GLuint index[3][3];
	GLfloat RGB[3][3]; 
	GLint type; // | 0 : 선 | 1 : 삼각형 | 2 : 사각형 | 3 : 오각형 |

}SHAPE;

GLfloat vPositionList[] = {
	0.5f, 0.5f, 0.0f, // 우측 상단
	0.5f, -0.5f, 0.0f , // 우측 하단
	 -0.5f, -0.5f, 0.0f, // 좌측 하단
	-0.5f, 0.5f, 0.0f // 좌측 상단
};
GLuint index[] = {
	0, 1, 3 , // 첫 번째 삼각형
	1, 2, 3 // 두 번째 삼각형
};

char vertex[] = { "vertex1.glsl" };
char fragment[] = { "fragment1.glsl" };
GLuint VAO, VBO_pos, EBO; 
SHAPE shape;

GLuint shaderProgramID;

GLvoid drawScene();
GLvoid Reshape(int w, int h);

void InitBuffer_EBO(GLfloat* vPositionList, GLuint* index);
void make_shaderProgram();

void make_rectangle(SHAPE* s);

int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Example1");

	glewExperimental = GL_TRUE;
	glewInit();


	make_rectangle(&shape);
	make_shaderProgram();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

GLvoid drawScene()
{
	glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgramID);

	InitBuffer_EBO(vPositionList, index);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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

void InitBuffer_EBO(GLfloat* vPositionList, GLuint* index)
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

void reset_shape(SHAPE* s) {

}

void make_iine(SHAPE* s) {

}

void make_triangle(SHAPE* s) {

}

void make_rectangle(SHAPE* s) {
	s->point[0][0] = -0.5f;
	s->point[0][1] = -0.5f;

	s->point[1][0] = 0.5f;
	s->point[1][1] = -0.5f;

	s->point[2][0] = 0.5f;
	s->point[2][1] = 0.5f;

	s->point[3][0] = 0.5f;
	s->point[3][1] = 0.5f;

	s->index[0][0] = 0;
	s->index[0][1] = 1;
	s->index[0][2] = 3;

	s->index[1][0] = 1;
	s->index[1][1] = 2;
	s->index[1][2] = 3;
}

void make_pentagon(SHAPE* s) {

}
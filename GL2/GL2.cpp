#include "GL_My_header.h"

GLint width, height; 
GLuint shaderProgramID;

char vertex[] = {"vertex1.glsl"};
char fragment[] = { "fragment1.glsl" };

GLuint vao, vbo[2];

GLvoid drawScene();
GLvoid Reshape(int w, int h);

void make_shaderProgram();

int main(int argc, char** argv)
{
	width = 500;
	height = 500;
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("Example1"); 
		
	glewExperimental = GL_TRUE;
	glewInit();
	
	//make_VertexShaders(vertex);
	//make_FragmentShaders(fragment);
	//shaderProgramID = make_ShaderProgram();

	make_shaderProgram();
	InitBuffer(vao, vbo);

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

GLvoid drawScene()
{
	glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgramID);

	glPointSize(vao);

	glDrawArrays(GL_TRIANGLES, 0, 3);

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
	//-- shader Program
	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexShader);
	glAttachShader(shaderProgramID, fragmentShader);
	glLinkProgram(shaderProgramID);
	//--- 세이더 삭제하기
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program 사용하기
	glUseProgram(shaderProgramID);
}
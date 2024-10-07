#include "GL_My_header.h"

GLint width, height; 
GLuint shaderProgramID;

char vertex[] = {"vertex.glsl"};
char fragment[] = { "fragment.glsl" };

GLvoid drawScene();
GLvoid Reshape(int w, int h);

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
	
	make_VertexShaders(vertex); 
	make_FragmentShaders(fragment);
	shaderProgramID = make_ShaderProgram();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

GLvoid drawScene()
{
	GLfloat rColor, gColor, bColor;
	rColor = gColor = 0.0;
	bColor = 1.0; 
	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(shaderProgramID);
	glPointSize(5.0);
	glDrawArrays(GL_POINTS, 0, 1);
	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

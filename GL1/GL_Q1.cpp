#include <math.h>
#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

#define ON 1
#define OFF 0

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
void TimerFunction(int value);

GLclampf color[3] = { 0.0f, 0.0f, 1.0f};
int timer;

void main(int argc, char** argv)
{	
	srand(time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100); 
	glutInitWindowSize(800, 600); 
	glutCreateWindow("GL_Q1 : 배경 색 바꾸기"); 

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape); 
	glutKeyboardFunc(Keyboard);
	glutMainLoop();
}

GLvoid drawScene()
{
	glClearColor(color[0], color[1], color[2], 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glutSwapBuffers(); 
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'c' :
		color[0] = 0.0f;
		color[1] = 1.0f;
		color[2] = 1.0f;
		break;
	case 'm' :
		color[0] = 1.0f;
		color[1] = 0.0f;
		color[2] = 1.0f;
		break;
	case 'y' :
		color[0] = 1.0f;
		color[1] = 1.0f;
		color[2] = 0.0f;
		break;
	case 'a' :
		color[0] = ((GLclampf)rand() / (GLclampf)rand());
		color[1] = ((GLclampf)rand() / (GLclampf)rand());
		color[2] = ((GLclampf)rand() / (GLclampf)rand());
		break;
	case 'w':
		color[0] = 1.0f;
		color[1] = 1.0f;
		color[2] = 1.0f;
		break;
	case 'k':
		color[0] = 0.0f;
		color[1] = 0.0f;
		color[2] = 0.0f;
		break;
	case 't':
		timer = ON;
		glutTimerFunc(1000, TimerFunction, 1);
		break;
	case 's':
		timer = OFF;
		break;
	case 'q': glutLeaveMainLoop();
	}
	glutPostRedisplay();
}

void TimerFunction(int value) {

	color[0] = ((GLclampf)rand() / (GLclampf)rand());
	color[1] = ((GLclampf)rand() / (GLclampf)rand());
	color[2] = ((GLclampf)rand() / (GLclampf)rand());

	glutPostRedisplay();
	if(timer == ON) glutTimerFunc(1000, TimerFunction, 1);
}
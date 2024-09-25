#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

#define MAX_X 800
#define MAX_Y 600

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

void Mouse(int button, int state, int x, int y);
void draw_rectangle(GLclampf* color, int* xy);
void rectangle_color_rand(GLclampf* color);

int conversion_x(int x);
int conversion_y(int y);

GLclampf rectangle_color[4][3] = { {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 1.0f},
									{0.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f} };
int rectangle_xy[4][4] = { {(MAX_X / 2 * -1), MAX_Y / 2, 0, 0},{0, MAX_Y / 2, MAX_X / 2, 0},
						{(MAX_X / 2 * -1), 0, 0, (MAX_Y / 2 * -1)},{0, 0, MAX_X / 2, (MAX_Y / 2 * -1)}};

void main(int argc, char** argv)
{	
	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); 
	glutInitWindowPosition(100, 100); 
	glutInitWindowSize(MAX_X, MAX_Y);
	glutCreateWindow("Example1"); 

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
	glutMouseFunc(Mouse);
	glutMainLoop();
}

GLvoid drawScene() 
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); 
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < 4; i++) {
		draw_rectangle(rectangle_color[i], rectangle_xy[i]);
	}
	
	glutSwapBuffers(); 
}

GLvoid Reshape(int w, int h) 
{
	glViewport(0, 0, w, h);
}

void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {

	}

}

void draw_rectangle(GLclampf* color, int* xy) {
	glColor3f(color[0], color[1], color[2]);
	glRectf(xy[0], xy[1], xy[2], xy[3]);
}

void rectangle_color_rand(GLclampf* color) {
	for (int i = 0; i < 3; i++) {
		color[i] = ((GLclampf)rand() / (GLclampf)rand());
	}
}

int conversion_x(int x) { return x - 400; }

int conversion_y(int y) { return (y - 300) * -1; }

int select_rectangle(int x, int y) {
	return 1;
}
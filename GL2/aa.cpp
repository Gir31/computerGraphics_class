#include "GL_My_header.h"
#include <cstdlib>
#include <ctime>
#include <cmath>

#define PI 3.141592
#define MAX_SPIRALS 5

typedef struct {
    GLfloat x, y;  // �߽� ��ǥ
    GLfloat r;     // ������
    GLint direction;  // 1: �ð����, -1: �ݽð����
    GLint rotations;  // ȸ�� ��
} SPIRAL;

GLuint shaderProgramID;
GLuint vao, vbo[2];

int spiralCount = 1;
bool drawAsPoints = true;  // true�� ������, false�� ������ �׸���
std::vector<SPIRAL> spirals;

GLvoid drawScene();
GLvoid Reshape(int w, int h);

void make_shaderProgram();
void TimerFunction(int value);
void Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);

void addSpiral(int x, int y);
void drawSpiral(SPIRAL* spiral);
void changeBackgroundColor();
GLfloat randomFloat(GLfloat min, GLfloat max);

int main(int argc, char** argv)
{
    srand(static_cast<unsigned int>(time(0)));  // ���� �ʱ�ȭ

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Spiral Animation");

    glewExperimental = GL_TRUE;
    glewInit();

    make_shaderProgram();

    // �ݹ� �Լ� ���
    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutTimerFunc(10, TimerFunction, 1);
    glutKeyboardFunc(Keyboard);
    glutMouseFunc(Mouse);

    glutMainLoop();
}

GLvoid drawScene()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgramID);

    for (auto& spiral : spirals) {
        drawSpiral(&spiral);
    }

    glutSwapBuffers();
}

GLvoid Reshape(int w, int h) { glViewport(0, 0, w, h); }

void make_shaderProgram()
{
    // ���̴� ���α׷� ����
    shaderProgramID = glCreateProgram();
    // ���̴� �ۼ� �Լ� �߰�
    glUseProgram(shaderProgramID);
}

void TimerFunction(int value) {
    glutPostRedisplay();
    glutTimerFunc(10, TimerFunction, 1);
}

void Keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 'p':  // ������ �׸���
        drawAsPoints = true;
        break;
    case 'l':  // ������ �׸���
        drawAsPoints = false;
        break;
    case '1': case '2': case '3': case '4': case '5':  // 1~5�� �����̷� �׸���
        spiralCount = key - '0';  // ���� ��ȯ
        break;
    }
}

void Mouse(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN) {
        for (int i = 0; i < spiralCount; i++) {
            int randomX = rand() % 800;
            int randomY = rand() % 800;
            addSpiral(randomX, randomY);
        }
        // ������ �������� ����
        changeBackgroundColor();
    }
}

void addSpiral(int x, int y)
{
    SPIRAL newSpiral;
    newSpiral.x = (x - 400.0f) / 400.0f;  // ��ǥ ��ȯ (0~800�� -1~1��)
    newSpiral.y = -(y - 400.0f) / 400.0f;
    newSpiral.r = 0.0001f;
    newSpiral.direction = (rand() % 2 == 0) ? 1 : -1;  // �ð�/�ݽð� ���� ����
    newSpiral.rotations = 2 + rand() % 2;  // 2~3 ���� ���� ȸ��

    spirals.push_back(newSpiral);
}

void drawSpiral(SPIRAL* spiral)
{
    int points = 100;  // �� ������ 100�� �׸���
    float angleStep = 2 * PI / points;
    float rStep = 0.005f;

    glBegin(drawAsPoints ? GL_POINTS : GL_LINE_STRIP);
    for (int i = 0; i < points * spiral->rotations; i++) {
        float angle = i * angleStep * spiral->direction;
        float r = spiral->r + rStep * i;

        float x = spiral->x + r * cos(angle);
        float y = spiral->y + r * sin(angle);
        glPointSize(10);
        glVertex2f(x, y);
    }
    glEnd();
}

void changeBackgroundColor()
{
    GLfloat r = randomFloat(0.0f, 1.0f);
    GLfloat g = randomFloat(0.0f, 1.0f);
    GLfloat b = randomFloat(0.0f, 1.0f);

    glClearColor(r, g, b, 1.0f);
}

GLfloat randomFloat(GLfloat min, GLfloat max)
{
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

#include "GL_My_header.h"
#include <cmath>

#define PI (GLfloat)3.141592

typedef struct triangle {
	GLfloat shape[3][3];
	GLfloat color[3][3];
	GLfloat speed;
	GLfloat target_sprial;
	GLfloat length;
	GLfloat clength; // 중점으로부터의 거리
	GLint radian; 
	int count;
	int spiral_count;
	bool x_inc, y_inc; // FALSE - / TRUE +
	bool show;
	bool type; // FALSE 는 면, TRUE 는 선
}TRIANGLE;


char vertex[] = { "vertex1.glsl" };
char fragment[] = { "fragment1.glsl" };

GLuint shaderProgramID;

GLfloat line_shape[2][3][3] = {
	{{-1.0, 0.0, 1.0}, {1.0, 0.0, 1.0}, {0.0, 0.0, 0.0} },
	{{0.0, 1.0, 1.0}, {0.0, -1.0, 1.0}, {0.0, 0.0, 0.0} }
};
GLfloat line_color[2][3][3] = {
	{{0.0, 0.0, 0.0},{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}},
	{{0.0, 0.0, 0.0},{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}
};

TRIANGLE tri[4][3];
GLuint vao, vbo[2];
GLint tri_count[4];

bool shape_type = FALSE;

bool time_switch = FALSE;
bool dia_switch = FALSE;
bool zigzag_switch = FALSE;
bool rec_spiral_switch = FALSE;
bool circle_spiral_switch = FALSE;

GLvoid drawScene();  
GLvoid Reshape(int w, int h);
void make_shaderProgram();

GLvoid Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void TimerFunction(int value);

void c_find(TRIANGLE t[][3]);
void make_triangle(TRIANGLE* t, GLfloat cx, GLfloat cy);
void circle_triangle(TRIANGLE* t, GLfloat cx, GLfloat cy);

void dia_move(TRIANGLE* t);
void zigzag(TRIANGLE* t);
void rec_spiral(TRIANGLE* t); 
void circle_spiral(TRIANGLE* t);
void hori_move(TRIANGLE* t);
void ver_move(TRIANGLE* t);

void turn_tri(TRIANGLE* t, GLint radian);

GLfloat random_length();
GLint in_quadrant(int x, int y);

GLfloat find_cx(GLfloat x1, GLfloat x2);
GLfloat find_cy(GLfloat y1, GLfloat y2);
GLint get_radian(GLfloat cx, GLfloat cy);
GLfloat get_length(GLfloat cx, GLfloat cy);

bool target_length(GLfloat tc, GLfloat tl, GLfloat value);
bool touch_top(GLfloat top);
bool touch_bottom(GLfloat bottom);
bool touch_right(GLfloat right);
bool touch_left(GLfloat left);

int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Q8. 화면에 삼각형 그리기");

	glewExperimental = GL_TRUE;
	glewInit();

	c_find(tri);

	make_shaderProgram();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard);
	glutMainLoop();
}

GLvoid drawScene()
{
	glClearColor(0.5, 0.5, 0.5, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgramID);

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			InitBuffer(vao, vbo, tri[i][j].shape, tri[i][j].color);
			glPointSize(vao);

			if (tri[i][j].type) glPolygonMode(GL_FRONT, GL_LINE);
			else  glPolygonMode(GL_FRONT, GL_FILL);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
	}

	for (int i = 0; i < 2; i++) {
		InitBuffer(vao, vbo, line_shape[i], line_color[i]);
		glPointSize(vao);

		glDrawArrays(GL_LINES, 0, 2);
	}

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

void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		int quadrant = in_quadrant(x, y);

		if (tri_count[quadrant] < 2){
			tri_count[quadrant]++;
			make_triangle(&tri[quadrant][tri_count[quadrant]], conversion_x(x), conversion_y(y));
		}

	} else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		int quadrant = in_quadrant(x, y);

		make_triangle(&tri[quadrant][tri_count[quadrant]], conversion_x(x), conversion_y(y));
	}
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case '1':
		if (!time_switch) {
			time_switch = TRUE;
			dia_switch = TRUE;
			zigzag_switch = FALSE;
			rec_spiral_switch = FALSE;
			glutTimerFunc(10, TimerFunction, 1);
		}
		else {
			dia_switch = FALSE;
			zigzag_switch = FALSE;
		}
		break;
	case '2':
		if (!time_switch) {
			time_switch = TRUE;
			dia_switch = FALSE;
			zigzag_switch = TRUE;
			rec_spiral_switch = FALSE;
			glutTimerFunc(10, TimerFunction, 1);
		}
		else {
			time_switch = FALSE;
			zigzag_switch = FALSE;
		}
		break;
	case '3':
		if (!time_switch) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 3; j++) {
					tri[i][j].target_sprial = (GLfloat)1.0;
					tri[i][j].x_inc = TRUE;
					tri[i][j].y_inc = FALSE;
					tri[i][j].spiral_count = 0;
					turn_tri(&tri[i][j], 0);
				}
			}
			time_switch = TRUE;
			dia_switch = FALSE;
			zigzag_switch = FALSE;
			rec_spiral_switch = TRUE;
			glutTimerFunc(10, TimerFunction, 1);
		}
		else {
			time_switch = FALSE;
			rec_spiral_switch = FALSE;
		}
		break;
	case '4':
		if (!time_switch) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 3; j++) {
					GLfloat cx = find_cx(tri[i][j].shape[2][0], find_cx(tri[i][j].shape[0][0], tri[i][j].shape[1][0]));
					GLfloat cy = find_cy(tri[i][j].shape[2][1], find_cy(tri[i][j].shape[0][1], tri[i][j].shape[1][1]));

					tri[i][j].radian = get_radian(cx, cy);
					tri[i][j].clength = get_length(cx, cy);
				}
			}
			time_switch = TRUE;
			circle_spiral_switch = TRUE;
			dia_switch = FALSE;
			zigzag_switch = FALSE;
			rec_spiral_switch = FALSE;
			glutTimerFunc(10, TimerFunction, 1);
		}
		else {
			time_switch = FALSE;
			circle_spiral_switch = FALSE;
		}
		break;
	case 'a': shape_type = FALSE; break;
	case 'b': shape_type = TRUE; break;
	case 'q': glutLeaveMainLoop();
	}
	glutPostRedisplay();
}

void TimerFunction(int value) {
	glutPostRedisplay();

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			if (tri[i][j].show) {
				if (dia_switch) {
					dia_move(&tri[i][j]);
				}
				else if (zigzag_switch) {
					zigzag(&tri[i][j]);
				}
				else if (rec_spiral_switch) {
					rec_spiral(&tri[i][j]);
				}
				else if (circle_spiral_switch) {
					circle_spiral(&tri[i][j]);
				}
			}
		}
	}
	if(time_switch)glutTimerFunc(10, TimerFunction, 1);
}


void c_find(TRIANGLE t[][3]) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(100, HEIGHT / 2 - 100);
	GLfloat x, y;

	// 1사분면 삼각형
	x = conversion_x(dis(gen)) * -1;
	y = conversion_y(dis(gen));

	make_triangle(&t[0][0], x, y);

	// 2사분면 삼각형
	x = conversion_x(dis(gen));
	y = conversion_y(dis(gen));

	make_triangle(&t[1][0], x, y);

	// 3사분면 삼각형
	x = conversion_x(dis(gen));
	y = conversion_y(dis(gen)) * -1;

	make_triangle(&t[2][0], x, y);

	// 4사분면 삼각형
	x = conversion_x(dis(gen)) * -1;
	y = conversion_y(dis(gen)) * -1;

	make_triangle(&t[3][0], x, y);
}

void make_triangle(TRIANGLE* t, GLfloat cx, GLfloat cy) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(1, 4);
	GLfloat length  = random_length();
	t->length = length;
	// 첫번째 정점
	t->shape[0][0] = cx - (length / 2);
	t->shape[0][1] = cy - length;
	t->shape[0][2] = 0.5;
	// 두번째 정점
	t->shape[1][0] = cx + (length / 2);
	t->shape[1][1] = cy - length;
	t->shape[1][2] = 0.5;
	// 세번째 정점
	t->shape[2][0] = cx;
	t->shape[2][1] = cy + length;
	t->shape[2][2] = 0.5;

	random_color(t->color);

	t->show = TRUE;
	t->type = shape_type;
	t->x_inc = dis(gen) % 2;
	t->y_inc = dis(gen) % 2;
	t->speed = (GLfloat)dis(gen) / (GLfloat)100;

	if (rec_spiral_switch) {
		t->target_sprial = (GLfloat)1.0;
		t->x_inc = TRUE;
		t->y_inc = FALSE;
		t->spiral_count = 0;
		turn_tri(t, 0);
	}
	
	if (circle_spiral_switch) {
		t->radian = get_radian(cx, cy);
		t->clength = get_length(cx, cy);
		circle_triangle(t, cx, cy);
	}
}

void circle_triangle(TRIANGLE* t, GLfloat cx, GLfloat cy) {
	GLfloat length = t->length;

	// 첫번째 정점
	t->shape[0][0] = cx - (length / 2);
	t->shape[0][1] = cy - length;
	t->shape[0][2] = 0.5;
	// 두번째 정점
	t->shape[1][0] = cx + (length / 2);
	t->shape[1][1] = cy - length;
	t->shape[1][2] = 0.5;
	// 세번째 정점
	t->shape[2][0] = cx;
	t->shape[2][1] = cy + length;
	t->shape[2][2] = 0.5;
}

void dia_move(TRIANGLE* t) {
	if (touch_top(t->shape[2][1])) t->y_inc = FALSE;
	else if (touch_bottom(t->shape[0][1])) t->y_inc = TRUE;
	if (touch_right(t->shape[1][0])) t->x_inc = FALSE;
	else if (touch_left(t->shape[0][0])) t->x_inc = TRUE;

	hori_move(t);

	ver_move(t);

	if (t->x_inc && t->y_inc) turn_tri(t, 45);
	else if(!(t->x_inc) && t->y_inc) turn_tri(t, 135);
	else if (t->x_inc && !(t->y_inc)) turn_tri(t, 315);
	else if (!(t->x_inc) && !(t->y_inc)) turn_tri(t, 225);
}

void zigzag(TRIANGLE* t) {
	if (touch_top(t->shape[2][1])) t->y_inc = FALSE;
	else if (touch_bottom(t->shape[0][1])) t->y_inc = TRUE;

	if (touch_right(t->shape[1][0])) {
		t->x_inc = FALSE;
		t->count = 0;
		if (t->y_inc) turn_tri(t, 90);
		else turn_tri(t, 270);
		hori_move(t);
		hori_move(t);
		hori_move(t);
	}
	else if (touch_left(t->shape[0][0])) {
		t->x_inc = TRUE;
		t->count = 0;
		if (t->y_inc) turn_tri(t, 90);
		else turn_tri(t, 270);
		hori_move(t);
		hori_move(t);
		hori_move(t);
	}
	
	if (t->count == 8) {
		if(t->x_inc) turn_tri(t, 0);
		else turn_tri(t, 180);
		hori_move(t);
	}
	else { 
		ver_move(t);
		t->count++;
	}
}

void rec_spiral(TRIANGLE* t) {
	GLfloat value = -0.1;
	GLfloat cx = find_cx(t->shape[2][0], find_cx(t->shape[0][0], t->shape[1][0]));
	GLfloat cy = find_cy(t->shape[2][1], find_cy(t->shape[0][1], t->shape[1][1]));

	if (t->target_sprial < 0.1 && t->target_sprial > -0.1) {
		t->target_sprial = (GLfloat)1.0;
		t->x_inc = TRUE;
		t->y_inc = FALSE;
		t->spiral_count = 0;
		turn_tri(t, 0);
	}

	switch (t->spiral_count) {
	case 0: // right move
		hori_move(t);

		if (target_length(cx, t->target_sprial, value)) {
			t->target_sprial = t->target_sprial * -1;

			t->x_inc = FALSE;
			t->spiral_count++;
			turn_tri(t, 270);
		}
		break;
	case 1: // down move
		ver_move(t);

		if (target_length(cy, t->target_sprial, value)) {
			t->y_inc = TRUE;
			t->spiral_count++;
			turn_tri(t, 180);
		}
		break;
	case 2: // left move
		hori_move(t);

		if (target_length(cx, t->target_sprial, value)) {
			t->target_sprial = t->target_sprial * -1 + value;
			t->x_inc = TRUE;
			t->spiral_count++;
			turn_tri(t, 90);
		}
		break;
	case 3: // up move
		ver_move(t);

		
		if (target_length(cy, t->target_sprial, value)) {
			t->y_inc = FALSE;
			t->spiral_count = 0;
			turn_tri(t, 0);
		}
		break;
	}
}

void circle_spiral(TRIANGLE* t) {
	GLfloat cx = t->clength * cosf(t->radian * PI / 180);
	GLfloat	cy = t->clength * sinf(t->radian * PI / 180);
	GLint speed = t->speed * 100;

	if (touch_top(cy)) {
		cx = 0;
		cy = 0;
		t->clength = 0;
	}
	else if (touch_bottom(cy)) {
		cx = 0;
		cy = 0;
		t->clength = 0;
	}
	else if (touch_right(cx)) {
		cx = 0;
		cy = 0;
		t->clength = 0;
	}
	else if (touch_left(cx)) {
		cx = 0;
		cy = 0;
		t->clength = 0;
	}

	circle_triangle(t, cx, cy);

	turn_tri(t, t->radian + 90);
	t->radian = (t->radian + speed) % 360;
	t->clength += t->speed / 100;
}

void hori_move(TRIANGLE* t) {
	for (int i = 0; i < 3; i++) {
		if (t->x_inc) t->shape[i][0] += t->speed;
		else t->shape[i][0] -= t->speed;
	}
}

void ver_move(TRIANGLE* t) {
	for (int i = 0; i < 3; i++) {
		if (t->y_inc) t->shape[i][1] += t->speed;
		else t->shape[i][1] -= t->speed;
	}
}

void turn_tri(TRIANGLE* t, GLint radian) { 
	GLfloat cx = find_cx(t->shape[2][0], find_cx(t->shape[0][0], t->shape[1][0])); 
	GLfloat cy = find_cy(t->shape[2][1], find_cy(t->shape[0][1], t->shape[1][1])); 

	GLfloat length = sqrtf((powf((cx - t->shape[2][0]), 2) + powf((cy - t->shape[2][1]), 2)));
	GLfloat length2 = sqrtf((powf((cx - t->shape[1][0]), 2) + powf((cy - t->shape[1][1]), 2)));

	t->shape[0][0] = cx + (length2 * cosf((radian + 150) * PI / 180));
	t->shape[0][1] = cy + (length2 * sinf((radian + 150) * PI / 180));

	t->shape[1][0] = cx + (length2 * cosf((radian + 210) * PI / 180));
	t->shape[1][1] = cy + (length2 * sinf((radian + 210) * PI / 180));

	t->shape[2][0] = cx + (length * cosf(radian * PI / 180));
	t->shape[2][1] = cy + (length * sinf(radian * PI / 180));
}

GLfloat random_length() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(1, 5);

	return (GLfloat)dis(gen) / (GLfloat)20;
}

GLint in_quadrant(int x, int y) {
	if (x > WIDTH / 2) {
		if (y < HEIGHT / 2) return 0;
		else return 3;
	}
	else {
		if (y < HEIGHT / 2) return 1;
		else return 2;
	}
}

GLfloat find_cx(GLfloat x1, GLfloat x2) {
	GLfloat length = (x2 - x1) / 2;

	return x1 + length;
}

GLfloat find_cy(GLfloat y1, GLfloat y2) {
	GLfloat length = (y2 - y1) / 2;

	return y1 + length;
}

GLint get_radian(GLfloat cx, GLfloat cy) {
	return atan2(cy, cx) * 180 / PI;
}

GLfloat get_length(GLfloat cx, GLfloat cy) {
	return sqrtf((powf(cx, 2) + powf(cy, 2)));
}

bool target_length(GLfloat tc, GLfloat tl, GLfloat value) {
	if (value < 0) {
		if (tl < 0) {
			if (tl > tc) return TRUE;
		}
		else {
			if (tl < tc) return TRUE;
		}
	}else {
		if (tl < 0) {
			if (tl < tc) return TRUE;
		}
		else {
			if (tl > tc) return TRUE;
		}
	}
	return FALSE;
}

bool touch_top(GLfloat top) { return (1.0 < top); }
bool touch_bottom(GLfloat bottom) { return (-1.0 > bottom); }
bool touch_right(GLfloat right) { return (1.0 < right); }
bool touch_left(GLfloat left) { return (-1.0 > left); }
#include "GL_My_header.h"
//========================================================
typedef struct {
	glm::vec3 move;
	glm::vec3 dir;
	glm::vec3 left_motion, right_motion;

	glm::vec3 size;

	GLint count;

	GLint moveDir; // | 0 : up | 1 : right | 2 : down | 3 : left |
}ROBOT;

ROBOT robot;
ROBOT origin_robot[20];
ROBOT small_robot[3];
ROBOT origin_small_robot[20];
ROBOT origin_small_robot2[20];
//========================================================
// 원근 투영
PROJECTION projection = { 45.0f, 0.0f, 0.1f, 50.0f };
glm::vec3 spaceTrans = glm::vec3(0.0f, 0.0f, -2.0f);
//========================================================
char vertex[] = { "vertex.glsl" };
char fragment[] = { "fragment.glsl" };
GLuint vao, vbo[2]; 
GLuint shaderProgramID;
//========================================================
// 사용자 지정 변수
float points[] = {
	-1.f, 1.f, -1.f,
	1.f, 1.f, -1.f,
	1.f, 1.f, 1.f,
	-1.f, 1.f, 1.f, // 윗

	-1.f, 1.f, -1.f,
	-1.f, -1.f, -1.f,
	1.f, -1.f, -1.f,
	1.f, 1.f, -1.f, // 뒷

	-1.f, 1.f, -1.f,
	-1.f, 1.f, 1.f,
	-1.f, -1.f, 1.f,
	-1.f, -1.f, -1.f, // 왼

	1.f, 1.f, 1.f,
	1.f, 1.f, -1.f,
	1.f, -1.f, -1.f, 
	1.f, -1.f, 1.f, // 오

	1.f, -1.f, -1.f,
	-1.f, -1.f, -1.f,
	-1.f, -1.f, 1.f,
	1.f, -1.f, 1.f, // 아래

	0.f, 1.f, 0.f,
	0.f, -1.f, 0.f,
	1.f, -1.f, 0.f,
	1.f, 1.f, 0.f, // 좌 문

	-1.f, 1.f, 0.f,
	-1.f, -1.f, 0.f,
	0.f, -1.f, 0.f,
	0.f, 1.f, 0.f, // 우 문

	-0.05f, -0.7f, -0.05f,
	-0.05f, -0.7f, 0.05f,
	0.05f, -0.7f, 0.05f,
	0.05f, -0.7f, -0.05f, // 머리 위

	-0.05f, -0.7f, -0.05f,
	-0.05f, -0.8f, -0.05f,
	-0.05f, -0.8f, 0.05f,
	-0.05f, -0.7f, 0.05f, // 머리 왼쪽

	-0.05f, -0.8f, -0.05f,
	-0.05f, -0.7f, -0.05f,
	0.05f, -0.7f, -0.05f,
	0.05f, -0.8f, -0.05f, // 머리 뒤쪽

	0.05f, -0.7f, 0.05f,
	0.05f, -0.8f, 0.05f,
	0.05f, -0.8f, -0.05f,
	0.05f, -0.7f, -0.05f, // 머리 오른쪽

	-0.05f, -0.7f, 0.05f,
	-0.05f, -0.8f, 0.05f,
	0.05f, -0.8f, 0.05f,
	0.05f, -0.7f, 0.05f, // 머리 앞쪽

	-0.05f, -0.8f, 0.05f,
	-0.05f, -0.8f, -0.05f,
	0.05f, -0.8f, -0.05f,
	0.05f, -0.8f, 0.05f, // 머리 아래쪽

	-0.05f, -0.8f, -0.025f,
	-0.05f, -0.8f, 0.025f,
	0.05f, -0.8f, 0.025f,
	0.05f, -0.8f, -0.025f, // 몸통 위

	-0.05f, -0.8f, -0.025f,
	-0.05f, -0.9f, -0.025f,
	-0.05f, -0.9f, 0.025f,
	-0.05f, -0.8f, 0.025f, // 몸통 왼쪽

	-0.05f, -0.9f, -0.025f,
	-0.05f, -0.8f, -0.025f,
	0.05f, -0.8f, -0.025f,
	0.05f, -0.9f, -0.025f, // 몸통 뒤쪽

	0.05f, -0.8f, 0.025f,
	0.05f, -0.9f, 0.025f,
	0.05f, -0.9f, -0.025f,
	0.05f, -0.8f, -0.025f, // 몸통 오른쪽

	-0.05f, -0.8f, 0.025f,
	-0.05f, -0.9f, 0.025f,
	0.05f, -0.9f, 0.025f,
	0.05f, -0.8f, 0.025f, // 몸통 앞쪽

	-0.05f, -0.9f, 0.025f,
	-0.05f, -0.9f, -0.025f,
	0.05f, -0.9f, -0.025f,
	0.05f, -0.9f, 0.025f, // 몸통 아래쪽

	-0.07f, -0.8f, -0.025f,
	-0.07f, -0.8f, 0.025f,
	-0.05f, -0.8f, 0.025f,
	-0.05f, -0.8f, -0.025f, // 오른팔 위

	-0.07f, -0.8f, -0.025f,
	-0.07f, -0.9f, -0.025f,
	-0.07f, -0.9f, 0.025f,
	-0.07f, -0.8f, 0.025f, // 오른팔 왼쪽

	-0.07f, -0.9f, -0.025f,
	-0.07f, -0.8f, -0.025f,
	-0.05f, -0.8f, -0.025f,
	-0.05f, -0.9f, -0.025f, // 오른팔 뒤쪽

	-0.05f, -0.8f, 0.025f,
	-0.05f, -0.9f, 0.025f,
	-0.05f, -0.9f, -0.025f,
	-0.05f, -0.8f, -0.025f, // 오른팔 오른쪽

	-0.07f, -0.8f, 0.025f,
	-0.07f, -0.9f, 0.025f,
	-0.05f, -0.9f, 0.025f,
	-0.05f, -0.8f, 0.025f, // 오른팔 앞쪽

	-0.07f, -0.9f, 0.025f,
	-0.05f, -0.9f, -0.025f,
	-0.05f, -0.9f, -0.025f,
	-0.07f, -0.9f, 0.025f, // 오른팔 아래쪽

	0.07f, -0.8f, -0.025f,
	0.07f, -0.8f, 0.025f,
	0.05f, -0.8f, 0.025f,
	0.05f, -0.8f, -0.025f, // 왼팔 위

	0.07f, -0.8f, -0.025f,
	0.07f, -0.9f, -0.025f,
	0.07f, -0.9f, 0.025f,
	0.07f, -0.8f, 0.025f, // 왼팔 왼쪽

	0.07f, -0.9f, -0.025f,
	0.07f, -0.8f, -0.025f,
	0.05f, -0.8f, -0.025f,
	0.05f, -0.9f, -0.025f, // 왼팔 뒤쪽

	0.05f, -0.8f, 0.025f,
	0.05f, -0.9f, 0.025f,
	0.05f, -0.9f, -0.025f,
	0.05f, -0.8f, -0.025f, // 왼팔 오른쪽

	0.07f, -0.8f, 0.025f,
	0.07f, -0.9f, 0.025f,
	0.05f, -0.9f, 0.025f,
	0.05f, -0.8f, 0.025f, // 왼팔 앞쪽

	0.07f, -0.9f, 0.025f,
	0.05f, -0.9f, -0.025f,
	0.05f, -0.9f, -0.025f,
	0.07f, -0.9f, 0.025f, // 왼팔 아래쪽

	-0.05f, -0.9f, -0.025f,
	-0.05f, -0.9f, 0.025f,
	0.f, -0.9f, 0.025f,
	0.f, -0.9f, -0.025f, // 오른다리 위

	-0.05f, -0.9f, -0.025f,
	-0.05f, -1.f, -0.025f,
	-0.05f, -1.f, 0.025f,
	-0.05f, -0.9f, 0.025f, // 오른다리 왼쪽

	-0.05f, -1.f, -0.025f,
	-0.05f, -0.9f, -0.025f,
	0.f, -0.9f, -0.025f,
	0.f, -1.f, -0.025f, // 오른다리 뒤쪽

	0.f, -0.9f, 0.025f,
	0.f, -1.f, 0.025f,
	0.f, -1.f, -0.025f,
	0.f, -0.9f, -0.025f, // 오른다리 오른쪽

	-0.05f, -0.9f, 0.025f,
	-0.05f, -1.f, 0.025f,
	0.f, -1.f, 0.025f,
	0.f, -0.9f, 0.025f, // 오른다리 앞쪽

	-0.05f, -1.f, 0.025f,
	-0.05f, -1.f, -0.025f,
	0.f, -1.f, -0.025f,
	0.f, -1.f, 0.025f, // 오른다리 아래쪽

	0.f, -0.9f, -0.025f,
	0.f, -0.9f, 0.025f,
	0.05f, -0.9f, 0.025f,
	0.05f, -0.9f, -0.025f, // 왼다리 위

	0.f, -0.9f, -0.025f,
	0.f, -1.f, -0.025f,
	0.f, -1.f, 0.025f,
	0.f, -0.9f, 0.025f, // 왼다리 왼쪽

	0.f, -1.f, -0.025f,
	0.f, -0.9f, -0.025f,
	0.05f, -0.9f, -0.025f,
	0.05f, -1.f, -0.025f, // 왼다리 뒤쪽

	0.05f, -0.9f, 0.025f,
	0.05f, -1.f, 0.025f,
	0.05f, -1.f, -0.025f,
	0.05f, -0.9f, -0.025f, // 왼다리 오른쪽

	0.f, -0.9f, 0.025f,
	0.f, -1.f, 0.025f,
	0.05f, -1.f, 0.025f,
	0.05f, -0.9f, 0.025f, // 왼다리 앞쪽

	0.f, -1.f, 0.025f,
	0.f, -1.f, -0.025f,
	0.05f, -1.f, -0.025f,
	0.05f, -1.f, 0.025f, // 왼다리 아래쪽

	0.f, -0.75f, 0.05f,
	-0.01f, -0.775f, 0.05f,
	0.f, -0.775f, 0.07f, // 코 왼쪽

	0.f, -0.75f, 0.05f,
	0.01f, -0.775f, 0.05f,
	0.f, -0.775f, 0.07f, // 코 오른쪽

	-0.01f, -0.775f, 0.05f,
	0.01f, -0.775f, 0.05f,
	0.f, -0.775f, 0.07f, // 코 밑

	-0.2f, 0.2f, -0.2f,
	-0.2f, 0.2f, 0.2f,
	0.2f, 0.2f, 0.2f,
	0.2f, 0.2f, -0.2f,

	-0.2f, 0.2f, -0.2f,
	0.2f, 0.2f, -0.2f,
	0.2f, 0.f, -0.2f,
	-0.2f, 0.f, -0.2f,

	-0.2f, 0.2f, 0.2f,
	-0.2f, 0.2f, -0.2f,
	-0.2f, 0.f, -0.2f,
	-0.2f, 0.f, 0.2f,

	-0.2f, 0.2f, 0.2f,
	-0.2f, 0.f, 0.2f,
	0.2f, 0.f, 0.2f,
	0.2f, 0.2f, 0.2f,

	0.2f, 0.2f, 0.2f,
	0.2f, 0.f, 0.2f,
	0.2f, 0.f, -0.2f,
	0.2f, 0.2f, -0.2f,

	-0.2f, 0.2f, -0.2f,
	-0.2f, 0.2f, 0.2f,
	0.2f, 0.2f, 0.2f,
	0.2f, 0.2f, -0.2f,

	-0.2f, 0.2f, -0.2f,
	0.2f, 0.2f, -0.2f,
	0.2f, 0.f, -0.2f,
	-0.2f, 0.f, -0.2f,

	-0.2f, 0.2f, 0.2f,
	-0.2f, 0.2f, -0.2f,
	-0.2f, 0.f, -0.2f,
	-0.2f, 0.f, 0.2f,

	-0.2f, 0.2f, 0.2f,
	-0.2f, 0.f, 0.2f,
	0.2f, 0.f, 0.2f,
	0.2f, 0.2f, 0.2f,

	0.2f, 0.2f, 0.2f,
	0.2f, 0.f, 0.2f,
	0.2f, 0.f, -0.2f,
	0.2f, 0.2f, -0.2f,
	//
	-0.5f, 0.1f, -0.2f,
	-0.5f, 0.1f, 0.2f,
	0.5f, 0.1f, 0.2f,
	0.5f, 0.1f, -0.2f,

	-0.5f, 0.1f, -0.2f,
	0.5f, 0.1f, -0.2f,
	0.5f, 0.f, -0.2f,
	-0.5f, 0.f, -0.2f,

	-0.5f, 0.1f, 0.2f,
	-0.5f, 0.1f, -0.2f,
	-0.5f, 0.f, -0.2f,
	-0.5f, 0.f, 0.2f,

	-0.5f, 0.1f, 0.2f,
	-0.5f, 0.f, 0.2f,
	0.5f, 0.f, 0.2f,
	0.5f, 0.1f, 0.2f,

	0.5f, 0.1f, 0.2f,
	0.5f, 0.f, 0.2f,
	0.5f, 0.f, -0.2f,
	0.5f, 0.1f, -0.2f,

	-0.5f, 0.f, -0.2f,
	0.5f, 0.f, -0.2f,
	0.5f, 0.f, 0.2f,
	-0.5f, 0.f, 0.2f
};
float color[] = {
	0, 1, 1, 
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,

	0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,

	1, 0, 1,
	1, 0, 1,
	1, 0, 1,
	1, 0, 1,

	1, 1, 1,
	1, 1, 1,
	1, 1, 1,
	1, 1, 1,

	1, 1, 0,
	1, 1, 0,
	1, 1, 0,
	1, 1, 0,

	1, 0, 0, 
	0, 1, 0, 
	0, 0, 1, 
	0, 1, 1, 

	0, 1, 1,
	0, 0, 1,
	0, 1, 0,
	1, 0, 0,

	0, 0, 1,
	0, 0, 1,
	0, 0, 1,
	0, 0, 1,

	0, 0, 1,
	0, 0, 1,
	0, 0, 1,
	0, 0, 1,

	0, 0, 1,
	0, 0, 1,
	0, 0, 1,
	0, 0, 1,

	0, 0, 1,
	0, 0, 1,
	0, 0, 1,
	0, 0, 1,

	0, 0, 1,
	0, 0, 1,
	0, 0, 1,
	0, 0, 1,

	0, 0, 1,
	0, 0, 1,
	0, 0, 1,
	0, 0, 1,

	0, 1, 0,
	0, 1, 0,
	0, 1, 0,
	0, 1, 0,

	0, 1, 0,
	0, 1, 0,
	0, 1, 0,
	0, 1, 0,

	0, 1, 0,
	0, 1, 0,
	0, 1, 0,
	0, 1, 0,

	0, 1, 0,
	0, 1, 0,
	0, 1, 0,
	0, 1, 0,

	0, 1, 0,
	0, 1, 0,
	0, 1, 0,
	0, 1, 0,

	0, 1, 0,
	0, 1, 0,
	0, 1, 0,
	0, 1, 0,

	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,

	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,

	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,

	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,

	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,

	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
		
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,

	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,

	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,

	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,

	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,

	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
		
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,

	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,

	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,

	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,

	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,

	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
		
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,

	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,

	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,

	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,

	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,

	0, 1, 1,
	0, 1, 1,
	0, 1, 1,
	0, 1, 1,

	0, 0, 0,
	0, 0, 0,
	0, 0, 0,

	0, 0, 0,
	0, 0, 0,
	0, 0, 0,

	0, 0, 0,
	0, 0, 0,
	0, 0, 0,
	//
	0.25f, 0.25f, 0.25f,
	0.25f, 0.25f, 0.25f,
	0.25f, 0.25f, 0.25f,
	0.25f, 0.25f, 0.25f,

	0.25f, 0.25f, 0.25f,
	0.25f, 0.25f, 0.25f,
	0.25f, 0.25f, 0.25f,
	0.25f, 0.25f, 0.25f,

	0.25f, 0.25f, 0.25f,
	0.25f, 0.25f, 0.25f,
	0.25f, 0.25f, 0.25f,
	0.25f, 0.25f, 0.25f,

	0.25f, 0.25f, 0.25f,
	0.25f, 0.25f, 0.25f,
	0.25f, 0.25f, 0.25f,
	0.25f, 0.25f, 0.25f,

	0.25f, 0.25f, 0.25f,
	0.25f, 0.25f, 0.25f,
	0.25f, 0.25f, 0.25f,
	0.25f, 0.25f, 0.25f,
	//
	0.75f, 0.75f, 0.75f,
	0.75f, 0.75f, 0.75f,
	0.75f, 0.75f, 0.75f,
	0.75f, 0.75f, 0.75f,

	0.75f, 0.75f, 0.75f,
	0.75f, 0.75f, 0.75f,
	0.75f, 0.75f, 0.75f,
	0.75f, 0.75f, 0.75f,

	0.75f, 0.75f, 0.75f,
	0.75f, 0.75f, 0.75f,
	0.75f, 0.75f, 0.75f,
	0.75f, 0.75f, 0.75f,

	0.75f, 0.75f, 0.75f,
	0.75f, 0.75f, 0.75f,
	0.75f, 0.75f, 0.75f,
	0.75f, 0.75f, 0.75f,

	0.75f, 0.75f, 0.75f,
	0.75f, 0.75f, 0.75f,
	0.75f, 0.75f, 0.75f,
	0.75f, 0.75f, 0.75f,
	//
	0.f, 0.5f, 0.5f,
	0.f, 0.5f, 0.5f,
	0.f, 0.5f, 0.5f,
	0.f, 0.5f, 0.5f,

	0.f, 0.5f, 0.5f,
	0.f, 0.5f, 0.5f,
	0.f, 0.5f, 0.5f,
	0.f, 0.5f, 0.5f,

	0.f, 0.5f, 0.5f,
	0.f, 0.5f, 0.5f,
	0.f, 0.5f, 0.5f,
	0.f, 0.5f, 0.5f,

	0.f, 0.5f, 0.5f,
	0.f, 0.5f, 0.5f,
	0.f, 0.5f, 0.5f,
	0.f, 0.5f, 0.5f,

	0.f, 0.5f, 0.5f,
	0.f, 0.5f, 0.5f,
	0.f, 0.5f, 0.5f,
	0.f, 0.5f, 0.5f,

	0.f, 0.5f, 0.5f,
	0.f, 0.5f, 0.5f,
	0.f, 0.5f, 0.5f,
	0.f, 0.5f, 0.5f
};

GLUquadric* qobj;

float obstacle1[4] = {-1.f, -0.2f, -0.6f, 0.2f}; // 장애물 xz좌표
float obstacle2[4] = {0.3f, 0.3f, 0.7f, 0.7f};
float obstacle3[4] = { -0.4f, 0.6f, 0.f, 1.f };

glm::vec3 transObstacle = glm::vec3(-0.8f, -1.f, 0.f);
glm::vec3 transObstacle1 = glm::vec3(0.5f, -1.f, 0.5f);
glm::vec3 transObstacle2 = glm::vec3(-0.2f, -1.f, 0.8f);

glm::vec3 floor_trans[25];
glm::vec3 pillar_locate[4] = {
	glm::vec3(-0.8f, -1.f, -0.8f),glm::vec3(0.8f, -1.f, -0.8f),
	glm::vec3(-0.8f, -1.f, 0.8f),glm::vec3(0.8f, -1.f, 0.8f)
};
glm::vec3 obstacle_pillar_locate[2] = {
	glm::vec3(-0.4f, -1.f, 0.f), glm::vec3(0.4f, -1.f, 0.f)
};

glm::vec3 transCamera = glm::vec3(0.0f, 0.0f, 2.0f);
glm::vec3 rotateCamera = glm::vec3(0, 0, 0);
glm::vec3 open = glm::vec3(0, 0, 0);

GLint random_fall[5];
GLint random_freefall;

GLfloat rotateValue = 1.f;
GLfloat openValue;
GLfloat armLegMotionValue = 5.f;
GLfloat robotJumpValue = 0.05f;
GLfloat speedValue = 0.01f;

GLboolean timeSwitch = FALSE;
GLboolean fFlag[5];
GLboolean rFlag = FALSE;
GLboolean oFlag = FALSE;
GLboolean jFlag = FALSE;
GLboolean jsFlag = FALSE;
GLboolean alFlag = TRUE;
GLboolean sFlag = FALSE;
//========================================================
GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid SpecialKeyboard(int key, int x, int y);
GLvoid TimerFunction(int value);
void make_shaderProgram();
void InitBuffer_();
//========================================================
// 사용자 지정 함수
GLvoid cameraTranslation(glm::vec3 cameraTrans, glm::vec3 cameraRotate);

GLvoid armLegMotion(ROBOT* robot);
glm::mat4 armMotion(glm::vec3 motionValue);
glm::mat4 legMotion(glm::vec3 motionValue);

GLvoid robotJump();
GLvoid smallRobotJump();
GLvoid touchWall(ROBOT* robot);
GLboolean touchPillar(glm::vec3 locate);
GLboolean touchObstaclePillar(glm::vec3 locate);
GLboolean touchObstaclePlate();
GLboolean touchFloor(GLint number);

GLvoid robotMove(ROBOT* robot);
GLvoid small_robot_move(ROBOT* small_robot);
GLvoid small_robot_move2(ROBOT* small_robot2);
GLvoid small_robot_move3(ROBOT* small_robot3);
GLvoid resetAll();
GLvoid draw_robot(ROBOT robot, unsigned int transformLocation);
GLvoid draw_floor(unsigned int transformLocation);
GLvoid draw_pillar(unsigned int transformLocation);
GLvoid draw_obstacle(unsigned int transformLocation);

GLvoid fall_floor(GLint number, GLint count);


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

	resetAll();
	robot.moveDir = 2;
	for (int i = 0; i < 5; i++) {
		random_fall[i] = rand() % 25;
	}
	random_freefall = rand() % 5;

	make_shaderProgram();
	InitBuffer_();
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
	cameraTranslation(transCamera, rotateCamera);

	unsigned int transformLocation = glGetUniformLocation(shaderProgramID, "model");

	glm::mat4 box = glm::mat4(1.0f);
	box = translation_shape(glm::vec3(0.f, 0.f, 0.f));
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(box));
	glDrawArrays(GL_QUADS, 0, 16);

	glm::mat4 openLeftDoor = glm::mat4(1.0f);
	openLeftDoor = translation_shape(glm::vec3(-1.f, 0.f, 1.f)) * rotate_shape(open);
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(openLeftDoor));
	glDrawArrays(GL_QUADS, 20, 4);

	glm::mat4 openRightDoor = glm::mat4(1.0f);
	openRightDoor = translation_shape(glm::vec3(1.f, 0.f, 1.f)) * rotate_shape(-open);
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(openRightDoor));
	glDrawArrays(GL_QUADS, 24, 4);

	draw_robot(robot, transformLocation);
	if (sFlag) {
		draw_robot(small_robot[0], transformLocation);
		draw_robot(small_robot[1], transformLocation);
		draw_robot(small_robot[2], transformLocation);
	}

	draw_pillar(transformLocation);
	draw_floor(transformLocation);
	draw_obstacle(transformLocation);
	


	glUseProgram(shaderProgramID);

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

void InitBuffer_() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(2, vbo); 

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(points), points, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(color), color, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'j':
		if (timeSwitch) 
			if (!jFlag) {
				jFlag = TRUE;
				for (int i = 0; i < 5; i++) {
					if (touchFloor(random_fall[i]) && floor_trans[random_fall[i]].y == -1.2f) {
						fFlag[i] = TRUE;
					}
				}
			}
		break;
	case 'w': case 'W':
		robot.dir.y = 180.f;
		robot.moveDir = 0;
		break;
	case 'a': case 'A':
		robot.dir.y = -90.f;
		robot.moveDir = 3;
		break;
	case 's': case 'S':
		robot.dir.y = 0.f;
		robot.moveDir = 2;
		break;
	case 'd': case 'D':
		robot.dir.y = 90.f;
		robot.moveDir = 1;
		break;
	case 't': case 'T':
		if (sFlag) sFlag = FALSE;
		else sFlag = TRUE;
		break;
	case 'f': case 'F':
		if (timeSwitch)
			if (!jsFlag) {
				jsFlag = TRUE;
			}
		break;
	case 'i':
		resetAll();
		break;
	case '+':
		if (speedValue < 0.3f) {
			speedValue += 0.001f;
			if(armLegMotionValue < 10.0f)armLegMotionValue += 0.01f;
		}
		break;
	case '-':
		if (speedValue > 0.002f) {
			speedValue -= 0.001f;
			if(armLegMotionValue > 0.02f)armLegMotionValue -= 0.01f;
		}
		break;
	case 'o':
		if (open.y > -90.f) {
			openValue = -1.f;

			if (!oFlag) oFlag = TRUE;


			if (!timeSwitch) {
				timeSwitch = TRUE;
				glutTimerFunc(10, TimerFunction, 1);
			}
		}
		break;
	case 'O':
		if (open.y < 0.f) {
			openValue = 1.f;

			if (!oFlag) oFlag = TRUE;


			if (!timeSwitch) {
				timeSwitch = TRUE;
				glutTimerFunc(10, TimerFunction, 1);
			}
		}
		break;
	case 'y':
		rotateValue = 1;

		if (!rFlag) rFlag = TRUE;

		if (!timeSwitch) {
			timeSwitch = TRUE;
			glutTimerFunc(10, TimerFunction, 1);
		}
		break;
	case 'Y':
		rotateValue = -1;

		if (!rFlag) rFlag = TRUE;

		if (!timeSwitch) {
			timeSwitch = TRUE;
			glutTimerFunc(10, TimerFunction, 1);
		}
		break;
	case 'q': case 'Q':
		glutLeaveMainLoop();
	}
	glutPostRedisplay();
}

GLvoid SpecialKeyboard(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		transCamera.x -= 0.1f;
		break;
	case GLUT_KEY_RIGHT:
		transCamera.x += 0.1f;
		break;
	case GLUT_KEY_UP:
		transCamera.z += 0.1f;
		break;
	case GLUT_KEY_DOWN:
		transCamera.z -= 0.1f;
		break;
	}
	glutPostRedisplay();
}

GLvoid TimerFunction(int value) {
	glutPostRedisplay();

	armLegMotion(&robot);
	robotMove(&robot);

	small_robot[0].left_motion = robot.left_motion;
	small_robot[0].right_motion = robot.right_motion;
	small_robot_move(&small_robot[0]);

	small_robot[1].left_motion = robot.left_motion;
	small_robot[1].right_motion = robot.right_motion;
	small_robot_move2(&small_robot[1]);

	small_robot[2].left_motion = robot.left_motion;
	small_robot[2].right_motion = robot.right_motion;
	small_robot_move3(&small_robot[2]);

	if (jFlag) robotJump();
	if (jsFlag) smallRobotJump();

	if (oFlag) {
		open.y += openValue;
		if (open.y == 0.f || open.y == -90.f) oFlag = FALSE;
	}
	if(rFlag) rotateCamera.y += rotateValue;

	for (int i = 0; i < 5; i++) {
		if(fFlag[i]) fall_floor(random_fall[i], i);
	}

	if (timeSwitch) glutTimerFunc(10, TimerFunction, 1);
}

GLvoid cameraTranslation(glm::vec3 cameraTrans, glm::vec3 cameraRotate) {
	glm::vec3 zeroPoint = glm::vec3(0, 0, 0);
	glm::mat4 view = glm::mat4(1.0f);
	view = camera_locate(cameraTrans, zeroPoint) * rotate_camera(cameraRotate);

	unsigned int viewLocation = glGetUniformLocation(shaderProgramID, "view");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
}

GLvoid armLegMotion(ROBOT* robot) {

	robot->left_motion.x += armLegMotionValue;
	robot->right_motion.x -= armLegMotionValue;

	if ((robot->left_motion.x > speedValue * 4500.f && alFlag) || (robot->left_motion.x < speedValue * -4500.f && !alFlag)) {
		armLegMotionValue *= -1;

		alFlag = (alFlag + 1) % 2;
	}
}

glm::mat4 armMotion(glm::vec3 motionValue) {
	glm::mat4 motion = glm::mat4(1.0f);
	motion = translation_shape(glm::vec3(0.f, -0.8f, 0.0f)) * rotate_shape(motionValue) * translation_shape(glm::vec3(0.f, 0.8f, 0.0f));
	return motion;
}

glm::mat4 legMotion(glm::vec3 motionValue) {
	glm::mat4 motion = glm::mat4(1.0f);
	motion = translation_shape(glm::vec3(0.f, -0.9f, 0.0f)) * rotate_shape(motionValue) * translation_shape(glm::vec3(0.f, 0.9f, 0.0f));
	return motion;
}

GLvoid robotJump() {
	robot.move.y += robotJumpValue;

	if (robot.move.y > 1.f)
		robotJumpValue *= -1;
	else if(touchObstaclePlate()){
		robot.move.y = 0.6f;
		robotJumpValue *= -1;
		jFlag = FALSE;
	}
	else if (robot.move.y < 0.f) {
		robot.move.y = 0.0f;
		robotJumpValue *= -1;
		jFlag = FALSE;
	}
}

GLvoid smallRobotJump() {
	small_robot[0].move.y += robotJumpValue;

	if (small_robot[0].move.y > 1.f)
		robotJumpValue *= -1;
	if (small_robot[0].move.y < -0.25f) {
		small_robot[0].move.y = 0.0f;
		robotJumpValue *= -1;
		jsFlag = FALSE;
	}
}

GLvoid touchWall(ROBOT* robot) {
	if (robot->move.x > 1.0f || robot->move.x < -1.0f || robot->move.z > 1.0f || robot->move.z < -1.0f) {
		robot->moveDir = (robot->moveDir + 2) % 4;
		robot->dir.y += 180.f;
		return;
	}
	else if (touchPillar(pillar_locate[0]) || touchPillar(pillar_locate[1]) || touchPillar(pillar_locate[2]) || touchPillar(pillar_locate[3])) {
		robot->moveDir = (robot->moveDir + 2) % 4;
		robot->dir.y += 180.f;
		return;
	}
	else if (touchObstaclePillar(obstacle_pillar_locate[0]) || touchObstaclePillar(obstacle_pillar_locate[1])) {
		robot->moveDir = (robot->moveDir + 2) % 4;
		robot->dir.y += 180.f;
		return;
	}
	else if (robot->move.y > 0.5f && jFlag && touchObstaclePlate()) {
		robot->moveDir = (robot->moveDir + 2) % 4;
		robot->dir.y += 180.f;
		return;
	}
}

GLboolean touchPillar(glm::vec3 locate) {
	GLfloat minX, maxX, minZ, maxZ;

	minX = locate.x - 0.2f;	maxX = locate.x + 0.2f;
	minZ = locate.z - 0.2f; maxZ = locate.z + 0.2f;

	if (robot.move.x > maxX) return FALSE;
	if (robot.move.x < minX) return FALSE;
	if (robot.move.z > maxZ) return FALSE;
	if (robot.move.z < minZ) return FALSE;

	return TRUE;
}

GLboolean touchObstaclePillar(glm::vec3 locate) {
	GLfloat minX, maxX, minZ, maxZ;

	minX = locate.x - 0.1f;	maxX = locate.x + 0.1f;
	minZ = locate.z - 0.1f; maxZ = locate.z + 0.1f;

	if (robot.move.x > maxX) return FALSE;
	if (robot.move.x < minX) return FALSE;
	if (robot.move.z > maxZ) return FALSE;
	if (robot.move.z < minZ) return FALSE;

	return TRUE;
}

GLboolean touchObstaclePlate() {
	if (robot.move.x > 0.5f) return FALSE;
	if (robot.move.x < -0.5f) return FALSE;
	if (robot.move.z > 0.2f) return FALSE;
	if (robot.move.z < -0.2f) return FALSE;

	return TRUE;
}

GLboolean touchFloor(GLint number) {
	GLfloat minX, maxX, minZ, maxZ;

	minX = floor_trans[number].x - 0.2f; maxX = floor_trans[number].x + 0.2f;
	minZ = floor_trans[number].z - 0.2f; maxZ = floor_trans[number].z + 0.2f;

	if (robot.move.x > maxX) return FALSE;
	if (robot.move.x < minX) return FALSE;
	if (robot.move.z > maxZ) return FALSE;
	if (robot.move.z < minZ) return FALSE;

	return TRUE;
}

GLvoid robotMove(ROBOT* robot) {

	switch (robot->moveDir) {
	case 0: // up
		robot->move.z -= speedValue;
		break;
	case 1: // right
		robot->move.x += speedValue;
		break;
	case 2: // down
		robot->move.z += speedValue;
		break;
	case 3: // left
		robot->move.x -= speedValue;
		break;
	}
	if (!jFlag && !(touchObstaclePlate())) {
		for (int i = 0; i < 25; i++) {
			if(touchFloor(i))
				robot->move.y = floor_trans[i].y + 1.2f;
		}
		
	}
	touchWall(robot);

	robot->count++;

	if (robot->count < 20)
		origin_robot[robot->count] = *robot;
	else
		origin_robot[19] = *robot;
}

GLvoid small_robot_move(ROBOT* small_robot) {
	if (robot.count > 20) {
		small_robot->move.x = origin_robot[0].move.x;
		small_robot->move.z = origin_robot[0].move.z;
		if(!jsFlag) small_robot->move.y = origin_robot[0].move.y - 0.25f;

		small_robot->dir.y = origin_robot[0].dir.y;
		small_robot->moveDir = origin_robot[0].moveDir;

		for (int i = 0; i < 19; i++) {
			origin_robot[i] = origin_robot[i + 1];
		}

		small_robot->count++;

		if (small_robot->count < 20)
			origin_small_robot[small_robot->count] = *small_robot;
		else
			origin_small_robot[19] = *small_robot;
	}
}

GLvoid small_robot_move2(ROBOT* small_robot2) {
	if (small_robot[0].count > 20) {
		small_robot2->move.x = origin_small_robot[0].move.x;
		small_robot2->move.z = origin_small_robot[0].move.z;
		small_robot2->move.y = origin_small_robot[0].move.y - 0.25f;

		small_robot2->dir.y = origin_small_robot[0].dir.y;
		small_robot2->moveDir = origin_small_robot[0].moveDir;

		for (int i = 0; i < 19; i++) {
			origin_small_robot[i] = origin_small_robot[i + 1];
		}

		small_robot2->count++;

		if (small_robot2->count < 20)
			origin_small_robot2[small_robot2->count] = *small_robot2;
		else
			origin_small_robot2[19] = *small_robot2;
	}
}

GLvoid small_robot_move3(ROBOT* small_robot3) {
	if (small_robot[1].count > 20) {
		small_robot3->move.x = origin_small_robot2[0].move.x;
		small_robot3->move.z = origin_small_robot2[0].move.z;
		small_robot3->move.y = origin_small_robot2[0].move.y - 0.25f;

		small_robot3->dir.y = origin_small_robot2[0].dir.y;
		small_robot3->moveDir = origin_small_robot2[0].moveDir;

		for (int i = 0; i < 19; i++) {
			origin_small_robot2[i] = origin_small_robot2[i + 1];
		}
	}
}

GLvoid resetAll() {
	robot.move = glm::vec3(0.f, 0.f, 0.f);
	robot.dir = glm::vec3(0.f, 0.f, 0.f);
	robot.moveDir = 2;
	robot.left_motion = glm::vec3(0.f, 0.f, 0.f);
	robot.right_motion = glm::vec3(0.f, 0.f, 0.f);
	robot.size = glm::vec3(1.f, 1.f, 1.f);

	small_robot[0].move = glm::vec3(0.f, -0.25f, 0.f);
	small_robot[0].dir = glm::vec3(0.f, 0.f, 0.f);
	small_robot[0].moveDir = 2;
	small_robot[0].left_motion = glm::vec3(0.f, 0.f, 0.f);
	small_robot[0].right_motion = glm::vec3(0.f, 0.f, 0.f);
	small_robot[0].size = glm::vec3(0.75f, 0.75f, 0.75f);

	small_robot[1].move = glm::vec3(0.f, -0.5f, 0.f);
	small_robot[1].dir = glm::vec3(0.f, 0.f, 0.f);
	small_robot[1].moveDir = 2;
	small_robot[1].left_motion = glm::vec3(0.f, 0.f, 0.f);
	small_robot[1].right_motion = glm::vec3(0.f, 0.f, 0.f);
	small_robot[1].size = glm::vec3(0.5f, 0.5f, 0.5f);

	small_robot[2].move = glm::vec3(0.f, -0.75f, 0.f);
	small_robot[2].dir = glm::vec3(0.f, 0.f, 0.f);
	small_robot[2].moveDir = 2;
	small_robot[2].left_motion = glm::vec3(0.f, 0.f, 0.f);
	small_robot[2].right_motion = glm::vec3(0.f, 0.f, 0.f);
	small_robot[2].size = glm::vec3(0.25f, 0.25f, 0.25f);

	transCamera = glm::vec3(0.0f, 0.0f, 2.0f);
	rotateCamera = glm::vec3(0, 0, 0);

	speedValue = 0.01f;
	armLegMotionValue = 5.0f;

	alFlag = TRUE;

	for (int i = 0; i < 25; i++) {
		floor_trans[i].x = -0.8f + (0.4f * (i % 5));
		floor_trans[i].y = -1.2f;
		floor_trans[i].z = -0.8f + (0.4f * (i / 5));
	}
}

GLvoid draw_robot(ROBOT robot, unsigned int transformLocation) {
	glm::mat4 robotHead = glm::mat4(1.0f);
	robotHead = translation_shape(robot.move) * rotate_shape(robot.dir) * scaling_shape(robot.size);
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(robotHead));
	glDrawArrays(GL_QUADS, 28, 24);

	glm::mat4 robotBody = glm::mat4(1.0f);
	robotBody = translation_shape(robot.move) * rotate_shape(robot.dir) * scaling_shape(robot.size);
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(robotBody));
	glDrawArrays(GL_QUADS, 52, 24);

	glm::mat4 robotRightArm = glm::mat4(1.0f);
	robotRightArm = translation_shape(robot.move) * rotate_shape(robot.dir) * scaling_shape(robot.size) * armMotion(robot.right_motion);
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(robotRightArm));
	glDrawArrays(GL_QUADS, 76, 24);

	glm::mat4 robotLeftArm = glm::mat4(1.0f);
	robotLeftArm = translation_shape(robot.move) * rotate_shape(robot.dir) * scaling_shape(robot.size) * armMotion(robot.left_motion);
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(robotLeftArm));
	glDrawArrays(GL_QUADS, 100, 24);

	glm::mat4 robotRightLeg = glm::mat4(1.0f);
	robotRightLeg = translation_shape(robot.move) * rotate_shape(robot.dir) * scaling_shape(robot.size) * legMotion(robot.right_motion);
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(robotRightLeg));
	glDrawArrays(GL_QUADS, 124, 24);

	glm::mat4 robotLeftLeg = glm::mat4(1.0f);
	robotLeftLeg = translation_shape(robot.move) * rotate_shape(robot.dir) * scaling_shape(robot.size) * legMotion(robot.left_motion);
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(robotLeftLeg));
	glDrawArrays(GL_QUADS, 148, 24);

	glm::mat4 nose = glm::mat4(1.0f);
	nose = translation_shape(robot.move) * rotate_shape(robot.dir) * scaling_shape(robot.size);
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(nose));
	glDrawArrays(GL_TRIANGLES, 172, 9);
}

GLvoid draw_floor(unsigned int transformLocation) {
	for (int i = 0; i < 25; i++) {
		glm::mat4 floor_mat = glm::mat4(1.f);
		floor_mat = translation_shape(floor_trans[i]);
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(floor_mat));
		i % 2 ? glDrawArrays(GL_QUADS, 181, 20) : glDrawArrays(GL_QUADS, 201, 20);
	}
}

GLvoid draw_pillar(unsigned int transformLocation) {
	for (int i = 0; i < 4; i++) {
		qobj = gluNewQuadric();
		gluQuadricDrawStyle(qobj, GLU_FILL);

		glm::mat4 pillar = glm::mat4(1.f);
		pillar = translation_shape(pillar_locate[i]) * rotate_shape(glm::vec3(-90.f, 0.f, 0.f));
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(pillar));

		gluCylinder(qobj, 0.2f, 0.2f, 2.f, 20, 8); 
	}
}

GLvoid draw_obstacle(unsigned int transformLocation) {
	for (int i = 0; i < 2; i++) {
		qobj = gluNewQuadric();
		gluQuadricDrawStyle(qobj, GLU_FILL);

		glm::mat4 pillar = glm::mat4(1.f);
		pillar = translation_shape(obstacle_pillar_locate[i]) * rotate_shape(glm::vec3(-90.f, 0.f, 0.f));
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(pillar));

		gluCylinder(qobj, 0.1f, 0.1f, 0.5f, 20, 8);
	}

	glm::mat4 plate = glm::mat4(1.0f);
	plate = translation_shape(glm::vec3(0.f, -0.5f, 0.f));
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(plate));
	glDrawArrays(GL_QUADS, 221, 24);
}

GLvoid fall_floor(GLint number, GLint count) {
	floor_trans[number].y -= 0.001f;
	if (floor_trans[number].y < -1.4f && count != random_freefall) fFlag[count] = FALSE;
	else if (floor_trans[number].y < -2.0f && count == random_freefall) {
		fFlag[count] = FALSE;
		floor_trans[number].y = -9999.0f;
	}
}
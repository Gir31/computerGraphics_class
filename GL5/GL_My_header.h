#pragma once
#define _CRT_SECURE_NO_WARNINGS

#define WIDTH 1000.0
#define HEIGHT 600.0

#include <stdlib.h>
#include <stdio.h>
#include <random>
#include <iostream>
#include <gl/glew.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include "GL_My_header.h"

typedef struct {
	GLfloat Fovy, Aspect, Near, Far;
}PROJECTION;

typedef struct {
	GLuint VAO, VBO, VBO_pos, EBO;
}BUFFER;

GLuint vertexShader;
GLuint fragmentShader;


extern char* filetobuf(const char* file);

extern void make_VertexShaders(char* fileName);
extern void make_FragmentShaders(char* fileName);
extern GLuint make_ShaderProgram();

extern void InitBuffer(BUFFER* buffer, glm::vec3* vPositionList, GLuint index[][3], glm::vec3* color, GLuint vSize, GLuint iSize, GLuint cSize);

extern GLfloat conversion_x(GLfloat x);
extern GLfloat conversion_y(GLfloat y);

//=========================================================================================================================//
// �𵨸���ȯ
extern glm::mat4 rotate_shape(glm::vec3 degree);
extern glm::mat4 translation_shape(glm::vec3 trans);
extern glm::mat4 scaling_shape(glm::vec3 scale);
//=========================================================================================================================//
// ���׺�ȯ
extern glm::mat4 rotate_camera(glm::vec3 degree);
extern glm::mat4 trans_camera(glm::vec3 camera_trans);
extern glm::mat4 camera_locate(glm::vec3 cameraPos_value, glm::vec3 cameraTarget_value);
//=========================================================================================================================//
// ������ȯ
extern GLvoid Orthogoanl_Projection_Transformation(GLfloat value, GLuint shaderProgramID);
extern GLvoid Perspective_Projection_Transformation(PROJECTION projectionValue, glm::vec3 spaceTrans, GLuint shaderProgramID);
//=========================================================================================================================//

char* filetobuf(const char* file) {

	FILE* fptr;
	long length;
	char* buf;

	fptr = fopen(file, "rb");
	if (!fptr) return NULL;
	fseek(fptr, 0, SEEK_END);
	length = ftell(fptr);
	buf = (char*)malloc(length + 1);
	fseek(fptr, 0, SEEK_SET);
	fread(buf, length, 1, fptr);
	fclose(fptr);
	buf[length] = 0;

	return buf;
}

void make_VertexShaders(char* fileName) {
	GLchar* vertexSource;

	vertexSource = filetobuf(fileName);
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, NULL);
	glCompileShader(vertexShader);

	GLint result;
	GLchar errorLog[512];

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader ������ ����\n" << errorLog << std::endl;
		return;
	}
}

void make_FragmentShaders(char* fileName)
{
	GLchar* fragmentSource;

	fragmentSource = filetobuf(fileName);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, NULL);
	glCompileShader(fragmentShader);

	GLint result;
	GLchar errorLog[512];

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: frag_shader ������ ����\n" << errorLog << std::endl;
		return;
	}
}

GLuint make_ShaderProgram()
{
	GLuint shaderID;
	shaderID = glCreateProgram();

	glAttachShader(shaderID, vertexShader);
	glAttachShader(shaderID, fragmentShader);

	glLinkProgram(shaderID);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLint result;
	GLchar errorLog[512];

	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderID, 512, NULL, errorLog);
		std::cerr << "ERROR: shader program ���� ����\n" << errorLog << std::endl;
		return false;
	}
	glUseProgram(shaderID);
	return shaderID;
}

void InitBuffer(BUFFER* buffer, glm::vec3* vPositionList, GLuint index[][3], glm::vec3* color, GLuint vSize, GLuint iSize, GLuint cSize)
{
	glGenVertexArrays(1, &buffer->VAO);
	glGenBuffers(1, &buffer->VBO_pos);

	glBindVertexArray(buffer->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, buffer->VBO_pos);
	glBufferData(GL_ARRAY_BUFFER, vSize, vPositionList, GL_STATIC_DRAW);

	glGenBuffers(1, &buffer->EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, iSize, index, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &buffer->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, buffer->VBO);
	glBufferData(GL_ARRAY_BUFFER, cSize, color, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
}

GLfloat conversion_x(GLfloat x) { return (GLfloat)((x - (WIDTH / 2)) / (WIDTH / 2)); }

GLfloat conversion_y(GLfloat y) { return (GLfloat)((y - (HEIGHT / 2)) / (HEIGHT / -2)); }

// �ϳ��� ����Ѵٸ� �������
//=========================================================================================================================//
// �𵨸���ȯ

// �� ȸ��
glm::mat4 rotate_shape(glm::vec3 degree) {
	glm::mat4 rotateMatrix_x(1.0f);
	rotateMatrix_x = glm::rotate(rotateMatrix_x, glm::radians(degree.x), glm::vec3(1.0f, 0.0f, 0.0f)); //--- X�� ȸ��

	glm::mat4 rotateMatrix_y(1.0f);
	rotateMatrix_y = glm::rotate(rotateMatrix_y, glm::radians(degree.y), glm::vec3(0.0f, 1.0f, 0.0f)); //--- Y�� ȸ��

	glm::mat4 rotateMatrix_z(1.0f);
	rotateMatrix_z = glm::rotate(rotateMatrix_z, glm::radians(degree.z), glm::vec3(0.0f, 0.0f, 1.0f)); //--- Z�� ȸ��

	glm::mat4 rotateMatrix(1.0f);
	rotateMatrix = rotateMatrix_z * rotateMatrix_y * rotateMatrix_x;

	return rotateMatrix;
}

// �� �̵�
glm::mat4 translation_shape(glm::vec3 trans) {
	glm::mat4 transMatrix(1.0f);
	transMatrix = glm::translate(transMatrix, trans); //--- �̵�

	return transMatrix;
}

// �� ����
glm::mat4 scaling_shape(glm::vec3 scale) {
	glm::mat4 scaleMatrix(1.0f);
	scaleMatrix = glm::scale(scaleMatrix, scale);

	return scaleMatrix;
}

//=========================================================================================================================//
// ���׺�ȯ

// ī�޶� ȸ��
glm::mat4 rotate_camera(glm::vec3 degree) {
	glm::mat4 view_revolution_x = glm::mat4(1.0f);
	view_revolution_x = glm::rotate(view_revolution_x, glm::radians(degree.x), glm::vec3(1.0f, 0.0f, 0.0f));

	glm::mat4 view_revolution_y = glm::mat4(1.0f);
	view_revolution_y = glm::rotate(view_revolution_y, glm::radians(degree.y), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 view_revolution_z = glm::mat4(1.0f);
	view_revolution_z = glm::rotate(view_revolution_z, glm::radians(degree.z), glm::vec3(0.0f, 0.0f, 1.0f));

	glm::mat4 view_revolution = glm::mat4(1.0f);
	view_revolution = view_revolution_z * view_revolution_y * view_revolution_x;

	return view_revolution;
}

// ī�޶� �̵�
glm::mat4 trans_camera(glm::vec3 camera_trans) {
	glm::mat4 view_translate = glm::mat4(1.0f);
	view_translate = glm::translate(view_translate, camera_trans);

	return view_translate;
}

// ī�޶� ��ġ
glm::mat4 camera_locate(glm::vec3 cameraPos_value, glm::vec3 cameraTarget_value) {
	// ī�޶� ��ġ
	glm::vec3 cameraPos = cameraPos_value;

	// ī�޶� ���� : ī�޶� ���� ���� (n)
	glm::vec3 cameraTarget = cameraTarget_value;
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

	// ī�޶��� ������ �� (u)
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

	// ī�޶��� ���� �� (v)
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

	// ī�޶� ���� ��
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);

	return view;
}

//=========================================================================================================================//
// ������ȯ

// ��������
GLvoid Orthogoanl_Projection_Transformation(GLfloat value, GLuint shaderProgramID) {
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::ortho(-value, value, -value, value, -value, value);

	unsigned int projectionLocation = glGetUniformLocation(shaderProgramID, "projection"); //--- ���� ��ȯ �� ����
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
}
// ��������
GLvoid Perspective_Projection_Transformation(PROJECTION projectionValue, glm::vec3 spaceTrans, GLuint shaderProgramID) {
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(projectionValue.Fovy), projectionValue.Aspect, projectionValue.Near, projectionValue.Far); //--- ���� ���� ����: fovy, aspect, near, far 
	projection = glm::translate(projection, spaceTrans); //--- ������ z�� �̵� 

	unsigned int projectionLocation = glGetUniformLocation(shaderProgramID, "projection"); //--- ���� ��ȯ �� ����
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
}
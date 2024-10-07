#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include "GL_My_header.h"

GLuint vertexShader;
GLuint fragmentShader;

extern char* filetobuf(const char* file);
extern void make_VertexShaders(char* fileName);
extern void make_FragmentShaders(char* fileName);
extern GLuint make_ShaderProgram();

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

	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	
	GLint result;
	GLchar errorLog[512];
	
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if(!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

void make_FragmentShaders(char* fileName)
{
	GLchar* fragmentSource;
	
	fragmentSource = filetobuf(fileName);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	GLint result;
	GLchar errorLog[512];

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: frag_shader 컴파일 실패\n" << errorLog << std::endl;
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
		std::cerr << "ERROR: shader program 연결 실패\n" << errorLog << std::endl; 
		return false;
	}
	glUseProgram(shaderID); 
	return shaderID;
}
#version 330 core

//--- in_Position: attribute index 0 
//--- in_Color: attribute index 1

layout (location = 0) in vec3 in_Position; //--- ��ġ ����: attribute position 0
layout (location = 1) in vec3 in_Color; //--- �÷� ����: attribute position 1
out vec3 out_Color; //--- �����׸�Ʈ ���̴����� ����

uniform mat4 model = mat4(1.0f); // �𵨸���ȯ
uniform mat4 view = mat4(1.0f); // ���׺�ȯ
uniform mat4 projection = mat4(1.0f); // ������ȯ
uniform vec3 color;

void main(void) 
{
	gl_Position = projection * view * model * vec4 (in_Position, 1.0f); 
	
	out_Color = in_Color;
	out_Color = color;
}
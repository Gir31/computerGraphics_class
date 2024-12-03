#version 330 core

//--- in_Position: attribute index 0 
//--- in_Color: attribute index 1

layout (location = 0) in vec3 vPos; //--- ��ġ ����: attribute position 0
layout (location = 1) in vec3 vNormal; //--- �븻 ����: attribute position 1
layout (location = 2) in vec3 vColor; //--- �븻 ����: attribute position 1

out vec3 FragPos;
out vec3 Normal;
out vec3 out_Color;

uniform mat4 model = mat4(1.0f); // �𵨸���ȯ
uniform mat4 view = mat4(1.0f); // ���׺�ȯ
uniform mat4 projection = mat4(1.0f); // ������ȯ

void main(void) 
{
	gl_Position = projection * view * model * vec4(vPos, 1.0);
	FragPos = vec3(model * vec4(vPos, 1.0));
	Normal = vNormal;
	out_Color = vColor;
}

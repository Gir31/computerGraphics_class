#version 330 core

//--- in_Position: attribute index 0 
//--- in_Color: attribute index 1

layout (location = 0) in vec3 in_Position; //--- 위치 변수: attribute position 0
layout (location = 1) in vec3 in_Color; //--- 컬러 변수: attribute position 1
out vec3 out_Color; //--- 프래그먼트 세이더에게 전달

uniform mat4 model = mat4(1.0f); // 모델링변환
uniform mat4 view = mat4(1.0f); // 뷰잉변환
uniform mat4 projection = mat4(1.0f); // 투영변환
uniform vec3 color;

void main(void) 
{
	gl_Position = projection * view * model * vec4 (in_Position, 1.0f); 
	
	out_Color = in_Color;
	out_Color = color;
}

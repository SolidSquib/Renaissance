#version 330 core

layout(location=0) in vec3 a_Pos;
layout(location=1) in vec4 a_Col;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec4 v_Col;

void main()
{
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Pos, 1.0);
	v_Col = a_Col;
}
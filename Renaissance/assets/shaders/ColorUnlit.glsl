#type vertex
#version 330 core

layout(location=0) in vec4 a_Pos;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main()
{
	gl_Position = u_ViewProjection * u_Transform * a_Pos;
}

#type fragment
#version 330 core

uniform vec4 u_Col;

out vec4 f_Col;

void main()
{
	f_Col = u_Col;
}
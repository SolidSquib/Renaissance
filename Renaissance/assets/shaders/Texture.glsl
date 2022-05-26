#type vertex
#version 420 core

layout(location=0) in vec3 a_Pos;
layout(location=1) in vec4 a_Tint;
layout(location=2) in vec2 a_TexCoord;
layout(location=3) in float a_TexIndex;

uniform mat4 u_ViewProjection;

out vec4 v_Tint;
out vec2 v_TexCoord;
flat out float v_TexIndex;

void main()
{
	gl_Position = u_ViewProjection * vec4(a_Pos, 1.0);
	v_Tint = a_Tint;
	v_TexCoord = a_TexCoord;
	v_TexIndex = a_TexIndex;
}

#type fragment
#version 420 core

in vec4 v_Tint;
in vec2 v_TexCoord;
flat in float v_TexIndex;

layout (binding=0) uniform sampler2D u_Textures[16];

out vec4 f_Col;

void main()
{
	f_Col = texture(u_Textures[int(v_TexIndex)], v_TexCoord) * v_Tint;
}
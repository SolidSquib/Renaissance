#type vertex
#version 450 core

layout(location=0) in vec3 a_Pos;
layout(location=1) in vec4 a_Tint;
layout(location=2) in vec2 a_TexCoord;
layout(location=3) in uint a_TexIndex;
layout(location=4) in uint a_entityId;

uniform mat4 u_ViewProjection;

out vec4 v_Tint;
out vec2 v_TexCoord;
flat out uint v_TexIndex;
flat out uint v_entityId;

void main()
{
	gl_Position = u_ViewProjection * vec4(a_Pos, 1.0);
	v_Tint = a_Tint;
	v_TexCoord = a_TexCoord;
	v_TexIndex = a_TexIndex;
	v_entityId = a_entityId;
}

#type fragment
#version 450 core

in vec4 v_Tint;
in vec2 v_TexCoord;
flat in uint v_TexIndex;
flat in uint v_entityId;

layout (binding = 0) uniform sampler2D u_Textures[16];

layout (location = 0) out vec4 f_Col;
layout (location = 1) out uint f_EntityId;

void main()
{
	f_Col = texture(u_Textures[v_TexIndex], v_TexCoord) * v_Tint;
	f_EntityId = v_entityId;
}
#version 460 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

out gl_PerVertex
{
	vec4 gl_Position;
};

out vec3 color;
out vec3 position;

void main()
{
	position = inPosition;
	gl_Position = vec4(inPosition, 1.0f);
	color = inColor;
}

#version 460 core

out gl_PerVertex
{
    vec4 gl_Position;
};

out vec3 color;

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;

uniform mat4 model;
uniform mat4 proj;

void main()
{
    color = inColor;
    gl_Position = vec4(inPosition, 0.0f, 1.0f) * model * proj;
}


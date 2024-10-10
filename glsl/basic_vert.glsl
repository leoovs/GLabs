#version 460 core

out gl_PerVertex
{
    vec4 gl_Position;
};

out vec2 position;

layout (location = 0) in vec2 inPosition;

void main()
{
    gl_Position = vec4(position = inPosition, 0.0f, 1.0f);
}


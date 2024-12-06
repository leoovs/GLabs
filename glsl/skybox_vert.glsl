#version 460 core

layout (location = 0) in vec3 inPosition;

uniform mat4 uViewProjection;

out gl_PerVertex
{
    vec4 gl_Position;
};

out vec3 uv;

void main()
{
    uv = inPosition;
    gl_Position = uViewProjection * vec4(inPosition, 1.0f);
}


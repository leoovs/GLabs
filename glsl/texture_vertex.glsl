#version 460 core

out gl_PerVertex
{
    vec4 gl_Position;
};
out vec2 texCoord;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

void main()
{
    texCoord = inTexCoord;
    gl_Position = vec4(inPosition, 1.0f);
}


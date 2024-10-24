#version 460 core

layout(location = 0) in vec3 inPosition;

uniform mat4 uModel;
uniform mat4 uProj;

out gl_PerVertex
{
    vec4 gl_Position;
};

void main()
{
    gl_Position = uProj * uModel * vec4(inPosition, 1.0f);
}


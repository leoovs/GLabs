#version 460 core

layout (location = 0) in vec3 inPosition; 
layout (location = 0) in vec3 inNormal; 
layout (location = 0) in vec2 inUV; 

uniform mat4 uMVP;

out gl_PerVertex
{
    vec4 gl_Position;
};

out vec3 normal;
out vec2 uv;

void main()
{
    gl_Position = uMVP * vec4(inPosition, 1.0f);
    normal = (uMVP * vec4(inNormal, 1.0f)).xyz;
    uv = inUV;
}


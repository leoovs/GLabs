#version 460 core

in vec3 position;
in vec3 normal;
in vec2 uv;

layout (location = 0) out vec3 outPosition;
layout (location = 1) out vec3 outNormal;
layout (location = 2) out vec2 outUV;

void main()
{
    outPosition = position;
    outNormal = normalize(normal);
    outUV = uv; 
}


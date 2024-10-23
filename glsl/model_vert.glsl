#version 460 core

layout (location = 0) in vec3 inPosition; 
layout (location = 0) in vec3 inNormal; 
layout (location = 0) in vec2 inUV; 

uniform mat4 uModelMtx;
uniform mat4 uNormalMtx;
uniform mat4 uViewProjectionMtx;

out gl_PerVertex
{
    vec4 gl_Position;
};

out vec3 worldPos;
out vec3 normal;
out vec2 uv;

void main()
{
    vec4 pos = uModelMtx * vec4(inPosition, 1.0f); 
    vec4 nor = uNormalMtx * vec4(inNormal, 1.0f);

    worldPos = pos.xyz;
    normal = nor.xyz;
    uv = inUV;

    gl_Position = uViewProjectionMtx * pos;
}


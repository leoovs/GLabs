#version 460 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;

uniform mat4 uViewProj;
uniform mat4 uModel;
uniform mat4 uITModel;

out gl_PerVertex
{
    vec4 gl_Position;
};

layout (location = 0) out vec3 oWorldPosition;
layout (location = 1) out vec3 oNormal;
layout (location = 2) out vec2 oUV;

void main()
{
    vec4 worldPosition4 = uModel * vec4(aPosition, 1.0f);
    oWorldPosition = worldPosition4.xyz;
    gl_Position = uViewProj * worldPosition4;

    vec4 normal4 = uITModel * vec4(aNormal, 1.0f);
    oNormal = normal4.xyz;

    oUV = aUV;
}


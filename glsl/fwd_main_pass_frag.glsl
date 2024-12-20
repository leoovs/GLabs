#version 460 core

layout (location = 0) in vec3 iPosition;
layout (location = 1) in vec3 iUnnormalizedNormal;
layout (location = 2) in vec2 iUV;

layout (binding = 0) uniform sampler2DArray uAlbedoMap;
layout (binding = 1) uniform sampler2DArray uNormalMap;
layout (binding = 2) uniform sampler2DArray uSpecularMap;

out vec4 oColor;

vec4 sampleMaterial(sampler2DArray map)
{
    return texture(map, vec3(iUV, 0.0f));
}

void main()
{
    oColor = vec4(sampleMaterial(uSpecularMap).w);
}


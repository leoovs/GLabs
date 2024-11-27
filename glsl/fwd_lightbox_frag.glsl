#version 460 core

#define POINT_LIGHT_N 5

struct PointLight
{
    vec3 diffuse[POINT_LIGHT_N];
    int casts[POINT_LIGHT_N];
};

uniform PointLight uPointLight;

flat in int vertexIndex;

out vec4 outColor;

void main()
{
    int lightIndex = vertexIndex / 36;  
    outColor = vec4(uPointLight.diffuse[lightIndex], 1.0f) * uPointLight.casts[lightIndex];
}


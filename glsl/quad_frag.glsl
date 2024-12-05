#version 460 core

in vec3 uv;

layout (binding = 0) uniform samplerCube uCubemap;

out vec4 outColor;

void main()
{
    outColor = sampleCubeArray(uCubemap, uv);
    // outColor = vec4(uv, 1.0f);
}


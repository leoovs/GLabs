#version 460 core

in vec3 uv;

layout (binding = 0) uniform samplerCubeArray uCubemap;

out vec4 outColor;

void main()
{
    outColor = texture(uCubemap, vec4(uv, 0.0f));
}


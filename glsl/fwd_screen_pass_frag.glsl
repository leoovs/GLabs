#version 460 core

layout (location = 0) in vec2 iUV;

layout (binding = 0) uniform sampler2DArray uScreen;

out vec4 oScreen;

void main()
{
    oScreen = texture(uScreen, vec3(iUV, 0.0f));
}


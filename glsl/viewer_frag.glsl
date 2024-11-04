#version 460 core

in vec2 texCoord;
in float light;

layout (binding = 0) uniform sampler2D uColor;

out vec4 outColor;

void main()
{
    // outColor = texture(uColor, texCoord);
    outColor = vec4(vec3(0.1f, 0.0f, 0.0f) + vec3(light), 1.0f);
}


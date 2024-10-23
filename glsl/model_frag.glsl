#version 460 core

in vec3 worldPos;
in vec3 normal;
in vec2 uv;

out vec4 outColor;

layout (binding = 0) uniform sampler2D albedo;

void main()
{
    outColor = texture(albedo, uv);
    outColor = vec4(normal, 1.0f);
}


#version 460 core

in float directionalLight;

out vec4 outColor;

void main()
{
    vec3 red = vec3(1.0f, 0.0f, 0.0f);
    vec3 color = red * directionalLight;

    outColor = vec4(color, 1.0f);
}


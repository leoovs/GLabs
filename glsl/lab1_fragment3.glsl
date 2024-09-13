#version 460 core

out vec4 outColor;

float noise1()
{
    vec2 n = gl_FragCoord.xy;
    return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}

void main()
{
    outColor = vec4(noise1() * vec3(0.0f, 0.0f, 1.0f), 1.0f);
}


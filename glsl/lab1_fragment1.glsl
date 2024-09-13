#version 460 core

out vec4 outColor;

float circles()
{
    const vec2 coords =  0.25f * gl_FragCoord.xy;
    return (sin(coords.x) + sin(coords.y)) / 2.0f; 
}

void main()
{
    outColor = vec4(circles() * vec3(1.0f, 1.0f, 0.0f), 1.0f);
}


#version 460 core

uniform vec3 uColor;
uniform int uImitateDepthBuffer;

out vec4 outColor;

void main()
{
    if (uImitateDepthBuffer != 0)
    {
	outColor = vec4(vec3(gl_FragCoord.z), 1.0f);
	return;
    }
    outColor = vec4(uColor, 1.0f);
}


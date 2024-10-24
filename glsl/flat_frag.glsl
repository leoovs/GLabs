#version 460 core

uniform vec3 uColor;
uniform int uImitateDepthBuffer;

out vec4 outColor;

float imitateDepthValue(float depth)
{
    const float cNear = 0.01f;
    const float cFar = 1.0f;

    float z = depth * 2.0f - 1.0f; 
    return (2.0f * cNear * cFar) / (cFar + cNear - z * (cFar - cNear)) / cFar; 
}

void main()
{
    if (uImitateDepthBuffer != 0)
    {
	outColor = vec4(vec3(imitateDepthValue(gl_FragCoord.z)), 1.0f);
	return;
    }
    outColor = vec4(uColor, 1.0f);
}


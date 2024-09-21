#version 460 core

uniform int enableInterpolation; 

in vec4 color;
in flat vec4 flatColor;

out vec4 outColor;

void main()
{
    vec4 result;

    if (0 == enableInterpolation)
    {
	result = flatColor;
    }
    else
    {
	result = color;
    }

    result.a = 1.0f;

    outColor = result;
    // outColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);
}


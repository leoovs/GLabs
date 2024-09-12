#version 460 core

uniform vec3 tone;

in vec3 color;
in vec3 position;
out vec4 outColor;

float noise(vec2 co)
{
	return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{
	vec3 clampedColor = color;

	if (clampedColor.r >= 0.5f)
	{
		clampedColor = vec3(1.0f, 0.0f, 0.0f);
	}
	else if (clampedColor.g >= 0.5f)
	{
		clampedColor = vec3(0.0f, 1.0f, 0.0f);
	}
	else if (clampedColor.b >= 0.5f)
	{
		clampedColor = vec3(0.0f, 0.0f, 1.0f);
	}
	else
	{
		float noiseResult = noise(position.xy);
		clampedColor = vec3(noiseResult, noiseResult, noiseResult) * tone;
	}

	outColor = vec4(clampedColor, 1.0f);
}

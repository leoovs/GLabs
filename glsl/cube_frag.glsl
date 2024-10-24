#version 460 core

out vec4 outColor;

uniform int uDrawWireframe;

void main()
{
    if (uDrawWireframe != 0)
    {
	outColor = vec4(1.0f);
	return;
    }

    vec3[6] faceColors = vec3[6]
    (
	vec3(0.0f, 1.0f, 0.0f),
	vec3(1.0f, 0.0f, 0.0f),
	vec3(0.0f, 0.0f, 1.0f),
	vec3(1.0f, 0.5f, 0.0f),
	vec3(1.0f, 1.0f, 0.0f),
	vec3(1.0f, 1.0f, 1.0f)
    );

    vec3 color = faceColors[gl_PrimitiveID / 2];
    outColor = vec4(color, 1.0f);
}


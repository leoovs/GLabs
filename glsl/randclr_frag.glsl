#version 460 core

out vec4 outColor;

float noise(vec2 co)
{
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

float clamped_noise(vec2 co)
{
    return clamp(noise(co), 0.2f, 1.0f);
}

void main()
{
    const float Pi = radians(180.0f);
    float primitive = float(gl_PrimitiveID);
    primitive *= primitive;

    vec2 offsets[3];
    offsets[0] = vec2(0.0f, primitive);
    offsets[1] = vec2(primitive, 0.0f);
    offsets[2] = vec2(primitive, 0.0f);

    outColor = vec4(
	clamped_noise(offsets[0]),
	clamped_noise(offsets[1]),
	clamped_noise(offsets[2]),
	1.0f
    );
}


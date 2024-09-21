#version 460 core

layout(location = 0) in vec2 inPosition;

out gl_PerVertex
{
    vec4 gl_Position;
};

out vec4 color;
out vec4 flatColor;

float noise(vec2 co)
{
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{
    vec4 clr = vec4(
	noise(inPosition + vec2(0.0f, -0.2f)),
	noise(inPosition + vec2(0.2f, 0.0f)),
	noise(inPosition + vec2(0.0f, 0.2f)),
	noise(inPosition + vec2(-0.2f, 0.0f))
    );

    clr *= noise(inPosition);
    
    color = clr;
    flatColor = clr;
    gl_Position = vec4(inPosition, 0.0f, 1.0f);
}


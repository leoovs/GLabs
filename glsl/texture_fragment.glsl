#version 460 core

out vec4 outColor;

layout(location = 0) uniform sampler2D inTexture;
in vec2 texCoord;

void main()
{
    outColor = texture(inTexture, texCoord);    
}


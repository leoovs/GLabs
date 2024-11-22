#version 460 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;

uniform mat4 uM;
uniform mat4 uVP;
uniform mat4 uITM;

out gl_PerVertex
{
    vec4 gl_Position;
};

out vec3 position;
out vec3 normal;
out vec2 texCoord;

void main()
{
    vec4 worldPosition = uM * vec4(inPosition, 1.0f);
    gl_Position = uVP * worldPosition;

    position = vec3(worldPosition);

    normal = normalize(vec3(uITM * vec4(inNormal, 1.0f)));

    texCoord = inTexCoord;
}


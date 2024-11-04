#version 460 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;

uniform mat4 uInvTranspM;
uniform mat4 uMVP;
uniform vec3 uCameraFront;

out gl_PerVertex
{
    vec4 gl_Position;
};

out float directionalLight;

void main()
{
    gl_Position = uMVP * vec4(inPosition, 1.0f);

    vec3 normal = normalize((uInvTranspM * vec4(inNormal, 1.0f)).xyz);
    directionalLight = max(dot(normal, -normalize(uCameraFront)), 0.0f);
}


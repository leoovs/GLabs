#version 460 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;

uniform mat4 uInvTranspModel;
uniform mat4 uViewProjection;

out gl_PerVertex
{
    vec4 gl_Position;
};

out vec2 texCoord;
out float light;

void main()
{
    vec3 position = (uInvTranspModel * vec4(inPosition, 1.0f)).xyz;
    vec3 norm = normalize((uInvTranspModel * vec4(inNormal, 1.0f)).xyz);

    gl_Position = uViewProjection * vec4(position, 1.0f);
    texCoord = inTexCoord;

    const vec3 cLightPos = vec3(0.0f, 0.0f, 0.0f);
    
    vec3 ray = normalize(cLightPos - position);
    light = max(dot(norm, ray), 0.0f);
}


#version 460 core 

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;

uniform mat4 uViewProjection;
uniform mat4 uModel;
uniform mat4 uInversedTransposedModel;

out vec3 position;
out vec3 normal;
out vec2 uv;

out gl_PerVertex
{
    vec4 gl_Position;
};

void main()
{
    vec4 worldPosition = uModel * vec4(inPosition, 1.0f);
    vec4 clipPosition = uViewProjection * worldPosition;
    vec4 worldNormal = uInversedTransposedModel * vec4(inNormal, 1.0f);

    position = worldPosition.xyz;
    gl_Position = clipPosition;
    normal = worldNormal.xyz;
    uv = inUV;
}


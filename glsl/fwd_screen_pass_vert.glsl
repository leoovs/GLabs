#version 460 core

const vec2 cQuadPositions[] = vec2[4](
    vec2(-1.0f, -1.0f),
    vec2(-1.0f,  1.0f),
    vec2( 1.0f,  1.0f),
    vec2( 1.0f, -1.0f)
);

const vec2 cQuadUV[] = vec2[4](
    vec2(0.0f, 0.0f),
    vec2(0.0f, 1.0f),
    vec2(1.0f, 1.0f),
    vec2(1.0f, 0.0f)
);

const uint cQuadIndices[] = uint[6](
    0, 1, 2,
    2, 3, 0
);

out gl_PerVertex
{
    vec4 gl_Position;
};

layout (location = 0) out vec2 oUV;

void main()
{
    uint quadVertexIndex = cQuadIndices[gl_VertexID];

    gl_Position = vec4(cQuadPositions[quadVertexIndex], 0.0f, 1.0f);
    oUV = cQuadUV[quadVertexIndex];
}


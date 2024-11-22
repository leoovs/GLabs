#version 460 core

#define POINT_LIGHT_N 8

uniform mat4 uVP;
uniform vec3 uLightPositions[POINT_LIGHT_N];

const vec3 cubeVertices[8] = vec3[8](
    vec3(-1.0, -1.0,  1.0), 
    vec3( 1.0, -1.0,  1.0), 
    vec3(1.0,  1.0,  1.0), 
    vec3(-1.0,  1.0,  1.0), 

    vec3(-1.0, -1.0, -1.0), 
    vec3(1.0, -1.0, -1.0), 
    vec3(1.0,  1.0, -1.0), 
    vec3(-1.0,  1.0, -1.0)
);

const int baseIndices[36] = int[36](
    // top
    0, 1, 2,
    2, 3, 0,
    // right
    1, 5, 6,
    6, 2, 1,
    // back
    7, 6, 5,
    5, 4, 7,
    // left
    4, 0, 3,
    3, 7, 4,
    // bottom
    4, 5, 1,
    1, 0, 4,
    // top
    3, 2, 6,
    6, 7, 3
);
int cubeIndices[36*POINT_LIGHT_N];

out gl_PerVertex
{
    vec4 gl_Position;
};

out int vertexIndex;

void main()
{
    for (int iLight = 0; iLight < POINT_LIGHT_N; iLight++)
    {
	for (int iBase = 0; iBase < 36; iBase++)
	{
	    cubeIndices[iLight * 36 + iBase] = baseIndices[iBase];
	}
    }

    vertexIndex = gl_VertexID;
    
    int lightIndex = gl_VertexID / 36;

    vec3 vertex = cubeVertices[cubeIndices[gl_VertexID]];
    vertex /= 16.0f;

    vec3 worldPosition = vertex + uLightPositions[lightIndex];

    gl_Position = uVP * vec4(worldPosition, 1.0f);
}


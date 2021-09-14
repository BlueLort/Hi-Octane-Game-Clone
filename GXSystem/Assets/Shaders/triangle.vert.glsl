#version 450

const vec3 trianglePositions[3] = vec3[3](
    vec3(1.0, 1.0, 0.0),
    vec3(-1.0, 1.0, 0.0),
    vec3(0.0, -1.0, 0.0)
);

void main()
{
    gl_Position = vec4(trianglePositions[gl_VertexIndex], 1.0);
}
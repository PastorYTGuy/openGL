#version 420

layout (location = 0) in vec3 VertexPosition;

out vec3 TexCoords;

uniform mat4 ProjMat;
uniform mat4 ViewMat;

void main()
{
    TexCoords = VertexPosition;
    vec4 pos = ProjMat * ViewMat * vec4(VertexPosition, 1.0);
    gl_Position = pos.xyww;
}
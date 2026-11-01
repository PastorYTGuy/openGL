#version 420

uniform mat4 ProjMat;
uniform mat4 ViewMat;
uniform mat4 ModelMat;
uniform sampler2D heightMap;
uniform float heightScale;

layout (location=0) in vec3 VertexPosition;
layout (location=1) in vec3 VertexTexCoord;
layout (location=2) in vec3 VertexNormal;
layout (location=3) in vec3 VertexTangent;

out vec3 FragPos;
out vec2 TexCoord;
out vec3 MeshNormal;
out vec3 MeshTangent;
out vec3 MeshBitangent;

void main()
{
    TexCoord = VertexTexCoord.xy;
    MeshNormal = VertexNormal;
    MeshTangent = VertexTangent;
    MeshBitangent = cross(VertexNormal, VertexTangent);

    vec3 displacedPosition = VertexPosition;
    if (heightScale > 0.0) {
        float height = texture(heightMap, TexCoord).r;
        displacedPosition.y += height * heightScale;
    }

    FragPos = vec3(ModelMat * vec4(displacedPosition, 1.0));
    gl_Position = ProjMat * ViewMat * vec4(FragPos, 1.0);
}
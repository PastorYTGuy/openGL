#version 420

in vec3 FragPos;
in vec2 TexCoord;
in vec3 MeshNormal;
in vec3 MeshTangent;
in vec3 MeshBitangent;

out vec4 FragColor;

uniform vec3 LightPos;
uniform vec3 ViewPos;
uniform vec3 LightColor;
uniform vec3 ObjectColor;
uniform mat4 ViewMat;
uniform mat4 ModelMat;
uniform float heightScale;

uniform sampler2D colorMap;
uniform sampler2D normalMap;
uniform sampler2D grassMap;
uniform sampler2D treeMap;
uniform int useNormalMap;

uniform vec3 fogColor;
uniform float fogStart;
uniform float fogEnd;

void main()
{
    vec3 w_norm;
    vec2 finalUV = TexCoord;
    float alpha = 1.0;

    if (useNormalMap == 1 && abs(MeshNormal.y) > 0.9) {
        finalUV = FragPos.xz * 3.0;
    }

    if (heightScale > 0.0) {
        vec3 w_tangentX = dFdx(FragPos);
        vec3 w_tangentY = dFdy(FragPos);
        w_norm = normalize(cross(w_tangentX, w_tangentY));
        if (w_norm.y < 0.0) w_norm = -w_norm;
    }
    else if (useNormalMap == 1) {
        vec3 T = normalize(vec3(ModelMat * vec4(MeshTangent, 0.0)));
        vec3 B = normalize(vec3(ModelMat * vec4(MeshBitangent, 0.0)));
        vec3 N = normalize(vec3(ModelMat * vec4(MeshNormal, 0.0)));
        mat3 TBN = mat3(T, B, N);

        vec3 localNormal = texture(normalMap, finalUV).rgb;
        localNormal = normalize(localNormal * 2.0 - 1.0);
        w_norm = normalize(TBN * localNormal);
    }
    else {
        w_norm = normalize(vec3(ModelMat * vec4(MeshNormal, 0.0)));
    }

    vec3 baseColor = ObjectColor;
    if (useNormalMap == 1) {
        baseColor = texture(colorMap, finalUV).rgb;
    }
    else if (useNormalMap == 2) {
        baseColor = texture(grassMap, TexCoord * 8.0).rgb;
    }
    else if (useNormalMap == 3) {
        vec4 texColor = texture(treeMap, TexCoord);
        if (texColor.a < 0.1) discard;
        baseColor = texColor.rgb;
        alpha = texColor.a;
    }

    vec3 strongLightColor = LightColor * 3.0;

    float ambientStrength = (useNormalMap == 2) ? 0.15 : 0.01;
    vec3 ambient = ambientStrength * strongLightColor;

    vec3 lightDir = normalize(LightPos - FragPos);
    float dotNL = max(dot(w_norm, lightDir), 0.0);
    float diff = pow(dotNL, 2.0);
    vec3 diffuse = diff * strongLightColor;

    vec3 viewDir = normalize(ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, w_norm);

    float specularStrength = 2.0;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
    vec3 specular = (dotNL > 0.0 && useNormalMap != 2) ? (specularStrength * spec * strongLightColor) : vec3(0.0);

    vec3 finalObjColor = (ambient + diffuse + specular) * baseColor;

    if (length(MeshNormal) < 0.1 && heightScale == 0.0) {
        FragColor = vec4(ObjectColor, 1.0);
    } else {
        float dist = length(ViewPos - FragPos);
        float fogFactor = clamp((fogEnd - dist) / (fogEnd - fogStart), 0.0, 1.0);
        vec3 colorWithFog = mix(fogColor, finalObjColor, fogFactor);
        FragColor = vec4(colorWithFog, alpha);
    }
}
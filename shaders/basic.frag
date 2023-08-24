#version 330 core

in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

const int lightMaxCount = 100;

struct Light
{
    vec4 lightPos;
    vec4 lightDir;
};

layout (std140) uniform LightInfo
{
    Light lights[lightMaxCount];
};

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D maskMap;

const float shadowRange = 0.5;
//const vec3 diffuseColor = vec3(1.0, 1.0, 1.0);
const vec3 shadowColor = vec3(0.1, 0.1, 0.1);

out vec4 FragColor;

void main()
{   
    vec3 lightDir = normalize(lights[0].lightPos.xyz - WorldPos);
    float lambert = max(dot(normalize(Normal), lightDir), 0.0) * 0.5 + 0.5;
    vec3 diffuseColor = texture(diffuseMap, vec2(TexCoords.x, TexCoords.y)).rgb;
    vec3 nromalColor = texture(normalMap, vec2(TexCoords.x, TexCoords.y)).rgb;
    vec3 maskColor = texture(maskMap, vec2(TexCoords.x, TexCoords.y)).rgb;
    vec3 diffuse = lambert > shadowRange ? diffuseColor : shadowColor;
    FragColor = vec4(lambert * diffuseColor, 1.0);
}
#version 330 core

in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;
in mat4 Model;

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
uniform sampler2D lightMap;

uniform vec3 camPos;

const float shadowRange = 0.5;
//const vec3 diffuseColor = vec3(1.0, 1.0, 1.0);
const vec3 shadowColor = vec3(0.1, 0.1, 0.1);

out vec4 FragColor;

void main()
{   
    vec3 lightDir = lights[0].lightDir.xyz;

    vec3 viewDir = normalize(camPos - WorldPos);

    /* TEXTURE CREATION */
    vec4 diffuseTex = texture(diffuseMap, vec2(TexCoords.x, TexCoords.y));
    vec4 nromalTex = texture(normalMap, vec2(TexCoords.x, TexCoords.y));
    vec4 lightTex = texture(lightMap, vec2(TexCoords.x, TexCoords.y));
    /* END OF TEXTURE CREATION */

    float NdotL = dot(Normal, normalize(lightDir));
    NdotL = NdotL * 0.5 + 0.5;

    float NdotH = dot(Normal, normalize(viewDir + lightDir));

    FragColor = vec4(diffuseTex.rgb, 1.0);
}
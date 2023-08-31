#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in ivec4 aBoneIds; 
layout (location = 6) in vec4 aWeights;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;
out mat4 Model;

const int MAX_BONES = 200;
const int MAX_BONE_INFLUENCE = 4;
//uniform mat4 finalBonesMatrices[MAX_BONES];

layout (std140) uniform PerBatchConstants
{
    mat4 model;
    mat4 finalBonesMatrices[MAX_BONES];
    bool skinMesh;
};

//uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 totalPosition = vec4(aPos, 1.0);
    if (skinMesh)
    {
        if (aWeights[0] > 0.0 || aWeights[1] > 0.0 || aWeights[2] > 0.0 || aWeights[3] > 0.0)
        {
            mat4 boneTransform = finalBonesMatrices[aBoneIds[0]] * aWeights[0];
            boneTransform += finalBonesMatrices[aBoneIds[1]] * aWeights[1];
            boneTransform += finalBonesMatrices[aBoneIds[2]] * aWeights[2];
            boneTransform += finalBonesMatrices[aBoneIds[3]] * aWeights[3];
            totalPosition = boneTransform * totalPosition;
        }
    }

    Model = model;
    TexCoords = aTexCoords;    
    WorldPos = vec3(model * totalPosition);
    Normal = mat3(transpose(inverse(model))) * aNormal;
 
    gl_Position = projection * view * vec4(WorldPos, 1.0);
}
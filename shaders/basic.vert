#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;
out mat4 Model;

layout (std140) uniform PerBatchConstants
{
    mat4 model;
};

//uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    Model = model;
    TexCoords = aTexCoords;    
    WorldPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
 
    gl_Position = projection * view * vec4(WorldPos, 1.0);
}
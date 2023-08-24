#include <string>
#include <vector>
#include <iostream>

#include "manager/OGL_AssetLoader.h"
#include "OGL_OpenGLPipelineStateCommonBaseManager.h"

using namespace OGL;

#define SHADER_ROOT "shaders/"

static void OutputShaderErrorMessage(unsigned int shaderId, const char* shaderFilename) 
{
    int logSize;
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logSize);

    logSize++;

    char* infoLog = new char[logSize];
    if (!infoLog) 
    {
        return;
    }

    glGetShaderInfoLog(shaderId, logSize, NULL, infoLog);


    for (int i = 0; i < logSize; i++) 
    {
        std::cerr << infoLog[i];
    }

    std::cerr << "Error compiling shader." << shaderFilename << std::endl;

    return;
}

static void OutputLinkerErrorMessage(unsigned int programId) 
{
    int logSize;
    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logSize);

    logSize++;

    char* infoLog = new char[logSize];
    glGetProgramInfoLog(programId, logSize, nullptr, infoLog);


    for (int i = 0; i < logSize; i++) 
    {
        std::cerr << infoLog[i];
    }

    std::cerr << "Error compiling linker." << std::endl;
}

static bool LoadShaderFromFile(const char* filename, const GLenum shaderType, GLuint& shader) 
{
    std::string cbufferShaderBuffer;
    std::string commonShaderBuffer;
    std::string shaderBuffer;
    int status;

    OGL_AssetLoader assetLoader;
    shaderBuffer = assetLoader.SyncOpenAndReadTextFileToString(filename);
    if (shaderBuffer.empty()) 
    {
        return false;
    }

    shaderBuffer = cbufferShaderBuffer + commonShaderBuffer + shaderBuffer;

    shader = glCreateShader(shaderType);

    const char* str = shaderBuffer.c_str();
    glShaderSource(shader, 1, &str, nullptr);

    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status != 1) 
    {
        OutputShaderErrorMessage(shader, filename);
        return false;
    }

    return true;
}

static bool LoadShaderProgram(const std::vector<std::pair<GLenum, std::string>>& source, GLuint& shaderProgram)
{
    int status;

    shaderProgram = glCreateProgram();

    for (auto it = source.cbegin(); it != source.cend(); it++) 
    {
        if (!it->second.empty())
        {
            GLuint shader;
            status = LoadShaderFromFile((SHADER_ROOT + it->second).c_str(), it->first, shader);
            if (!status) 
            {
                return false;
            }

            glAttachShader(shaderProgram, shader);
            glDeleteShader(shader);
        }
    }

    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
    if (status != 1) 
    {
        OutputLinkerErrorMessage(shaderProgram);
        return false;
    }

    return true;
}

bool OGL_OpenGLPipelineStateCommonBaseManager::InitializePipelineState(OGL_PipelineState** pipelineState)
{
    bool result;
    OGL_OpenGLPipelineState* pipeState = new OGL_OpenGLPipelineState(**pipelineState);

    std::vector<std::pair<GLenum, std::string>> list;

    if (!(*pipelineState)->vertexShaderName.empty()) 
    {
        list.emplace_back(GL_VERTEX_SHADER, (*pipelineState)->vertexShaderName);
    }

    if (!(*pipelineState)->fragmentShaderName.empty())
    {
        list.emplace_back(GL_FRAGMENT_SHADER, (*pipelineState)->fragmentShaderName);
    }

    if (!(*pipelineState)->geometryShaderName.empty())
    {
        list.emplace_back(GL_GEOMETRY_SHADER, (*pipelineState)->geometryShaderName);
    }

    result = LoadShaderProgram(list, pipeState->shaderProgram);

    *pipelineState = pipeState;

    return result;
}

void OGL_OpenGLPipelineStateCommonBaseManager::DestroyPipelineState(OGL_PipelineState& pipelineState)
{
}
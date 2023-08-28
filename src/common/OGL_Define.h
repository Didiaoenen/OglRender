#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>

namespace OGL
{
#ifdef ALIGN
#undef ALIGN
#endif

#define ALIGN(x, a) (((x) + ((a)-1)) & ~((a)-1))

#define MAX_LIGHTS 100

#define MAX_BONE 1000

#define MAX_BONE_INFLUENCE 4

enum class VertAttrib
{
    Position,
    Normal,
    TexCoord,
    Tangent,
    Bitangent,
};

enum class DepthTest
{
    NONE,
    LESS = GL_LESS,
    EQUAL,
    LEQUAL,
    GREATER,
    NOTEQUAL,
    GEQUAL,
    ALWAYS,
};

enum class StencilTest
{
    NONE
};

enum class CullFace 
{ 
    NONE, 
    FRONT = GL_FRONT, 
    BACK 
};

enum class DrawFlag
{ 
    NONE, 
    SHADOW, 
    DEBUG_DRAW 
};

enum class CompressedFormat
{
    NONE,
    DXT1,
    DXT2,
    DXT3,
    DXT4,
    DXT5,
    DXT10,
    BC1,
    BC1A,
    BC2,
    BC3,
    BC4,
    BC5,
    BC6H,
    BC7,
    PVRTC,
    ETC,
    ASTC_4x4,
    ASTC_5x4,
    ASTC_5x5,
    ASTC_6x5,
    ASTC_6x6,
    ASTC_8x5,
    ASTC_8x6,
    ASTC_8x8,
    ASTC_10x5,
    ASTC_10x6,
    ASTC_10x8,
    ASTC_10x10,
    ASTC_12x10,
    ASTC_12x12,
    ASTC_3x3x3,
    ASTC_4x3x3,
    ASTC_4x4x3,
    ASTC_4x4x4,
    ASTC_5x4x4,
    ASTC_5x5x4,
    ASTC_5x5x5,
    ASTC_6x5x5,
    ASTC_6x6x5,
    ASTC_6x6x6,
    UNKNOWN
};

enum class PixelFormat
{
    UNKNOWN,
    R8,
    RG8,
    RGB8,
    RGBA8,
    DEPTH,
};

enum class ImageDataType
{
    UNKNOWN,
    BYTE,
    UNSIGNED_BYTE,
};


enum class ImageDataFormat
{
    UNKNOWN,
    RED,
    RGB,
    RGBA
};

enum class PrimitiveType
{
    NONE = 0,
    POINT = 1 << 0,
    LINE = 1 << 1,
    TRIANGLE = 1 << 2,
    POLYGON = 1 << 3,
};

enum LightType
{
    UNDEFINED,
    DIRECTIONAL,
    POINT,
    SPOT,
    AREA,
};

enum class CameraType
{
    NONE,
    Perspective,
    Orthographic,
};

enum class CameraMovement 
{
    Forward,
    Backward,
    Left,
    Right
};

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texcoord;
    glm::vec3 tangent;
    glm::vec3 bitangent;
    int boneIDs[MAX_BONE_INFLUENCE];
    float weights[MAX_BONE_INFLUENCE];
};

struct BoneInfo
{
    int id;
    glm::mat4 offset;
};

struct BoneNode
{
    glm::mat4 transformation;
    std::string name;
    int childrenCount;
    std::vector<BoneNode> children;
};

struct TextureBase
{
	GLuint handler = 0;
	GLenum format = 0;
    PixelFormat pixelFormat;

	uint32_t width = 0;
	uint32_t height = 0;
	uint32_t mips = 1;
	uint32_t samples = 1;
};

struct TextureArrayBase : virtual TextureBase
{
	uint32_t size = 0;
};

struct TextureCube : virtual TextureBase
{

};

struct Texture2D : virtual TextureBase
{

};

struct TextureCubeArray : TextureCube, TextureArrayBase
{

};

struct Texture2DArray : Texture2D, TextureArrayBase
{

};

struct PerBatchConstants
{
    glm::mat4 modelMatrix;  // 64 bytes
};

struct PerFrameConstants
{
	glm::vec4 camPos;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
	int32_t numLights;
	int32_t clipSpaceType;
};

struct Light
{
    glm::vec4 lightPosition;                   // 16 bytes
    glm::vec4 lightDirection;                  // 16 bytes
    //LightType lightType;                      // 4 bytes
    //int lightCastShadow;                      // 4 bytes
    //int lightShadowMapIndex;                  // 4 bytes
    //float lightIntensity;                     // 4 bytes
    //glm::mat4 lightViewMatrix;               // 64 bytes
    //glm::mat4 lightProjectionMatrix;         // 64 bytes
    //glm::vec2 lightSize;                       // 8 bytes
    //glm::vec4 lightColor;                      // 16 bytes
};

struct LightInfo
{
    Light lights[MAX_LIGHTS];
};

struct ShadowMapConstants 
{
    int32_t light_index;          // 4 bytes
    float shadowmap_layer_index;  // 4 bytes
    float near_plane;             // 4 bytes
    float far_plane;              // 4 bytes
};

struct GlobalTexture
{
    TextureCubeArray skybox;
};

struct FrameTextures
{
	Texture2DArray shadowMap;
	Texture2DArray globalShadowMap;
	TextureCubeArray cubeShadowMap;
};

struct MaterialTexture
{
    Texture2D diffuseMap;
    Texture2D specularMap;
    Texture2D ambientMap;
    Texture2D emissiveMap;
    Texture2D normalMap;
    Texture2D lightMap;
};

const size_t kSizeLightInfo = ALIGN(sizeof(LightInfo), 256);
const size_t kSizePerFrameConstantBuffer = ALIGN(sizeof(PerFrameConstants), 256);
const size_t kSizePerBatchConstantBuffer = ALIGN(sizeof(PerBatchConstants), 256);

}

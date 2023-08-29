#pragma once

#include <map>
#include <vector>
#include <string>
#include <assimp/scene.h>

#include "sceneGraph/OGL_Asset.h"
#include "common/OGL_Define.h"

namespace OGL
{

class OGL_Mesh : public OGL_Asset
{
public:
    OGL_Mesh() = default;
    OGL_Mesh(const std::string& name): mName(name) {}
    virtual ~OGL_Mesh() = default;

    void SetVertexBoneDataToDefault(Vertex& vertex);
    void SetVertexBoneData(Vertex& vertex, int boneID, float weight);

    std::map<std::string, BoneInfo>& GetBoneInfoMap() { return mBoneInfoMap; }

public:
    std::string mName;
    std::vector<Vertex> mVertices;
    std::vector<uint32_t> mIndices;

    bool hasNormal{ false };
    bool hasVertexColors{ false };
    bool hasTextureCoords{ false };
    bool hasTangentsAndBitangents{ false };

    uint32_t mPrimitiveTypes{ 0 };

    int mBoneCounter{ 0 };
    std::map<std::string, BoneInfo> mBoneInfoMap;

private:

};
}
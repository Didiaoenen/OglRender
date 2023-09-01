
#include "common/OGL_AssimpGLMHelpers.h"
#include "OGL_Mesh.h"

using namespace OGL;

void OGL_Mesh::SetVertexBoneDataToDefault(Vertex& vertex)
{
	for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
	{
		vertex.boneIDs[i] = 0;
		vertex.weights[i] = 0.0f;
	}
}

void OGL_Mesh::SetVertexBoneData(Vertex& vertex, int boneID, float weight)
{
	for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
	{
		if (vertex.weights[i] == 0.0f)
		{
			vertex.boneIDs[i] = boneID;
			vertex.weights[i] = weight;
			break;
		}
	}
}
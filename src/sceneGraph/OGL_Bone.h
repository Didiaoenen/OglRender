#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <learnopengl/assimp_glm_helpers.h>

namespace OGL
{

struct KeyPosition
{
	glm::vec3 position;
	float timeStamp;
};

struct KeyRotation
{
	glm::quat orientation;
	float timeStamp;
};

struct KeyScale
{
	glm::vec3 scale;
	float timeStamp;
};

class OGL_Bone
{
public:
	OGL_Bone(const std::string& name, int ID)
		: mName(name), mID(ID), mLocalTransform(1.0f)
	{

	}

	void Tick(float dt);
	
	int GetPositionIndex(float animationTime);
	glm::mat4 InterpolatePosition(float animationTime);
	
	int GetRotationIndex(float animationTime);
	glm::mat4 InterpolateRotation(float animationTime);

	int GetScaleIndex(float animationTime);
	glm::mat4 InterpolateScaling(float animationTime);

	float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);

public:
	std::vector<KeyPosition> mPositions;
	std::vector<KeyRotation> mRotations;
	std::vector<KeyScale> mScales;
	int mNumPositions;
	int mNumRotations;
	int mNumScalings;

	glm::mat4 mLocalTransform;
	std::string mName;
	int mID;

};
}


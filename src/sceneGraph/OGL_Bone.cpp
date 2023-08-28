#include "OGL_Bone.h"

using namespace OGL;

void OGL_Bone::Tick(float dt)
{
	glm::mat4 translation = InterpolatePosition(dt);
	glm::mat4 rotation = InterpolateRotation(dt);
	glm::mat4 scale = InterpolateScaling(dt);
	mLocalTransform = translation * rotation * scale;
}

int OGL_Bone::GetPositionIndex(float animationTime)
{
	for (int index = 0; index < mNumPositions - 1; ++index)
	{
		if (animationTime < mPositions[index + 1].timeStamp)
		{
			return index;
		}
	}
	return 0;
}

glm::mat4 OGL_Bone::InterpolatePosition(float animationTime)
{
	if (1 == mNumPositions)
	{
		return glm::translate(glm::mat4(1.0f), mPositions[0].position);
	}

	int p0Index = GetPositionIndex(animationTime);
	float scaleFactor = GetScaleFactor(mPositions[p0Index].timeStamp, mPositions[p0Index + 1].timeStamp, animationTime);
	glm::vec3 finalPosition = glm::mix(mPositions[p0Index].position, mPositions[p0Index + 1].position, scaleFactor);
	return glm::translate(glm::mat4(1.0f), finalPosition);
}

int OGL_Bone::GetRotationIndex(float animationTime)
{
	for (int index = 0; index < mNumRotations - 1; ++index)
	{
		if (animationTime < mRotations[index + 1].timeStamp)
		{
			return index;
		}
	}
	return 0;
}

glm::mat4 OGL::OGL_Bone::InterpolateRotation(float animationTime)
{
	if (1 == mNumRotations)
	{
		auto rotation = glm::normalize(mRotations[0].orientation);
		return glm::toMat4(rotation);
	}

	int p0Index = GetRotationIndex(animationTime);
	float scaleFactor = GetScaleFactor(mRotations[p0Index].timeStamp, mRotations[p0Index + 1].timeStamp, animationTime);
	glm::quat finalRotation = glm::slerp(mRotations[p0Index].orientation, mRotations[p0Index + 1].orientation, scaleFactor);
	finalRotation = glm::normalize(finalRotation);
	return glm::toMat4(finalRotation);
}

int OGL_Bone::GetScaleIndex(float animationTime)
{
	for (int index = 0; index < mNumScalings - 1; ++index)
	{
		if (animationTime < mScales[index + 1].timeStamp)
		{
			return index;
		}
	}
	return 0;
}

glm::mat4 OGL_Bone::InterpolateScaling(float animationTime)
{
	if (1 == mNumScalings)
	{
		return glm::scale(glm::mat4(1.0f), mScales[0].scale);
	}

	int p0Index = GetScaleIndex(animationTime);
	float scaleFactor = GetScaleFactor(mScales[p0Index].timeStamp, mScales[p0Index + 1].timeStamp, animationTime);
	glm::vec3 finalScale = glm::mix(mScales[p0Index].scale, mScales[p0Index + 1].scale, scaleFactor);
	return glm::scale(glm::mat4(1.0f), finalScale);
}

float OGL_Bone::GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
{
	float scaleFactor = 0.0f;
	float midWayLength = animationTime - lastTimeStamp;
	float framesDiff = nextTimeStamp - lastTimeStamp;
	scaleFactor = midWayLength / framesDiff;
	return scaleFactor;
}

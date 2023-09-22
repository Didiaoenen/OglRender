#pragma once

#include <glm/glm.hpp>

#include <OglMaths/Maths_FTransform.h>

namespace Render
{
	struct Render_Light
	{
		enum class ELightType 
		{ 
			POINT, 
			DIRECTIONAL, 
			SPOT, 
			AMBIENT_BOX, 
			AMBIENT_SPHERE,
		};

		Render_Light(Maths::Maths_FTransform& pTranform, ELightType pType);

		glm::mat4 GenerateMatrix() const;

		float GetEffectRange() const;

		const Maths::Maths_FTransform& GetTransform() const;

		glm::vec3	mColor = { 1.f, 1.f, 1.f };
		float		mIntensity = 1.f;
		float		mConstant = 0.0f;
		float		mLinear = 0.0f;
		float		mQuadratic = 1.0f;
		float		mCutoff = 12.f;
		float		mOuterCutoff = 15.f;
		float		mType = 0.0f;

	protected:
		Maths::Maths_FTransform& mTransform;
	};
}


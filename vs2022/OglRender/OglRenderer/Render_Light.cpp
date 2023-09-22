#include "Render_Light.h"

uint32_t Pack(uint8_t c0, uint8_t c1, uint8_t c2, uint8_t c3)
{
	return (c0 << 24) | (c1 << 16) | (c2 << 8) | c3;
}

uint32_t Pack(const glm::vec3& pToPack)
{
	return Pack(static_cast<uint8_t>(pToPack.x * 255.f), static_cast<uint8_t>(pToPack.y * 255.f), static_cast<uint8_t>(pToPack.z * 255.f), 0);
}

Render::Render_Light::Render_Light(Maths::Maths_FTransform& pTranform, ELightType pType) :
	mTransform(pTranform), mType(static_cast<float>(pType))
{
}

glm::mat4 Render::Render_Light::GenerateMatrix() const
{
	glm::mat4 result;
	return result;
}

float CalculateLuminosity(float pConstant, float pLinear, float pQuadratic, float pIntensity, float pDistance)
{
	auto attenuation = (pConstant + pLinear * pDistance + pQuadratic * (pDistance * pDistance));
	return (1.0f / attenuation) * std::abs(pIntensity);
}

float CalculatePointLightRadius(float pConstant, float pLinear, float pQuadratic, float pIntensity)
{
	constexpr float threshold = 1 / 255.0f;
	constexpr float step = 1.0f;

	float distance = 0.0f;

#define TRY_GREATER(value)\
	else if (CalculateLuminosity(pConstant, pLinear, pQuadratic, pIntensity, value) > threshold)\
	{\
		distance = value;\
	}

#define TRY_LESS(value, newValue)\
	else if (CalculateLuminosity(pConstant, pLinear, pQuadratic, pIntensity, value) < threshold)\
	{\
		distance = newValue;\
	}

	if (CalculateLuminosity(pConstant, pLinear, pQuadratic, pIntensity, 1000.0f) > threshold)
	{
		return std::numeric_limits<float>::infinity();
	}

	TRY_LESS(20.0f, 0.0f)
		TRY_GREATER(750.0f)
		TRY_LESS(50.0f, 20.0f + step)
		TRY_LESS(100.0f, 50.0f + step)
		TRY_GREATER(500.0f)
		TRY_GREATER(250.0f)

		while (true)
		{
			if (CalculateLuminosity(pConstant, pLinear, pQuadratic, pIntensity, distance) < threshold)
			{
				return distance;
			}
			else
			{
				distance += step;
			}
		}
}

float CalculateAmbientBoxLightRadius(const glm::vec3& pPosition, const glm::vec3& pSize)
{
	return glm::distance(pPosition, pPosition + pSize);
}

float Render::Render_Light::GetEffectRange() const
{
	switch (static_cast<ELightType>(static_cast<int>(mType)))
	{
	case ELightType::POINT:
	case ELightType::SPOT:			return CalculatePointLightRadius(mConstant, mLinear, mQuadratic, mIntensity);
	case ELightType::AMBIENT_BOX:	return CalculateAmbientBoxLightRadius(mTransform.GetWorldPosition(), { mConstant, mLinear, mQuadratic });
	case ELightType::AMBIENT_SPHERE:	return mConstant;
	}

	return std::numeric_limits<float>::infinity();
}

const Maths::Maths_FTransform& Render::Render_Light::GetTransform() const
{
	return mTransform;
}

#include "Render_Light.h"

Render::Render_Light::Render_Light(Maths::Maths_FTransform& p_tranform, Type p_type) :
	m_transform(p_tranform), type(static_cast<float>(p_type))
{
}

glm::mat4 Render::Render_Light::GenerateMatrix() const
{
	return glm::mat4();
}

float Render::Render_Light::GetEffectRange() const
{
	return 0.0f;
}

const Maths::Maths_FTransform& Render::Render_Light::GetTransform() const
{
	return m_transform;
}

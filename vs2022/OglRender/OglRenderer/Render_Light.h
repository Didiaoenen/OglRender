#pragma once

#include <glm/glm.hpp>

#include <OglMaths/Maths_FTransform.h>

namespace Render
{
	struct Render_Light
	{
		enum class Type { POINT, DIRECTIONAL, SPOT, AMBIENT_BOX, AMBIENT_SPHERE };

		Render_Light(Maths::Maths_FTransform& p_tranform, Type p_type);

		glm::mat4 GenerateMatrix() const;

		float GetEffectRange() const;

		const Maths::Maths_FTransform& GetTransform() const;

		glm::vec3	color = { 1.f, 1.f, 1.f };
		float				intensity = 1.f;
		float				constant = 0.0f;
		float				linear = 0.0f;
		float				quadratic = 1.0f;
		float				cutoff = 12.f;
		float				outerCutoff = 15.f;
		float				type = 0.0f;

	protected:
		Maths::Maths_FTransform& m_transform;
	};
}


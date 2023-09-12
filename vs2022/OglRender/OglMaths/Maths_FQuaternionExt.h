#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Maths
{
	class Maths_FQuaternionExt
	{
	public:
		static glm::vec3 RotatePoint(const glm::vec3& p_point, const glm::quat& p_quaternion);
	};
}


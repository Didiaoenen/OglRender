#include "Maths_FQuaternionExt.h"

glm::vec3 Maths::Maths_FQuaternionExt::RotatePoint(const glm::vec3& p_point, const glm::quat& p_quaternion)
{
	glm::vec3 Q(p_quaternion.x, p_quaternion.y, p_quaternion.z);
	glm::vec3 T = glm::cross(Q, p_point) * 2.0f;

	return p_point + (T * p_quaternion.w) + glm::cross(Q, T);
}

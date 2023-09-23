#include "Maths_FQuaternionExt.h"

glm::vec3 Maths::Maths_FQuaternionExt::RotatePoint(const glm::vec3& pPoint, const glm::quat& pQuaternion)
{
	glm::vec3 Q(pQuaternion.x, pQuaternion.y, pQuaternion.z);
	glm::vec3 T = glm::cross(Q, pPoint) * 2.0f;

	return pPoint + (T * pQuaternion.w) + glm::cross(Q, T);
}

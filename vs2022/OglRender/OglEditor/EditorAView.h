#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Editor
{
class EditorAView
{
public:
	EditorAView
	(
		const std::string& pTitle,
		bool pOpened
	);

	virtual void Update(float pDeltaTime);

	void _Draw_Impl() override;

	virtual void _Render_Impl() = 0;

	void Render();

	void SetCameraPosition();

	void SetCameraRotaion();

	const glm::vec3& GetCameraPosition() const;

	const glm::quat& GetCameraRotation() const;

protected:
	void PrepareCamera();

protected:

};
}


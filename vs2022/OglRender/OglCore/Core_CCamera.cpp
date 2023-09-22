#include <OglUI/UI_ComboBox.h>
#include <OglUI/UI_DragFloat.h>
#include <OglUI/UI_DataDispatcher.h>

#include "Core_Actor.h"
#include "Core_CCamera.h"

Core::Core_CCamera::Core_CCamera(Core_Actor& pOwner) :
	Core_AComponent(pOwner)
{
	SetClearColor({ 0.1921569f, 0.3019608f, 0.4745098f });
}

std::string Core::Core_CCamera::GetName()
{
	return "Camera";
}

void Core::Core_CCamera::SetFov(float pValue)
{
	mCamera.SetFov(pValue);
}

void Core::Core_CCamera::SetSize(float pValue)
{
	mCamera.SetSize(pValue);
}

void Core::Core_CCamera::SetNear(float pValue)
{
	mCamera.SetNear(pValue);
}

void Core::Core_CCamera::SetFar(float pValue)
{
	mCamera.SetFar(pValue);
}

void Core::Core_CCamera::SetClearColor(const glm::vec3& pClearColor)
{
	mCamera.SetClearColor(pClearColor);
}

void Core::Core_CCamera::SetFrustumGeometryCulling(bool pEnable)
{
	mCamera.SetFrustumGeometryCulling(pEnable);
}

void Core::Core_CCamera::SetFrustumLightCulling(bool pEnable)
{
	mCamera.SetFrustumLightCulling(pEnable);
}

void Core::Core_CCamera::SetProjectionMode(Render::EProjectionMode pProjectionMode)
{
	mCamera.SetProjectionMode(pProjectionMode);
}

float Core::Core_CCamera::GetFov() const
{
	return mCamera.GetFov();
}

float Core::Core_CCamera::GetSize() const
{
	return mCamera.GetSize();
}

float Core::Core_CCamera::GetNear() const
{
	return mCamera.GetNear();
}

float Core::Core_CCamera::GetFar() const
{
	return mCamera.GetFar();
}

const glm::vec3& Core::Core_CCamera::GetClearColor() const
{
	return mCamera.GetClearColor();
}

bool Core::Core_CCamera::HasFrustumGeometryCulling() const
{
	return mCamera.HasFrustumGeometryCulling();
}

bool Core::Core_CCamera::HasFrustumLightCulling() const
{
	return mCamera.HasFrustumLightCulling();
}

Render::EProjectionMode Core::Core_CCamera::GetProjectionMode() const
{
	return mCamera.GetProjectionMode();
}

Render::Render_Camera& Core::Core_CCamera::GetCamera()
{
	return mCamera;
}

void Core::Core_CCamera::OnSerialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode)
{
	Core_Serializer::SerializeFloat(pDoc, pNode, "fov", mCamera.GetFov());
	Core_Serializer::SerializeFloat(pDoc, pNode, "size", mCamera.GetSize());
	Core_Serializer::SerializeFloat(pDoc, pNode, "near", mCamera.GetNear());
	Core_Serializer::SerializeFloat(pDoc, pNode, "far", mCamera.GetFar());
	Core_Serializer::SerializeVec3(pDoc, pNode, "clear_color", mCamera.GetClearColor());
	Core_Serializer::SerializeBoolean(pDoc, pNode, "frustum_geometry_culling", mCamera.HasFrustumGeometryCulling());
	Core_Serializer::SerializeBoolean(pDoc, pNode, "frustum_light_culling", mCamera.HasFrustumLightCulling());
	Core_Serializer::SerializeInt(pDoc, pNode, "projection_mode", static_cast<int>(mCamera.GetProjectionMode()));
}

void Core::Core_CCamera::OnDeserialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode)
{
	mCamera.SetFov(Core_Serializer::DeserializeFloat(pDoc, pNode, "fov"));
	mCamera.SetSize(Core_Serializer::DeserializeFloat(pDoc, pNode, "size"));
	mCamera.SetNear(Core_Serializer::DeserializeFloat(pDoc, pNode, "near"));
	mCamera.SetFar(Core_Serializer::DeserializeFloat(pDoc, pNode, "far"));
	mCamera.SetClearColor(Core_Serializer::DeserializeVec3(pDoc, pNode, "clear_color"));
	mCamera.SetFrustumGeometryCulling(Core_Serializer::DeserializeBoolean(pDoc, pNode, "frustum_geometry_culling"));
	mCamera.SetFrustumLightCulling(Core_Serializer::DeserializeBoolean(pDoc, pNode, "frustum_light_culling"));

	if (pNode->FirstChildElement("projection_mode"))
	{
		mCamera.SetProjectionMode(static_cast<Render::EProjectionMode>(Core_Serializer::DeserializeInt(pDoc, pNode, "projection_mode")));
	}
}

void Core::Core_CCamera::OnInspector(UI::UI_WidgetContainer& pRoot)
{
	auto currentProjectionMode = GetProjectionMode();

	Core_GUIDrawer::DrawScalar<float>(pRoot, "Field of view", std::bind(&Core_CCamera::GetFov, this), std::bind(&Core_CCamera::SetFov, this, std::placeholders::_1));
	auto& fovWidget = *pRoot.GetWidgets()[pRoot.GetWidgets().size() - 1].first;
	auto& fovWidgetLabel = *pRoot.GetWidgets()[pRoot.GetWidgets().size() - 2].first;
	fovWidget.mEnabled = fovWidgetLabel.mEnabled = currentProjectionMode == Render::EProjectionMode::PERSPECTIVE;

	Core_GUIDrawer::DrawScalar<float>(pRoot, "Size", std::bind(&Core_CCamera::GetSize, this), std::bind(&Core_CCamera::SetSize, this, std::placeholders::_1));
	auto& sizeWidget = *pRoot.GetWidgets()[pRoot.GetWidgets().size() - 1].first;
	auto& sizeWidgetLabel = *pRoot.GetWidgets()[pRoot.GetWidgets().size() - 2].first;
	sizeWidget.mEnabled = sizeWidgetLabel.mEnabled = currentProjectionMode == Render::EProjectionMode::ORTHOGRAPHIC;

	Core_GUIDrawer::DrawScalar<float>(pRoot, "Near", std::bind(&Core_CCamera::GetNear, this), std::bind(&Core_CCamera::SetNear, this, std::placeholders::_1));
	Core_GUIDrawer::DrawScalar<float>(pRoot, "Far", std::bind(&Core_CCamera::GetFar, this), std::bind(&Core_CCamera::SetFar, this, std::placeholders::_1));
	Core_GUIDrawer::DrawColor(pRoot, "Clear color", [this]() {return reinterpret_cast<const UI::Color&>(GetClearColor()); }, [this](UI::Color pColor) { SetClearColor({ pColor.r, pColor.g, pColor.b }); }, false);
	Core_GUIDrawer::DrawBoolean(pRoot, "Frustum Geometry Culling", std::bind(&Core_CCamera::HasFrustumGeometryCulling, this), std::bind(&Core_CCamera::SetFrustumGeometryCulling, this, std::placeholders::_1));
	Core_GUIDrawer::DrawBoolean(pRoot, "Frustum Light Culling", std::bind(&Core_CCamera::HasFrustumLightCulling, this), std::bind(&Core_CCamera::SetFrustumLightCulling, this, std::placeholders::_1));

	Core_GUIDrawer::CreateTitle(pRoot, "Projection Mode");
	auto& projectionMode = pRoot.CreateWidget<UI::UI_ComboBox>(static_cast<int>(GetProjectionMode()));
	projectionMode.mChoices.emplace(0, "Orthographic");
	projectionMode.mChoices.emplace(1, "Perspective");
	projectionMode.mValueChangedEvent += [this, &fovWidget, &fovWidgetLabel, &sizeWidget, &sizeWidgetLabel](int p_choice)
		{
			const auto newProjectionMode = static_cast<Render::EProjectionMode>(p_choice);
			SetProjectionMode(newProjectionMode);
			fovWidget.mEnabled = fovWidgetLabel.mEnabled = newProjectionMode == Render::EProjectionMode::PERSPECTIVE;
			sizeWidget.mEnabled = sizeWidgetLabel.mEnabled = newProjectionMode == Render::EProjectionMode::ORTHOGRAPHIC;
		};
}

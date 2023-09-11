#include <OglUI/UI_DragFloat.h>
#include <OglUI/UI_ComboBox.h>
#include <OglUI/UI_DataDispatcher.h>

#include "Core_Actor.h"
#include "Core_CCamera.h"

Core::Core_CCamera::Core_CCamera(Core_Actor& p_owner) :
	Core_AComponent(p_owner)
{
	SetClearColor({ 0.1921569f, 0.3019608f, 0.4745098f });
}

std::string Core::Core_CCamera::GetName()
{
	return "Camera";
}

void Core::Core_CCamera::SetFov(float pValue)
{
	m_camera.SetFov(pValue);
}

void Core::Core_CCamera::SetSize(float pValue)
{
	m_camera.SetSize(pValue);
}

void Core::Core_CCamera::SetNear(float pValue)
{
	m_camera.SetNear(pValue);
}

void Core::Core_CCamera::SetFar(float pValue)
{
	m_camera.SetFar(pValue);
}

void Core::Core_CCamera::SetClearColor(const glm::vec3& p_clearColor)
{
	m_camera.SetClearColor(p_clearColor);
}

void Core::Core_CCamera::SetFrustumGeometryCulling(bool p_enable)
{
	m_camera.SetFrustumGeometryCulling(p_enable);
}

void Core::Core_CCamera::SetFrustumLightCulling(bool p_enable)
{
	m_camera.SetFrustumLightCulling(p_enable);
}

void Core::Core_CCamera::SetProjectionMode(Render::EProjectionMode p_projectionMode)
{
	m_camera.SetProjectionMode(p_projectionMode);
}

float Core::Core_CCamera::GetFov() const
{
	return m_camera.GetFov();
}

float Core::Core_CCamera::GetSize() const
{
	return m_camera.GetSize();
}

float Core::Core_CCamera::GetNear() const
{
	return m_camera.GetNear();
}

float Core::Core_CCamera::GetFar() const
{
	return m_camera.GetFar();
}

const glm::vec3& Core::Core_CCamera::GetClearColor() const
{
	return m_camera.GetClearColor();
}

bool Core::Core_CCamera::HasFrustumGeometryCulling() const
{
	return m_camera.HasFrustumGeometryCulling();
}

bool Core::Core_CCamera::HasFrustumLightCulling() const
{
	return m_camera.HasFrustumLightCulling();
}

Render::EProjectionMode Core::Core_CCamera::GetProjectionMode() const
{
	return m_camera.GetProjectionMode();
}

Render::Render_Camera& Core::Core_CCamera::GetCamera()
{
	return m_camera;
}

void Core::Core_CCamera::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	Core_Serializer::SerializeFloat(p_doc, p_node, "fov", m_camera.GetFov());
	Core_Serializer::SerializeFloat(p_doc, p_node, "size", m_camera.GetSize());
	Core_Serializer::SerializeFloat(p_doc, p_node, "near", m_camera.GetNear());
	Core_Serializer::SerializeFloat(p_doc, p_node, "far", m_camera.GetFar());
	Core_Serializer::SerializeVec3(p_doc, p_node, "clear_color", m_camera.GetClearColor());
	Core_Serializer::SerializeBoolean(p_doc, p_node, "frustum_geometry_culling", m_camera.HasFrustumGeometryCulling());
	Core_Serializer::SerializeBoolean(p_doc, p_node, "frustum_light_culling", m_camera.HasFrustumLightCulling());
	Core_Serializer::SerializeInt(p_doc, p_node, "projection_mode", static_cast<int>(m_camera.GetProjectionMode()));
}

void Core::Core_CCamera::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	m_camera.SetFov(Core_Serializer::DeserializeFloat(p_doc, p_node, "fov"));
	m_camera.SetSize(Core_Serializer::DeserializeFloat(p_doc, p_node, "size"));
	m_camera.SetNear(Core_Serializer::DeserializeFloat(p_doc, p_node, "near"));
	m_camera.SetFar(Core_Serializer::DeserializeFloat(p_doc, p_node, "far"));
	m_camera.SetClearColor(Core_Serializer::DeserializeVec3(p_doc, p_node, "clear_color"));
	m_camera.SetFrustumGeometryCulling(Core_Serializer::DeserializeBoolean(p_doc, p_node, "frustum_geometry_culling"));
	m_camera.SetFrustumLightCulling(Core_Serializer::DeserializeBoolean(p_doc, p_node, "frustum_light_culling"));

	if (p_node->FirstChildElement("projection_mode"))
	{
		m_camera.SetProjectionMode(static_cast<Render::EProjectionMode>(Core_Serializer::DeserializeInt(p_doc, p_node, "projection_mode")));
	}
}

void Core::Core_CCamera::OnInspector(UI::UI_WidgetContainer& p_root)
{
	auto currentProjectionMode = GetProjectionMode();

	Core_GUIDrawer::DrawScalar<float>(p_root, "Field of view", std::bind(&Core_CCamera::GetFov, this), std::bind(&Core_CCamera::SetFov, this, std::placeholders::_1));
	auto& fovWidget = *p_root.GetWidgets()[p_root.GetWidgets().size() - 1].first;
	auto& fovWidgetLabel = *p_root.GetWidgets()[p_root.GetWidgets().size() - 2].first;
	fovWidget.mEnabled = fovWidgetLabel.mEnabled = currentProjectionMode == Render::EProjectionMode::PERSPECTIVE;

	Core_GUIDrawer::DrawScalar<float>(p_root, "Size", std::bind(&Core_CCamera::GetSize, this), std::bind(&Core_CCamera::SetSize, this, std::placeholders::_1));
	auto& sizeWidget = *p_root.GetWidgets()[p_root.GetWidgets().size() - 1].first;
	auto& sizeWidgetLabel = *p_root.GetWidgets()[p_root.GetWidgets().size() - 2].first;
	sizeWidget.mEnabled = sizeWidgetLabel.mEnabled = currentProjectionMode == Render::EProjectionMode::ORTHOGRAPHIC;

	Core_GUIDrawer::DrawScalar<float>(p_root, "Near", std::bind(&Core_CCamera::GetNear, this), std::bind(&Core_CCamera::SetNear, this, std::placeholders::_1));
	Core_GUIDrawer::DrawScalar<float>(p_root, "Far", std::bind(&Core_CCamera::GetFar, this), std::bind(&Core_CCamera::SetFar, this, std::placeholders::_1));
	Core_GUIDrawer::DrawColor(p_root, "Clear color", [this]() {return reinterpret_cast<const UI::Color&>(GetClearColor()); }, [this](UI::Color p_color) { SetClearColor({ p_color.r, p_color.g, p_color.b }); }, false);
	Core_GUIDrawer::DrawBoolean(p_root, "Frustum Geometry Culling", std::bind(&Core_CCamera::HasFrustumGeometryCulling, this), std::bind(&Core_CCamera::SetFrustumGeometryCulling, this, std::placeholders::_1));
	Core_GUIDrawer::DrawBoolean(p_root, "Frustum Light Culling", std::bind(&Core_CCamera::HasFrustumLightCulling, this), std::bind(&Core_CCamera::SetFrustumLightCulling, this, std::placeholders::_1));

	Core_GUIDrawer::CreateTitle(p_root, "Projection Mode");
	auto& projectionMode = p_root.CreateWidget<UI::UI_ComboBox>(static_cast<int>(GetProjectionMode()));
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

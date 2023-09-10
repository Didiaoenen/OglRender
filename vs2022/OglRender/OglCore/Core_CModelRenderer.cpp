#include <OglUI/UI_Text.h>
#include <OglUI/UI_TextColored.h>
#include <OglUI/UI_DDTarget.h>
#include <OglUI/UI_Image.h>
#include <OglUI/UI_ComboBox.h>
#include <OglUI/UI_CheckBox.h>
#include <OglUI/UI_DragFloat.h>

#include "Core_Actor.h"
#include "Core_ServiceLocator.h"
#include "Core_TextureManager.h"
#include "Core_ModelManager.h"
#include "Core_ShaderManager.h"
#include "Core_CMaterialRenderer.h"
#include "Core_CModelRenderer.h"

Core::Core_CModelRenderer::Core_CModelRenderer(Core_Actor& p_owner) :
	Core_AComponent(p_owner)
{
	m_modelChangedEvent += [this]
		{
			if (auto materialRenderer = owner.GetComponent<Core_CMaterialRenderer>())
				materialRenderer->UpdateMaterialList();
		};
}

std::string Core::Core_CModelRenderer::GetName()
{
	return "Model Renderer";
}

void Core::Core_CModelRenderer::SetModel(Render::Render_Model* p_model)
{
	m_model = p_model;
	m_modelChangedEvent.Invoke();
}

Render::Render_Model* Core::Core_CModelRenderer::GetModel() const
{
	return m_model;
}

void Core::Core_CModelRenderer::SetFrustumBehaviour(EFrustumBehaviour p_boundingMode)
{
	m_frustumBehaviour = p_boundingMode;
}

Core::Core_CModelRenderer::EFrustumBehaviour Core::Core_CModelRenderer::GetFrustumBehaviour() const
{
	return m_frustumBehaviour;
}

const Render::Render_BoundingSphere& Core::Core_CModelRenderer::GetCustomBoundingSphere() const
{
	return m_customBoundingSphere;
}

void Core::Core_CModelRenderer::SetCustomBoundingSphere(const Render::Render_BoundingSphere& p_boundingSphere)
{
	m_customBoundingSphere = p_boundingSphere;
}

void Core::Core_CModelRenderer::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	Core_Serializer::SerializeModel(p_doc, p_node, "model", m_model);
	Core_Serializer::SerializeInt(p_doc, p_node, "frustum_behaviour", reinterpret_cast<int&>(m_frustumBehaviour));
	Core_Serializer::SerializeVec3(p_doc, p_node, "custom_bounding_sphere_position", m_customBoundingSphere.position);
	Core_Serializer::SerializeFloat(p_doc, p_node, "custom_bounding_sphere_radius", m_customBoundingSphere.radius);
}

void Core::Core_CModelRenderer::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	Core_Serializer::DeserializeModel(p_doc, p_node, "model", m_model);
	Core_Serializer::DeserializeInt(p_doc, p_node, "frustum_behaviour", reinterpret_cast<int&>(m_frustumBehaviour));
	Core_Serializer::DeserializeVec3(p_doc, p_node, "custom_bounding_sphere_position", m_customBoundingSphere.position);
	Core_Serializer::DeserializeFloat(p_doc, p_node, "custom_bounding_sphere_radius", m_customBoundingSphere.radius);
}

void Core::Core_CModelRenderer::OnInspector(UI::UI_WidgetContainer& p_root)
{
	Core_GUIDrawer::DrawMesh(p_root, "Model", m_model, &m_modelChangedEvent);

	Core_GUIDrawer::CreateTitle(p_root, "Frustum Culling Behaviour");
	auto& boundingMode = p_root.CreateWidget<UI::UI_ComboBox>(0);
	boundingMode.mChoices.emplace(0, "Disabled");
	boundingMode.mChoices.emplace(1, "Cull model");
	boundingMode.mChoices.emplace(2, "Cull model & sub-meshes");
	boundingMode.mChoices.emplace(3, "Cull custom bounding sphere");
	auto& boundingModeDispatcher = boundingMode.AddPlugin<UI::UI_DataDispatcher<int>>();
	boundingModeDispatcher.RegisterReference(reinterpret_cast<int&>(m_frustumBehaviour));

	auto& centerLabel = p_root.CreateWidget<UI::UI_TextColored>("Bounding Sphere Center", Core_GUIDrawer::TitleColor);
	auto& centerWidget = p_root.CreateWidget<UI::UI_DragMultipleScalars<float, 3>>(Core_GUIDrawer::GetDataType<float>(), Core_GUIDrawer::_MIN_FLOAT, Core_GUIDrawer::_MAX_FLOAT, 0.f, 0.05f, "", Core_GUIDrawer::GetFormat<float>());
	auto& centerDispatcher = centerWidget.AddPlugin<UI::UI_DataDispatcher<std::array<float, 3>>>();
	centerDispatcher.RegisterReference(reinterpret_cast<std::array<float, 3>&>(m_customBoundingSphere.position));

	auto& radiusLabel = p_root.CreateWidget<UI::UI_TextColored>("Bounding Sphere Radius", Core_GUIDrawer::TitleColor);
	auto& radiusWidget = p_root.CreateWidget<UI::UI_DragFloat>(0.0f, Core_GUIDrawer::_MAX_FLOAT, 0.f, 0.1f);
	auto& radiusDispatcher = radiusWidget.AddPlugin<UI::UI_DataDispatcher<float>>();
	radiusDispatcher.RegisterReference(m_customBoundingSphere.radius);

	boundingMode.mValueChangedEvent += [&](int p_choice)
		{
			centerLabel.mEnabled = centerWidget.mEnabled = radiusLabel.mEnabled = radiusWidget.mEnabled = p_choice == 3;
		};

	centerLabel.mEnabled = centerWidget.mEnabled = radiusLabel.mEnabled = radiusWidget.mEnabled = m_frustumBehaviour == EFrustumBehaviour::CULL_CUSTOM;
}

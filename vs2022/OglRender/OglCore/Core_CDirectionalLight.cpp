#include <OglUI/UI_Text.h>
#include <OglUI/UI_DragFloat.h>
#include <OglUI/UI_ColorEdit.h>

#include "Core_Actor.h"
#include "Core_CDirectionalLight.h"

Core::Core_CDirectionalLight::Core_CDirectionalLight(Core_Actor& p_owner) :
	Core_CLight(p_owner)
{
	m_data.type = static_cast<float>(Render::Render_Light::Type::DIRECTIONAL);
}

std::string Core::Core_CDirectionalLight::GetName()
{
	return "Directional Light";
}

void Core::Core_CDirectionalLight::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	Core_CLight::OnSerialize(p_doc, p_node);
}

void Core::Core_CDirectionalLight::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	Core_CLight::OnDeserialize(p_doc, p_node);
}

void Core::Core_CDirectionalLight::OnInspector(UI::UI_WidgetContainer& p_root)
{
	Core_CLight::OnInspector(p_root);
}

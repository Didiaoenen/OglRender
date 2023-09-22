#include <OglUI/UI_Text.h>
#include <OglUI/UI_DragFloat.h>
#include <OglUI/UI_ColorEdit.h>

#include "Core_Actor.h"
#include "Core_CDirectionalLight.h"

Core::Core_CDirectionalLight::Core_CDirectionalLight(Core_Actor& pOwner) :
	Core_CLight(pOwner)
{
	mData.mType = static_cast<float>(Render::Render_Light::ELightType::DIRECTIONAL);
}

std::string Core::Core_CDirectionalLight::GetName()
{
	return "Directional Light";
}

void Core::Core_CDirectionalLight::OnSerialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode)
{
	Core_CLight::OnSerialize(pDoc, pNode);
}

void Core::Core_CDirectionalLight::OnDeserialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode)
{
	Core_CLight::OnDeserialize(pDoc, pNode);
}

void Core::Core_CDirectionalLight::OnInspector(UI::UI_WidgetContainer& pRoot)
{
	Core_CLight::OnInspector(pRoot);
}

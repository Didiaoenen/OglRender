#include <OglUI/UI_Text.h>
#include <OglUI/UI_Group.h>
#include <OglUI/UI_Button.h>
#include <OglUI/UI_DragFloat.h>
#include <OglUI/UI_ColorEdit.h>

#include "Core_Actor.h"
#include "Core_CLight.h"

Core::Core_CLight::Core_CLight(Core_Actor& pOwner) : 
	Core_AComponent(pOwner), 
	mData(pOwner.transform.GetFTransform(), {})
{
}

const Render::Render_Light& Core::Core_CLight::GetData() const
{
	return mData;
}

const glm::vec3& Core::Core_CLight::GetColor() const
{
	return mData.color;
}

float Core::Core_CLight::GetIntensity() const
{
	return mData.intensity;
}

void Core::Core_CLight::SetColor(const glm::vec3& pColor)
{
	mData.color = pColor;
}

void Core::Core_CLight::SetIntensity(float pIntensity)
{
	mData.intensity = pIntensity;
}

void Core::Core_CLight::OnSerialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode)
{
	Core_Serializer::SerializeVec3(pDoc, pNode, "color", mData.color);
	Core_Serializer::SerializeFloat(pDoc, pNode, "intensity", mData.intensity);
}

void Core::Core_CLight::OnDeserialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode)
{
	Core_Serializer::DeserializeVec3(pDoc, pNode, "color", mData.color);
	Core_Serializer::DeserializeFloat(pDoc, pNode, "intensity", mData.intensity);
}

void Core::Core_CLight::OnInspector(UI::UI_WidgetContainer& pRoot)
{
	Core_GUIDrawer::DrawColor(pRoot, "Color", reinterpret_cast<UI::Color&>(mData.color));
	Core_GUIDrawer::DrawScalar<float>(pRoot, "Intensity", mData.intensity, 0.005f, Core_GUIDrawer::_MIN_FLOAT, Core_GUIDrawer::_MAX_FLOAT);
}

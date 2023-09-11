#include <OglUI/UI_Text.h>
#include <OglUI/UI_DragFloat.h>
#include <OglUI/UI_ColorEdit.h>
#include <OglUI/UI_Bullet.h>
#include <OglUI/UI_Group.h>

#include "Core_Actor.h"
#include "Core_CLight.h"

Core::Core_CLight::Core_CLight(Core_Actor& p_owner) : 
	Core_AComponent(p_owner), 
	mData(p_owner.transform.GetFTransform(), {})
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

void Core::Core_CLight::SetColor(const glm::vec3& p_color)
{
	mData.color = p_color;
}

void Core::Core_CLight::SetIntensity(float p_intensity)
{
	mData.intensity = p_intensity;
}

void Core::Core_CLight::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	Core_Serializer::SerializeVec3(p_doc, p_node, "color", mData.color);
	Core_Serializer::SerializeFloat(p_doc, p_node, "intensity", mData.intensity);
}

void Core::Core_CLight::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	Core_Serializer::DeserializeVec3(p_doc, p_node, "color", mData.color);
	Core_Serializer::DeserializeFloat(p_doc, p_node, "intensity", mData.intensity);
}

void Core::Core_CLight::OnInspector(UI::UI_WidgetContainer& p_root)
{
	Core_GUIDrawer::DrawColor(p_root, "Color", reinterpret_cast<UI::Color&>(mData.color));
	Core_GUIDrawer::DrawScalar<float>(p_root, "Intensity", mData.intensity, 0.005f, Core_GUIDrawer::_MIN_FLOAT, Core_GUIDrawer::_MAX_FLOAT);
}

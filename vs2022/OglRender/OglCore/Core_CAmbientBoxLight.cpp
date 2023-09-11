#include <OglUI/UI_Text.h>
#include <OglUI/UI_DragFloat.h>
#include <OglUI/UI_Bullet.h>
#include <OglUI/UI_ColorEdit.h>

#include "Core_Actor.h"
#include "Core_CAmbientBoxLight.h"

Core::Core_CAmbientBoxLight::Core_CAmbientBoxLight(Core_Actor& p_owner) :
	Core_CLight(p_owner)
{
	mData.type = static_cast<float>(Render::Render_Light::Type::AMBIENT_BOX);

	mData.intensity = 0.1f;
	mData.constant = 1.0f;
	mData.linear = 1.0f;
	mData.quadratic = 1.0f;
}

std::string Core::Core_CAmbientBoxLight::GetName()
{
	return "Ambient Box Light";
}

glm::vec3 Core::Core_CAmbientBoxLight::GetSize() const
{
	return { mData.constant, mData.linear, mData.quadratic };
}

void Core::Core_CAmbientBoxLight::SetSize(const glm::vec3& p_size)
{
	mData.constant = p_size.x;
	mData.linear = p_size.y;
	mData.quadratic = p_size.z;
}

void Core::Core_CAmbientBoxLight::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	Core_CLight::OnSerialize(p_doc, p_node);
	
	Core_Serializer::SerializeVec3(p_doc, p_node, "size", { mData.constant, mData.linear, mData.quadratic });
}

void Core::Core_CAmbientBoxLight::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	Core_CLight::OnDeserialize(p_doc, p_node);

	glm::vec3 size = Core_Serializer::DeserializeVec3(p_doc, p_node, "size");
	mData.constant = size.x;
	mData.linear = size.y;
	mData.quadratic = size.z;
}

void Core::Core_CAmbientBoxLight::OnInspector(UI::UI_WidgetContainer& p_root)
{
	Core_CLight::OnInspector(p_root);

	auto sizeGatherer = [this]() -> glm::vec3 { return { mData.constant, mData.linear, mData.quadratic }; };
	auto sizeProvider = [this](const glm::vec3 &p_data) { mData.constant = p_data.x; mData.linear = p_data.y, mData.quadratic = p_data.z; };

	Core_GUIDrawer::DrawVec3(p_root, "Size", sizeGatherer, sizeProvider, 0.1f, 0.f);
}

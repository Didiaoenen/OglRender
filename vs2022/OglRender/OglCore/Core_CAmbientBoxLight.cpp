#include <OglUI/UI_Text.h>
#include <OglUI/UI_DragFloat.h>
#include <OglUI/UI_Bullet.h>
#include <OglUI/UI_ColorEdit.h>

#include "Core_Actor.h"
#include "Core_CAmbientBoxLight.h"

Core::Core_CAmbientBoxLight::Core_CAmbientBoxLight(Core_Actor& pOwner) :
	Core_CLight(pOwner)
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

void Core::Core_CAmbientBoxLight::SetSize(const glm::vec3& pSize)
{
	mData.constant = pSize.x;
	mData.linear = pSize.y;
	mData.quadratic = pSize.z;
}

void Core::Core_CAmbientBoxLight::OnSerialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode)
{
	Core_CLight::OnSerialize(pDoc, pNode);
	
	Core_Serializer::SerializeVec3(pDoc, pNode, "size", { mData.constant, mData.linear, mData.quadratic });
}

void Core::Core_CAmbientBoxLight::OnDeserialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode)
{
	Core_CLight::OnDeserialize(pDoc, pNode);

	glm::vec3 size = Core_Serializer::DeserializeVec3(pDoc, pNode, "size");
	mData.constant = size.x;
	mData.linear = size.y;
	mData.quadratic = size.z;
}

void Core::Core_CAmbientBoxLight::OnInspector(UI::UI_WidgetContainer& pRoot)
{
	Core_CLight::OnInspector(pRoot);

	auto sizeGatherer = [this]() -> glm::vec3 { return { mData.constant, mData.linear, mData.quadratic }; };
	auto sizeProvider = [this](const glm::vec3 &p_data) { mData.constant = p_data.x; mData.linear = p_data.y, mData.quadratic = p_data.z; };

	Core_GUIDrawer::DrawVec3(pRoot, "Size", sizeGatherer, sizeProvider, 0.1f, 0.f);
}

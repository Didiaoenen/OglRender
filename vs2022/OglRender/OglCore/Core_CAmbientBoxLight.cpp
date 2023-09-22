#include <OglUI/UI_Text.h>
#include <OglUI/UI_DragFloat.h>
#include <OglUI/UI_Bullet.h>
#include <OglUI/UI_ColorEdit.h>

#include "Core_Actor.h"
#include "Core_CAmbientBoxLight.h"

Core::Core_CAmbientBoxLight::Core_CAmbientBoxLight(Core_Actor& pOwner) :
	Core_CLight(pOwner)
{
	mData.mType = static_cast<float>(Render::Render_Light::ELightType::AMBIENT_BOX);

	mData.mIntensity = 0.1f;
	mData.mConstant = 1.0f;
	mData.mLinear = 1.0f;
	mData.mQuadratic = 1.0f;
}

std::string Core::Core_CAmbientBoxLight::GetName()
{
	return "Ambient Box Light";
}

glm::vec3 Core::Core_CAmbientBoxLight::GetSize() const
{
	return { mData.mConstant, mData.mLinear, mData.mQuadratic };
}

void Core::Core_CAmbientBoxLight::SetSize(const glm::vec3& pSize)
{
	mData.mConstant = pSize.x;
	mData.mLinear = pSize.y;
	mData.mQuadratic = pSize.z;
}

void Core::Core_CAmbientBoxLight::OnSerialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode)
{
	Core_CLight::OnSerialize(pDoc, pNode);
	
	Core_Serializer::SerializeVec3(pDoc, pNode, "size", { mData.mConstant, mData.mLinear, mData.mQuadratic });
}

void Core::Core_CAmbientBoxLight::OnDeserialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode)
{
	Core_CLight::OnDeserialize(pDoc, pNode);

	glm::vec3 size = Core_Serializer::DeserializeVec3(pDoc, pNode, "size");
	mData.mConstant = size.x;
	mData.mLinear = size.y;
	mData.mQuadratic = size.z;
}

void Core::Core_CAmbientBoxLight::OnInspector(UI::UI_WidgetContainer& pRoot)
{
	Core_CLight::OnInspector(pRoot);

	auto sizeGatherer = [this]() -> glm::vec3 { return { mData.mConstant, mData.mLinear, mData.mQuadratic }; };
	auto sizeProvider = [this](const glm::vec3 &pData) { mData.mConstant = pData.x; mData.mLinear = pData.y, mData.mQuadratic = pData.z; };

	Core_GUIDrawer::DrawVec3(pRoot, "Size", sizeGatherer, sizeProvider, 0.1f, 0.f);
}

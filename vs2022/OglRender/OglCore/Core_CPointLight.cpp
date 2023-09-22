#include <OglUI/UI_Text.h>
#include <OglUI/UI_Group.h>
#include <OglUI/UI_Button.h>
#include <OglUI/UI_DragFloat.h>
#include <OglUI/UI_ColorEdit.h>

#include "Core_Actor.h"
#include "Core_CPointLight.h"

Core::Core_CPointLight::Core_CPointLight(Core_Actor& pOwner) :
	Core_CLight(pOwner)
{
	mData.mType = static_cast<float>(Render::Render_Light::ELightType::POINT);
}

std::string Core::Core_CPointLight::GetName()
{
	return "Point Light";
}

float Core::Core_CPointLight::GetConstant() const
{
	return mData.mConstant;
}

float Core::Core_CPointLight::GetLinear() const
{
	return mData.mLinear;
}

float Core::Core_CPointLight::GetQuadratic() const
{
	return mData.mQuadratic;
}

void Core::Core_CPointLight::SetConstant(float pConstant)
{
	mData.mConstant = pConstant;
}

void Core::Core_CPointLight::SetLinear(float pLinear)
{
	mData.mLinear = pLinear;
}

void Core::Core_CPointLight::SetQuadratic(float pQuadratic)
{
	mData.mQuadratic = pQuadratic;
}

void Core::Core_CPointLight::OnSerialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode)
{
	Core_CLight::OnSerialize(pDoc, pNode);

	Core_Serializer::SerializeFloat(pDoc, pNode, "mConstant", mData.mConstant);
	Core_Serializer::SerializeFloat(pDoc, pNode, "mLinear", mData.mLinear);
	Core_Serializer::SerializeFloat(pDoc, pNode, "mQuadratic", mData.mQuadratic);
}

void Core::Core_CPointLight::OnDeserialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode)
{
	Core_CLight::OnDeserialize(pDoc, pNode);

	Core_Serializer::DeserializeFloat(pDoc, pNode, "mConstant", mData.mConstant);
	Core_Serializer::DeserializeFloat(pDoc, pNode, "mLinear", mData.mLinear);
	Core_Serializer::DeserializeFloat(pDoc, pNode, "mQuadratic", mData.mQuadratic);
}

void Core::Core_CPointLight::OnInspector(UI::UI_WidgetContainer& pRoot)
{
	Core_CLight::OnInspector(pRoot);

	Core_GUIDrawer::CreateTitle(pRoot, "Fall-off presets");
	auto& presetsRoot = pRoot.CreateWidget<UI::UI_Group>();

	auto& constantPreset = presetsRoot.CreateWidget<UI::UI_Button>("Constant");
	constantPreset.mClickedEvent += [this] { mData.mConstant = 1.f, mData.mLinear = mData.mQuadratic = 0.f; };
	constantPreset.mLineBreak = false;
	constantPreset.mIdleBackgroundColor = { 0.7f, 0.5f, 0.f, 1.f };

	auto& linearPreset = presetsRoot.CreateWidget<UI::UI_Button>("Linear");
	linearPreset.mClickedEvent += [this] { mData.mLinear = 1.f, mData.mConstant = mData.mQuadratic = 0.f; };
	linearPreset.mLineBreak = false;
	linearPreset.mIdleBackgroundColor = { 0.7f, 0.5f, 0.f, 1.f };

	auto& quadraticPreset = presetsRoot.CreateWidget<UI::UI_Button>("Quadratic");
	quadraticPreset.mClickedEvent += [this] { mData.mQuadratic = 1.f, mData.mConstant = mData.mLinear = 0.f; };
	quadraticPreset.mIdleBackgroundColor = { 0.7f, 0.5f, 0.f, 1.f };

	Core_GUIDrawer::DrawScalar<float>(pRoot, "Constant", mData.mConstant, 0.005f, 0.f);
	Core_GUIDrawer::DrawScalar<float>(pRoot, "Linear", mData.mLinear, 0.005f, 0.f);
	Core_GUIDrawer::DrawScalar<float>(pRoot, "Quadratic", mData.mQuadratic, 0.005f, 0.f);
}

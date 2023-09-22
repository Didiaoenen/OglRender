#include <OglUI/UI_Text.h>
#include <OglUI/UI_DragFloat.h>
#include <OglUI/UI_ColorEdit.h>
#include <OglUI/UI_Button.h>
#include <OglUI/UI_Group.h>

#include "Core_Actor.h"
#include "Core_CSpotLight.h"

Core::Core_CSpotLight::Core_CSpotLight(Core_Actor& pOwner) :
	Core_CLight(pOwner)
{
	mData.mType = static_cast<float>(Render::Render_Light::ELightType::SPOT);
}

std::string Core::Core_CSpotLight::GetName()
{
	return "Spot Light";
}

float Core::Core_CSpotLight::GetConstant() const
{
	return mData.mConstant;
}

float Core::Core_CSpotLight::GetLinear() const
{
	return mData.mLinear;
}

float Core::Core_CSpotLight::GetQuadratic() const
{
	return mData.mQuadratic;
}

float Core::Core_CSpotLight::GetCutoff() const
{
	return mData.mCutoff;
}

float Core::Core_CSpotLight::GetOuterCutoff() const
{
	return mData.mOuterCutoff;
}

void Core::Core_CSpotLight::SetConstant(float pConstant)
{
	mData.mConstant = pConstant;
}

void Core::Core_CSpotLight::SetLinear(float pLinear)
{
	mData.mLinear = pLinear;
}

void Core::Core_CSpotLight::SetQuadratic(float pQuadratic)
{
	mData.mQuadratic = pQuadratic;
}

void Core::Core_CSpotLight::SetCutoff(float pCutoff)
{
	mData.mCutoff = pCutoff;
}

void Core::Core_CSpotLight::SetOuterCutoff(float pOuterCutoff)
{
	mData.mOuterCutoff = pOuterCutoff;
}

void Core::Core_CSpotLight::OnSerialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode)
{
	Core_CLight::OnSerialize(pDoc, pNode);

	Core_Serializer::SerializeFloat(pDoc, pNode, "mConstant", mData.mConstant);
	Core_Serializer::SerializeFloat(pDoc, pNode, "mLinear", mData.mLinear);
	Core_Serializer::SerializeFloat(pDoc, pNode, "mQuadratic", mData.mQuadratic);
	Core_Serializer::SerializeFloat(pDoc, pNode, "mCutoff", mData.mCutoff);
	Core_Serializer::SerializeFloat(pDoc, pNode, "mOuterCutoff", mData.mOuterCutoff);
}

void Core::Core_CSpotLight::OnDeserialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode)
{
	Core_CLight::OnDeserialize(pDoc, pNode);

	Core_Serializer::DeserializeFloat(pDoc, pNode, "mConstant", mData.mConstant);
	Core_Serializer::DeserializeFloat(pDoc, pNode, "mLinear", mData.mLinear);
	Core_Serializer::DeserializeFloat(pDoc, pNode, "mQuadratic", mData.mQuadratic);
	Core_Serializer::DeserializeFloat(pDoc, pNode, "mCutoff", mData.mCutoff);
	Core_Serializer::DeserializeFloat(pDoc, pNode, "mOuterCutoff", mData.mOuterCutoff);
}

void Core::Core_CSpotLight::OnInspector(UI::UI_WidgetContainer& pRoot)
{
	Core_CLight::OnInspector(pRoot);

	Core_GUIDrawer::DrawScalar<float>(pRoot, "Cut-Off", mData.mCutoff, 0.1f, 0.f, 180.f);
	Core_GUIDrawer::DrawScalar<float>(pRoot, "Outer Cut-Off", mData.mOuterCutoff, 0.1f, 0.f, 180.f);

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

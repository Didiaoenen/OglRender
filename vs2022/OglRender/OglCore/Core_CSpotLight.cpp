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
	mData.type = static_cast<float>(Render::Render_Light::Type::SPOT);
}

std::string Core::Core_CSpotLight::GetName()
{
	return "Spot Light";
}

float Core::Core_CSpotLight::GetConstant() const
{
	return mData.constant;
}

float Core::Core_CSpotLight::GetLinear() const
{
	return mData.linear;
}

float Core::Core_CSpotLight::GetQuadratic() const
{
	return mData.quadratic;
}

float Core::Core_CSpotLight::GetCutoff() const
{
	return mData.cutoff;
}

float Core::Core_CSpotLight::GetOuterCutoff() const
{
	return mData.outerCutoff;
}

void Core::Core_CSpotLight::SetConstant(float pConstant)
{
	mData.constant = pConstant;
}

void Core::Core_CSpotLight::SetLinear(float pLinear)
{
	mData.linear = pLinear;
}

void Core::Core_CSpotLight::SetQuadratic(float pQuadratic)
{
	mData.quadratic = pQuadratic;
}

void Core::Core_CSpotLight::SetCutoff(float pCutoff)
{
	mData.cutoff = pCutoff;
}

void Core::Core_CSpotLight::SetOuterCutoff(float pOuterCutoff)
{
	mData.outerCutoff = pOuterCutoff;
}

void Core::Core_CSpotLight::OnSerialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode)
{
	Core_CLight::OnSerialize(pDoc, pNode);

	Core_Serializer::SerializeFloat(pDoc, pNode, "constant", mData.constant);
	Core_Serializer::SerializeFloat(pDoc, pNode, "linear", mData.linear);
	Core_Serializer::SerializeFloat(pDoc, pNode, "quadratic", mData.quadratic);
	Core_Serializer::SerializeFloat(pDoc, pNode, "cutoff", mData.cutoff);
	Core_Serializer::SerializeFloat(pDoc, pNode, "outercutoff", mData.outerCutoff);
}

void Core::Core_CSpotLight::OnDeserialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode)
{
	Core_CLight::OnDeserialize(pDoc, pNode);

	Core_Serializer::DeserializeFloat(pDoc, pNode, "constant", mData.constant);
	Core_Serializer::DeserializeFloat(pDoc, pNode, "linear", mData.linear);
	Core_Serializer::DeserializeFloat(pDoc, pNode, "quadratic", mData.quadratic);
	Core_Serializer::DeserializeFloat(pDoc, pNode, "cutoff", mData.cutoff);
	Core_Serializer::DeserializeFloat(pDoc, pNode, "outercutoff", mData.outerCutoff);
}

void Core::Core_CSpotLight::OnInspector(UI::UI_WidgetContainer& pRoot)
{
	Core_CLight::OnInspector(pRoot);

	Core_GUIDrawer::DrawScalar<float>(pRoot, "Cut-Off", mData.cutoff, 0.1f, 0.f, 180.f);
	Core_GUIDrawer::DrawScalar<float>(pRoot, "Outer Cut-Off", mData.outerCutoff, 0.1f, 0.f, 180.f);

	Core_GUIDrawer::CreateTitle(pRoot, "Fall-off presets");
	auto& presetsRoot = pRoot.CreateWidget<UI::UI_Group>();

	auto& constantPreset = presetsRoot.CreateWidget<UI::UI_Button>("Constant");
	constantPreset.mClickedEvent += [this] { mData.constant = 1.f, mData.linear = mData.quadratic = 0.f; };
	constantPreset.mLineBreak = false;
	constantPreset.mIdleBackgroundColor = { 0.7f, 0.5f, 0.f, 1.f };

	auto& linearPreset = presetsRoot.CreateWidget<UI::UI_Button>("Linear");
	linearPreset.mClickedEvent += [this] { mData.linear = 1.f, mData.constant = mData.quadratic = 0.f; };
	linearPreset.mLineBreak = false;
	linearPreset.mIdleBackgroundColor = { 0.7f, 0.5f, 0.f, 1.f };

	auto& quadraticPreset = presetsRoot.CreateWidget<UI::UI_Button>("Quadratic");
	quadraticPreset.mClickedEvent += [this] { mData.quadratic = 1.f, mData.constant = mData.linear = 0.f; };
	quadraticPreset.mIdleBackgroundColor = { 0.7f, 0.5f, 0.f, 1.f };

	Core_GUIDrawer::DrawScalar<float>(pRoot, "Constant", mData.constant, 0.005f, 0.f);
	Core_GUIDrawer::DrawScalar<float>(pRoot, "Linear", mData.linear, 0.005f, 0.f);
	Core_GUIDrawer::DrawScalar<float>(pRoot, "Quadratic", mData.quadratic, 0.005f, 0.f);
}

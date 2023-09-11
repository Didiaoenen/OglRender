#include <OglUI/UI_Text.h>
#include <OglUI/UI_DragFloat.h>
#include <OglUI/UI_ColorEdit.h>
#include <OglUI/UI_Button.h>
#include <OglUI/UI_Group.h>

#include "Core_Actor.h"
#include "Core_CSpotLight.h"

Core::Core_CSpotLight::Core_CSpotLight(Core_Actor& p_owner) :
	Core_CLight(p_owner)
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

void Core::Core_CSpotLight::SetConstant(float p_constant)
{
	mData.constant = p_constant;
}

void Core::Core_CSpotLight::SetLinear(float p_linear)
{
	mData.linear = p_linear;
}

void Core::Core_CSpotLight::SetQuadratic(float p_quadratic)
{
	mData.quadratic = p_quadratic;
}

void Core::Core_CSpotLight::SetCutoff(float p_cutoff)
{
	mData.cutoff = p_cutoff;
}

void Core::Core_CSpotLight::SetOuterCutoff(float p_outerCutoff)
{
	mData.outerCutoff = p_outerCutoff;
}

void Core::Core_CSpotLight::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	Core_CLight::OnSerialize(p_doc, p_node);

	Core_Serializer::SerializeFloat(p_doc, p_node, "constant", mData.constant);
	Core_Serializer::SerializeFloat(p_doc, p_node, "linear", mData.linear);
	Core_Serializer::SerializeFloat(p_doc, p_node, "quadratic", mData.quadratic);
	Core_Serializer::SerializeFloat(p_doc, p_node, "cutoff", mData.cutoff);
	Core_Serializer::SerializeFloat(p_doc, p_node, "outercutoff", mData.outerCutoff);
}

void Core::Core_CSpotLight::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	Core_CLight::OnDeserialize(p_doc, p_node);

	Core_Serializer::DeserializeFloat(p_doc, p_node, "constant", mData.constant);
	Core_Serializer::DeserializeFloat(p_doc, p_node, "linear", mData.linear);
	Core_Serializer::DeserializeFloat(p_doc, p_node, "quadratic", mData.quadratic);
	Core_Serializer::DeserializeFloat(p_doc, p_node, "cutoff", mData.cutoff);
	Core_Serializer::DeserializeFloat(p_doc, p_node, "outercutoff", mData.outerCutoff);
}

void Core::Core_CSpotLight::OnInspector(UI::UI_WidgetContainer& p_root)
{
	Core_CLight::OnInspector(p_root);

	Core_GUIDrawer::DrawScalar<float>(p_root, "Cut-Off", mData.cutoff, 0.1f, 0.f, 180.f);
	Core_GUIDrawer::DrawScalar<float>(p_root, "Outer Cut-Off", mData.outerCutoff, 0.1f, 0.f, 180.f);

	Core_GUIDrawer::CreateTitle(p_root, "Fall-off presets");
	auto& presetsRoot = p_root.CreateWidget<UI::UI_Group>();

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

	Core_GUIDrawer::DrawScalar<float>(p_root, "Constant", mData.constant, 0.005f, 0.f);
	Core_GUIDrawer::DrawScalar<float>(p_root, "Linear", mData.linear, 0.005f, 0.f);
	Core_GUIDrawer::DrawScalar<float>(p_root, "Quadratic", mData.quadratic, 0.005f, 0.f);
}

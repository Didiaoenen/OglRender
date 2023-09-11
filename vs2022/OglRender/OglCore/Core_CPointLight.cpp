#include <OglUI/UI_Text.h>
#include <OglUI/UI_DragFloat.h>
#include <OglUI/UI_ColorEdit.h>
#include <OglUI/UI_Button.h>
#include <OglUI/UI_Group.h>

#include "Core_Actor.h"
#include "Core_CPointLight.h"

Core::Core_CPointLight::Core_CPointLight(Core_Actor& p_owner) :
	Core_CLight(p_owner)
{
	mData.type = static_cast<float>(Render::Render_Light::Type::POINT);
}

std::string Core::Core_CPointLight::GetName()
{
	return "Point Light";
}

float Core::Core_CPointLight::GetConstant() const
{
	return mData.constant;
}

float Core::Core_CPointLight::GetLinear() const
{
	return mData.linear;
}

float Core::Core_CPointLight::GetQuadratic() const
{
	return mData.quadratic;
}

void Core::Core_CPointLight::SetConstant(float p_constant)
{
	mData.constant = p_constant;
}

void Core::Core_CPointLight::SetLinear(float p_linear)
{
	mData.linear = p_linear;
}

void Core::Core_CPointLight::SetQuadratic(float p_quadratic)
{
	mData.quadratic = p_quadratic;
}

void Core::Core_CPointLight::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	Core_CLight::OnSerialize(p_doc, p_node);

	Core_Serializer::SerializeFloat(p_doc, p_node, "constant", mData.constant);
	Core_Serializer::SerializeFloat(p_doc, p_node, "linear", mData.linear);
	Core_Serializer::SerializeFloat(p_doc, p_node, "quadratic", mData.quadratic);
}

void Core::Core_CPointLight::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	Core_CLight::OnDeserialize(p_doc, p_node);

	Core_Serializer::DeserializeFloat(p_doc, p_node, "constant", mData.constant);
	Core_Serializer::DeserializeFloat(p_doc, p_node, "linear", mData.linear);
	Core_Serializer::DeserializeFloat(p_doc, p_node, "quadratic", mData.quadratic);
}

void Core::Core_CPointLight::OnInspector(UI::UI_WidgetContainer& p_root)
{
	Core_CLight::OnInspector(p_root);

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

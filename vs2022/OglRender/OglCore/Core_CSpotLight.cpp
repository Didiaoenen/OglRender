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
	m_data.type = static_cast<float>(Render::Render_Light::Type::SPOT);
}

std::string Core::Core_CSpotLight::GetName()
{
	return "Spot Light";
}

float Core::Core_CSpotLight::GetConstant() const
{
	return m_data.constant;
}

float Core::Core_CSpotLight::GetLinear() const
{
	return m_data.linear;
}

float Core::Core_CSpotLight::GetQuadratic() const
{
	return m_data.quadratic;
}

float Core::Core_CSpotLight::GetCutoff() const
{
	return m_data.cutoff;
}

float Core::Core_CSpotLight::GetOuterCutoff() const
{
	return m_data.outerCutoff;
}

void Core::Core_CSpotLight::SetConstant(float p_constant)
{
	m_data.constant = p_constant;
}

void Core::Core_CSpotLight::SetLinear(float p_linear)
{
	m_data.linear = p_linear;
}

void Core::Core_CSpotLight::SetQuadratic(float p_quadratic)
{
	m_data.quadratic = p_quadratic;
}

void Core::Core_CSpotLight::SetCutoff(float p_cutoff)
{
	m_data.cutoff = p_cutoff;
}

void Core::Core_CSpotLight::SetOuterCutoff(float p_outerCutoff)
{
	m_data.outerCutoff = p_outerCutoff;
}

void Core::Core_CSpotLight::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	Core_CLight::OnSerialize(p_doc, p_node);

	Core_Serializer::SerializeFloat(p_doc, p_node, "constant", m_data.constant);
	Core_Serializer::SerializeFloat(p_doc, p_node, "linear", m_data.linear);
	Core_Serializer::SerializeFloat(p_doc, p_node, "quadratic", m_data.quadratic);
	Core_Serializer::SerializeFloat(p_doc, p_node, "cutoff", m_data.cutoff);
	Core_Serializer::SerializeFloat(p_doc, p_node, "outercutoff", m_data.outerCutoff);
}

void Core::Core_CSpotLight::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	Core_CLight::OnDeserialize(p_doc, p_node);

	Core_Serializer::DeserializeFloat(p_doc, p_node, "constant", m_data.constant);
	Core_Serializer::DeserializeFloat(p_doc, p_node, "linear", m_data.linear);
	Core_Serializer::DeserializeFloat(p_doc, p_node, "quadratic", m_data.quadratic);
	Core_Serializer::DeserializeFloat(p_doc, p_node, "cutoff", m_data.cutoff);
	Core_Serializer::DeserializeFloat(p_doc, p_node, "outercutoff", m_data.outerCutoff);
}

void Core::Core_CSpotLight::OnInspector(UI::UI_WidgetContainer& p_root)
{
	Core_CLight::OnInspector(p_root);

	Core_GUIDrawer::DrawScalar<float>(p_root, "Cut-Off", m_data.cutoff, 0.1f, 0.f, 180.f);
	Core_GUIDrawer::DrawScalar<float>(p_root, "Outer Cut-Off", m_data.outerCutoff, 0.1f, 0.f, 180.f);

	Core_GUIDrawer::CreateTitle(p_root, "Fall-off presets");
	auto& presetsRoot = p_root.CreateWidget<UI::UI_Group>();

	auto& constantPreset = presetsRoot.CreateWidget<UI::UI_Button>("Constant");
	constantPreset.mClickedEvent += [this] { m_data.constant = 1.f, m_data.linear = m_data.quadratic = 0.f; };
	constantPreset.mLineBreak = false;
	constantPreset.mIdleBackgroundColor = { 0.7f, 0.5f, 0.f, 1.f };

	auto& linearPreset = presetsRoot.CreateWidget<UI::UI_Button>("Linear");
	linearPreset.mClickedEvent += [this] { m_data.linear = 1.f, m_data.constant = m_data.quadratic = 0.f; };
	linearPreset.mLineBreak = false;
	linearPreset.mIdleBackgroundColor = { 0.7f, 0.5f, 0.f, 1.f };

	auto& quadraticPreset = presetsRoot.CreateWidget<UI::UI_Button>("Quadratic");
	quadraticPreset.mClickedEvent += [this] { m_data.quadratic = 1.f, m_data.constant = m_data.linear = 0.f; };
	quadraticPreset.mIdleBackgroundColor = { 0.7f, 0.5f, 0.f, 1.f };

	Core_GUIDrawer::DrawScalar<float>(p_root, "Constant", m_data.constant, 0.005f, 0.f);
	Core_GUIDrawer::DrawScalar<float>(p_root, "Linear", m_data.linear, 0.005f, 0.f);
	Core_GUIDrawer::DrawScalar<float>(p_root, "Quadratic", m_data.quadratic, 0.005f, 0.f);
}

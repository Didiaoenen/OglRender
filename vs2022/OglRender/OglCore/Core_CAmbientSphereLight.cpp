#include <OglUI/UI_Text.h>
#include <OglUI/UI_DragFloat.h>
#include <OglUI/UI_Bullet.h>
#include <OglUI/UI_ColorEdit.h>

#include "Core_Actor.h"
#include "Core_CAmbientSphereLight.h"

Core::Core_CAmbientSphereLight::Core_CAmbientSphereLight(Core_Actor& p_owner) :
	Core_CLight(p_owner)
{
	mData.intensity = 0.1f;
	mData.constant = 1.0f;

	mData.type = static_cast<float>(Render::Render_Light::Type::AMBIENT_SPHERE);
}

std::string Core::Core_CAmbientSphereLight::GetName()
{
	return "Ambient Sphere Light";
}

float Core::Core_CAmbientSphereLight::GetRadius() const
{
	return mData.quadratic;
}

void Core::Core_CAmbientSphereLight::SetRadius(float p_radius)
{
	mData.constant = p_radius;
}

void Core::Core_CAmbientSphereLight::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	Core_CLight::OnSerialize(p_doc, p_node);

	Core_Serializer::SerializeFloat(p_doc, p_node, "radius", mData.constant);
}

void Core::Core_CAmbientSphereLight::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	Core_CLight::OnDeserialize(p_doc, p_node);

	Core_Serializer::DeserializeFloat(p_doc, p_node, "radius", mData.constant);
}

void Core::Core_CAmbientSphereLight::OnInspector(UI::UI_WidgetContainer& p_root)
{
	Core_CLight::OnInspector(p_root);

	Core_GUIDrawer::DrawScalar<float>(p_root, "Radius", mData.constant, 0.1f, 0.f);
}

#include <OglUI/UI_Text.h>
#include <OglUI/UI_DragFloat.h>
#include <OglUI/UI_Bullet.h>
#include <OglUI/UI_ColorEdit.h>

#include "Core_Actor.h"
#include "Core_CAmbientSphereLight.h"

Core::Core_CAmbientSphereLight::Core_CAmbientSphereLight(Core_Actor& pOwner) :
	Core_CLight(pOwner)
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

void Core::Core_CAmbientSphereLight::SetRadius(float pRadius)
{
	mData.constant = pRadius;
}

void Core::Core_CAmbientSphereLight::OnSerialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode)
{
	Core_CLight::OnSerialize(pDoc, pNode);

	Core_Serializer::SerializeFloat(pDoc, pNode, "radius", mData.constant);
}

void Core::Core_CAmbientSphereLight::OnDeserialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode)
{
	Core_CLight::OnDeserialize(pDoc, pNode);

	Core_Serializer::DeserializeFloat(pDoc, pNode, "radius", mData.constant);
}

void Core::Core_CAmbientSphereLight::OnInspector(UI::UI_WidgetContainer& pRoot)
{
	Core_CLight::OnInspector(pRoot);

	Core_GUIDrawer::DrawScalar<float>(pRoot, "Radius", mData.constant, 0.1f, 0.f);
}

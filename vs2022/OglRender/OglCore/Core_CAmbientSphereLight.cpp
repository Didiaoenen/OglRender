#include <OglUI/UI_Text.h>
#include <OglUI/UI_DragFloat.h>
#include <OglUI/UI_Bullet.h>
#include <OglUI/UI_ColorEdit.h>

#include "Core_Actor.h"
#include "Core_CAmbientSphereLight.h"

Core::Core_CAmbientSphereLight::Core_CAmbientSphereLight(Core_Actor& pOwner) :
	Core_CLight(pOwner)
{
	mData.mIntensity = 0.1f;
	mData.mConstant = 1.0f;

	mData.mType = static_cast<float>(Render::Render_Light::ELightType::AMBIENT_SPHERE);
}

std::string Core::Core_CAmbientSphereLight::GetName()
{
	return "Ambient Sphere Light";
}

float Core::Core_CAmbientSphereLight::GetRadius() const
{
	return mData.mQuadratic;
}

void Core::Core_CAmbientSphereLight::SetRadius(float pRadius)
{
	mData.mConstant = pRadius;
}

void Core::Core_CAmbientSphereLight::OnSerialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode)
{
	Core_CLight::OnSerialize(pDoc, pNode);

	Core_Serializer::SerializeFloat(pDoc, pNode, "radius", mData.mConstant);
}

void Core::Core_CAmbientSphereLight::OnDeserialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode)
{
	Core_CLight::OnDeserialize(pDoc, pNode);

	Core_Serializer::DeserializeFloat(pDoc, pNode, "radius", mData.mConstant);
}

void Core::Core_CAmbientSphereLight::OnInspector(UI::UI_WidgetContainer& pRoot)
{
	Core_CLight::OnInspector(pRoot);

	Core_GUIDrawer::DrawScalar<float>(pRoot, "Radius", mData.mConstant, 0.1f, 0.f);
}

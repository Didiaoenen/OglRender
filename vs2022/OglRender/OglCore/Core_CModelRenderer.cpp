#include <OglUI/UI_Text.h>
#include <OglUI/UI_Image.h>
#include <OglUI/UI_DDTarget.h>
#include <OglUI/UI_ComboBox.h>
#include <OglUI/UI_CheckBox.h>
#include <OglUI/UI_DragFloat.h>
#include <OglUI/UI_TextColored.h>

#include "Core_Actor.h"
#include "Core_ModelManager.h"
#include "Core_ShaderManager.h"
#include "Core_ServiceLocator.h"
#include "Core_TextureManager.h"
#include "Core_CModelRenderer.h"
#include "Core_CMaterialRenderer.h"

Core::Core_CModelRenderer::Core_CModelRenderer(Core_Actor& pOwner) :
	Core_AComponent(pOwner)
{
	mModelChangedEvent += [this]
		{
			if (auto materialRenderer = mOwner.GetComponent<Core_CMaterialRenderer>())
				materialRenderer->UpdateMaterialList();
		};
}

std::string Core::Core_CModelRenderer::GetName()
{
	return "Model Renderer";
}

void Core::Core_CModelRenderer::SetModel(Render::Render_Model* pModel)
{
	mModel = pModel;
	mModelChangedEvent.Invoke();
}

Render::Render_Model* Core::Core_CModelRenderer::GetModel() const
{
	return mModel;
}

void Core::Core_CModelRenderer::SetFrustumBehaviour(EFrustumBehaviour pBoundingMode)
{
	mFrustumBehaviour = pBoundingMode;
}

Core::Core_CModelRenderer::EFrustumBehaviour Core::Core_CModelRenderer::GetFrustumBehaviour() const
{
	return mFrustumBehaviour;
}

const Render::Render_BoundingSphere& Core::Core_CModelRenderer::GetCustomBoundingSphere() const
{
	return mCustomBoundingSphere;
}

void Core::Core_CModelRenderer::SetCustomBoundingSphere(const Render::Render_BoundingSphere& pBoundingSphere)
{
	mCustomBoundingSphere = pBoundingSphere;
}

void Core::Core_CModelRenderer::OnSerialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode)
{
	Core_Serializer::SerializeModel(pDoc, pNode, "model", mModel);
	Core_Serializer::SerializeInt(pDoc, pNode, "frustum_behaviour", reinterpret_cast<int&>(mFrustumBehaviour));
	Core_Serializer::SerializeVec3(pDoc, pNode, "custom_bounding_sphere_position", mCustomBoundingSphere.position);
	Core_Serializer::SerializeFloat(pDoc, pNode, "custom_bounding_sphere_radius", mCustomBoundingSphere.radius);
}

void Core::Core_CModelRenderer::OnDeserialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode)
{
	Core_Serializer::DeserializeModel(pDoc, pNode, "model", mModel);
	Core_Serializer::DeserializeInt(pDoc, pNode, "frustum_behaviour", reinterpret_cast<int&>(mFrustumBehaviour));
	Core_Serializer::DeserializeVec3(pDoc, pNode, "custom_bounding_sphere_position", mCustomBoundingSphere.position);
	Core_Serializer::DeserializeFloat(pDoc, pNode, "custom_bounding_sphere_radius", mCustomBoundingSphere.radius);
}

void Core::Core_CModelRenderer::OnInspector(UI::UI_WidgetContainer& pRoot)
{
	Core_GUIDrawer::DrawMesh(pRoot, "Model", mModel, &mModelChangedEvent);

	Core_GUIDrawer::CreateTitle(pRoot, "Frustum Culling Behaviour");
	auto& boundingMode = pRoot.CreateWidget<UI::UI_ComboBox>(0);
	boundingMode.mChoices.emplace(0, "Disabled");
	boundingMode.mChoices.emplace(1, "Cull model");
	boundingMode.mChoices.emplace(2, "Cull model & sub-meshes");
	boundingMode.mChoices.emplace(3, "Cull custom bounding sphere");
	auto& boundingModeDispatcher = boundingMode.AddPlugin<UI::UI_DataDispatcher<int>>();
	boundingModeDispatcher.RegisterReference(reinterpret_cast<int&>(mFrustumBehaviour));

	auto& centerLabel = pRoot.CreateWidget<UI::UI_TextColored>("Bounding Sphere Center", Core_GUIDrawer::TitleColor);
	auto& centerWidget = pRoot.CreateWidget<UI::UI_DragMultipleScalars<float, 3>>(Core_GUIDrawer::GetDataType<float>(), Core_GUIDrawer::_MIN_FLOAT, Core_GUIDrawer::_MAX_FLOAT, 0.f, 0.05f, "", Core_GUIDrawer::GetFormat<float>());
	auto& centerDispatcher = centerWidget.AddPlugin<UI::UI_DataDispatcher<std::array<float, 3>>>();
	centerDispatcher.RegisterReference(reinterpret_cast<std::array<float, 3>&>(mCustomBoundingSphere.position));

	auto& radiusLabel = pRoot.CreateWidget<UI::UI_TextColored>("Bounding Sphere Radius", Core_GUIDrawer::TitleColor);
	auto& radiusWidget = pRoot.CreateWidget<UI::UI_DragFloat>(0.0f, Core_GUIDrawer::_MAX_FLOAT, 0.f, 0.1f);
	auto& radiusDispatcher = radiusWidget.AddPlugin<UI::UI_DataDispatcher<float>>();
	radiusDispatcher.RegisterReference(mCustomBoundingSphere.radius);

	boundingMode.mValueChangedEvent += [&](int p_choice)
		{
			centerLabel.mEnabled = centerWidget.mEnabled = radiusLabel.mEnabled = radiusWidget.mEnabled = p_choice == 3;
		};

	centerLabel.mEnabled = centerWidget.mEnabled = radiusLabel.mEnabled = radiusWidget.mEnabled = mFrustumBehaviour == EFrustumBehaviour::CULL_CUSTOM;
}

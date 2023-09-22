#include <OglUI/UI_InputInt.h>
#include <OglUI/UI_Group.h>
#include <OglUI/UI_Bullet.h>
#include <OglUI/UI_ButtonSmall.h>
#include <OglUI/UI_TextColored.h>
#include <OglUI/UI_DDTarget.h>

#include <OglTools/Tools_PathParser.h>

#include "Core_Actor.h"
#include "Core_CModelRenderer.h"
#include "Core_MaterialManager.h"
#include "Core_ServiceLocator.h"
#include "Core_CMaterialRenderer.h"

Core::Core_CMaterialRenderer::Core_CMaterialRenderer(Core_Actor& pOwner) :
	Core_AComponent(pOwner)
{
	mMaterials.fill(nullptr);

	for (uint8_t i = 0; i < MAX_MATERIAL_COUNT; ++i)
		mMaterialFields[i].fill(nullptr);

	UpdateMaterialList();
}

std::string Core::Core_CMaterialRenderer::GetName()
{
	return "Material Renderer";
}

void Core::Core_CMaterialRenderer::FillWithMaterial(Core_Material& pMaterial)
{
	for (uint8_t i = 0; i < mMaterials.size(); ++i)
		mMaterials[i] = &pMaterial;
}

void Core::Core_CMaterialRenderer::SetMaterialAtIndex(uint8_t pIndex, Core_Material& pMaterial)
{
	mMaterials[pIndex] = &pMaterial;
}

Core::Core_Material* Core::Core_CMaterialRenderer::GetMaterialAtIndex(uint8_t pIndex)
{
	return mMaterials.at(pIndex);
}

void Core::Core_CMaterialRenderer::RemoveMaterialAtIndex(uint8_t pIndex)
{
	if (pIndex < mMaterials.size())
	{
		mMaterials[pIndex] = nullptr;;
	}
}

void Core::Core_CMaterialRenderer::RemoveMaterialByInstance(Core_Material& pInstance)
{
	for (uint8_t i = 0; i < mMaterials.size(); ++i)
		if (mMaterials[i] == &pInstance)
			mMaterials[i] = nullptr;
}

void Core::Core_CMaterialRenderer::RemoveAllMaterials()
{
	for (uint8_t i = 0; i < mMaterials.size(); ++i)
		mMaterials[i] = nullptr;
}

void Core::Core_CMaterialRenderer::UpdateMaterialList()
{
	if (auto modelRenderer = mOwner.GetComponent<Core_CModelRenderer>(); modelRenderer && modelRenderer->GetModel())
	{
		uint8_t materialIndex = 0;

		for (const std::string& materialName : modelRenderer->GetModel()->GetMaterialNames())
		{
			mMaterialNames[materialIndex++] = materialName;
		}

		for (uint8_t i = materialIndex; i < MAX_MATERIAL_COUNT; ++i)
			mMaterialNames[i] = "";
	}

	for (uint8_t i = 0; i < mMaterialFields.size(); ++i)
	{
		if (mMaterialFields[i][0])
		{
			bool mEnabled = !mMaterialNames[i].empty();
			mMaterialFields[i][0]->mEnabled = mEnabled;
			mMaterialFields[i][1]->mEnabled = mEnabled;
			mMaterialFields[i][2]->mEnabled = mEnabled;
			reinterpret_cast<UI::UI_Text*>(mMaterialFields[i][0])->mContent = mMaterialNames[i];
		}
	}
}

void Core::Core_CMaterialRenderer::SetUserMatrixElement(uint32_t pRow, uint32_t pColumn, float pValue)
{
	if (pRow < 4 && pColumn < 4)
	{
	}
}

float Core::Core_CMaterialRenderer::GetUserMatrixElement(uint32_t pRow, uint32_t pColumn) const
{
	if (pRow < 4 && pColumn < 4)
	{
		return 0.0f;
	}
	else
	{
		return 0.0f;
	}
}

const glm::mat4& Core::Core_CMaterialRenderer::GetUserMatrix() const
{
	return mUserMatrix;
}

const Core::Core_CMaterialRenderer::MaterialList& Core::Core_CMaterialRenderer::GetMaterials() const
{
	return mMaterials;
}

void Core::Core_CMaterialRenderer::OnSerialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode)
{
	tinyxml2::XMLNode* materialsNode = pDoc.NewElement("materials");
	pNode->InsertEndChild(materialsNode);

	auto modelRenderer = mOwner.GetComponent<Core_CModelRenderer>();
	uint8_t elementsToSerialize = modelRenderer->GetModel() ? (uint8_t)std::min(modelRenderer->GetModel()->GetMaterialNames().size(), (size_t)MAX_MATERIAL_COUNT) : 0;

	for (uint8_t i = 0; i < elementsToSerialize; ++i)
	{
		Core_Serializer::SerializeMaterial(pDoc, materialsNode, "material", mMaterials[i]);
	}
}

void Core::Core_CMaterialRenderer::OnDeserialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode)
{
	tinyxml2::XMLNode* materialsRoot = pNode->FirstChildElement("materials");
	if (materialsRoot)
	{
		tinyxml2::XMLElement* currentMaterial = materialsRoot->FirstChildElement("material");

		uint8_t materialIndex = 0;

		while (currentMaterial)
		{
			if (auto material = Core_ServiceLocator::Get<Core::Core_MaterialManager>()[currentMaterial->GetText()])
			{
				mMaterials[materialIndex] = material;
			}

			currentMaterial = currentMaterial->NextSiblingElement("material");
			++materialIndex;
		}
	}

	UpdateMaterialList();
}

std::array<UI::UI_AWidget*, 3> CustomMaterialDrawer(UI::UI_WidgetContainer& pRoot, const std::string& pName, Core::Core_Material*& pData)
{
	std::array<UI::UI_AWidget*, 3> widgets;

	widgets[0] = &pRoot.CreateWidget<UI::UI_TextColored>(pName, Core::Core_GUIDrawer::TitleColor);

	std::string displayedText = (pData ? pData->path : std::string("Empty"));
	auto& rightSide = pRoot.CreateWidget<UI::UI_Group>();

	auto& widget = rightSide.CreateWidget<UI::UI_Text>(displayedText);

	widgets[1] = &widget;

	widget.AddPlugin<UI::UI_DDTarget<std::pair<std::string, UI::UI_Group*>>>("File").mDataReceivedEvent += [&widget, &pData](auto p_receivedData)
		{
			if (Tools::Tools_PathParser::GetFileType(p_receivedData.first) == Tools::Tools_PathParser::EFileType::MATERIAL)
			{
				if (auto resource = OVSERVICE(Core::Core_MaterialManager).GetResource(p_receivedData.first); resource)
				{
					pData = resource;
					widget.mContent = p_receivedData.first;
				}
			}
		};

	widget.mLineBreak = false;

	auto& resetButton = rightSide.CreateWidget<UI::UI_ButtonSmall>("Clear");
	resetButton.mIdleBackgroundColor = Core::Core_GUIDrawer::ClearButtonColor;
	resetButton.mClickedEvent += [&widget, &pData]
		{
			pData = nullptr;
			widget.mContent = "Empty";
		};

	widgets[2] = &resetButton;

	return widgets;
}

void Core::Core_CMaterialRenderer::OnInspector(UI::UI_WidgetContainer& pRoot)
{
	for (uint8_t i = 0; i < mMaterials.size(); ++i)
	{
		mMaterialFields[i] = CustomMaterialDrawer(pRoot, "Material", mMaterials[i]);
	}

	UpdateMaterialList();
}

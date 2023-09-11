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

Core::Core_CMaterialRenderer::Core_CMaterialRenderer(Core_Actor& p_owner) :
	Core_AComponent(p_owner)
{
	m_materials.fill(nullptr);

	for (uint8_t i = 0; i < MAX_MATERIAL_COUNT; ++i)
		m_materialFields[i].fill(nullptr);

	UpdateMaterialList();
}

std::string Core::Core_CMaterialRenderer::GetName()
{
	return "Material Renderer";
}

void Core::Core_CMaterialRenderer::FillWithMaterial(Core_Material& p_material)
{
	for (uint8_t i = 0; i < m_materials.size(); ++i)
		m_materials[i] = &p_material;
}

void Core::Core_CMaterialRenderer::SetMaterialAtIndex(uint8_t p_index, Core_Material& p_material)
{
	m_materials[p_index] = &p_material;
}

Core::Core_Material* Core::Core_CMaterialRenderer::GetMaterialAtIndex(uint8_t p_index)
{
	return m_materials.at(p_index);
}

void Core::Core_CMaterialRenderer::RemoveMaterialAtIndex(uint8_t p_index)
{
	if (p_index < m_materials.size())
	{
		m_materials[p_index] = nullptr;;
	}
}

void Core::Core_CMaterialRenderer::RemoveMaterialByInstance(Core_Material& p_instance)
{
	for (uint8_t i = 0; i < m_materials.size(); ++i)
		if (m_materials[i] == &p_instance)
			m_materials[i] = nullptr;
}

void Core::Core_CMaterialRenderer::RemoveAllMaterials()
{
	for (uint8_t i = 0; i < m_materials.size(); ++i)
		m_materials[i] = nullptr;
}

void Core::Core_CMaterialRenderer::UpdateMaterialList()
{
	if (auto modelRenderer = owner.GetComponent<Core_CModelRenderer>(); modelRenderer && modelRenderer->GetModel())
	{
		uint8_t materialIndex = 0;

		for (const std::string& materialName : modelRenderer->GetModel()->GetMaterialNames())
		{
			m_materialNames[materialIndex++] = materialName;
		}

		for (uint8_t i = materialIndex; i < MAX_MATERIAL_COUNT; ++i)
			m_materialNames[i] = "";
	}

	for (uint8_t i = 0; i < m_materialFields.size(); ++i)
	{
		if (m_materialFields[i][0])
		{
			bool mEnabled = !m_materialNames[i].empty();
			m_materialFields[i][0]->mEnabled = mEnabled;
			m_materialFields[i][1]->mEnabled = mEnabled;
			m_materialFields[i][2]->mEnabled = mEnabled;
			reinterpret_cast<UI::UI_Text*>(m_materialFields[i][0])->mContent = m_materialNames[i];
		}
	}
}

void Core::Core_CMaterialRenderer::SetUserMatrixElement(uint32_t p_row, uint32_t p_column, float pValue)
{

}

float Core::Core_CMaterialRenderer::GetUserMatrixElement(uint32_t p_row, uint32_t p_column) const
{
	return 0.0f;
}

const glm::mat4& Core::Core_CMaterialRenderer::GetUserMatrix() const
{
	return m_userMatrix;
}

const Core::Core_CMaterialRenderer::MaterialList& Core::Core_CMaterialRenderer::GetMaterials() const
{
	return m_materials;
}

void Core::Core_CMaterialRenderer::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	tinyxml2::XMLNode* materialsNode = p_doc.NewElement("materials");
	p_node->InsertEndChild(materialsNode);

	auto modelRenderer = owner.GetComponent<Core_CModelRenderer>();
	uint8_t elementsToSerialize = modelRenderer->GetModel() ? (uint8_t)std::min(modelRenderer->GetModel()->GetMaterialNames().size(), (size_t)MAX_MATERIAL_COUNT) : 0;

	for (uint8_t i = 0; i < elementsToSerialize; ++i)
	{
		Core_Serializer::SerializeMaterial(p_doc, materialsNode, "material", m_materials[i]);
	}
}

void Core::Core_CMaterialRenderer::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	tinyxml2::XMLNode* materialsRoot = p_node->FirstChildElement("materials");
	if (materialsRoot)
	{
		tinyxml2::XMLElement* currentMaterial = materialsRoot->FirstChildElement("material");

		uint8_t materialIndex = 0;

		while (currentMaterial)
		{
			if (auto material = Core_ServiceLocator::Get<Core::Core_MaterialManager>()[currentMaterial->GetText()])
				m_materials[materialIndex] = material;

			currentMaterial = currentMaterial->NextSiblingElement("material");
			++materialIndex;
		}
	}

	UpdateMaterialList();
}

std::array<UI::UI_AWidget*, 3> CustomMaterialDrawer(UI::UI_WidgetContainer& p_root, const std::string& p_name, Core::Core_Material*& p_data)
{
	std::array<UI::UI_AWidget*, 3> widgets;

	widgets[0] = &p_root.CreateWidget<UI::UI_TextColored>(p_name, Core::Core_GUIDrawer::TitleColor);

	std::string displayedText = (p_data ? p_data->path : std::string("Empty"));
	auto& rightSide = p_root.CreateWidget<UI::UI_Group>();

	auto& widget = rightSide.CreateWidget<UI::UI_Text>(displayedText);

	widgets[1] = &widget;

	widget.AddPlugin<UI::UI_DDTarget<std::pair<std::string, UI::UI_Group*>>>("File").mDataReceivedEvent += [&widget, &p_data](auto p_receivedData)
		{
			if (Tools::Tools_PathParser::GetFileType(p_receivedData.first) == Tools::Tools_PathParser::EFileType::MATERIAL)
			{
				if (auto resource = OVSERVICE(Core::Core_MaterialManager).GetResource(p_receivedData.first); resource)
				{
					p_data = resource;
					widget.mContent = p_receivedData.first;
				}
			}
		};

	widget.mLineBreak = false;

	auto& resetButton = rightSide.CreateWidget<UI::UI_ButtonSmall>("Clear");
	resetButton.mIdleBackgroundColor = Core::Core_GUIDrawer::ClearButtonColor;
	resetButton.mClickedEvent += [&widget, &p_data]
		{
			p_data = nullptr;
			widget.mContent = "Empty";
		};

	widgets[2] = &resetButton;

	return widgets;
}

void Core::Core_CMaterialRenderer::OnInspector(UI::UI_WidgetContainer& p_root)
{
	for (uint8_t i = 0; i < m_materials.size(); ++i)
		m_materialFields[i] = CustomMaterialDrawer(p_root, "Material", m_materials[i]);

	UpdateMaterialList();
}

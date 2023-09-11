#pragma once

#include <array>

#include <OglUI/UI_Columns.h>

#include <OglRenderer/Render_Mesh.h>

#include "Core_Material.h"
#include "Core_AComponent.h"

#define MAX_MATERIAL_COUNT 255

namespace Core
{
	class Core_Actor;

	class Core_CMaterialRenderer : public Core_AComponent
	{
	public:
		using MaterialList = std::array<Core_Material*, MAX_MATERIAL_COUNT>;
		using MaterialField = std::array<std::array<UI::UI_AWidget*, 3>, MAX_MATERIAL_COUNT>;

		Core_CMaterialRenderer(Core_Actor& p_owner);

		std::string GetName() override;

		void FillWithMaterial(Core_Material& p_material);

		void SetMaterialAtIndex(uint8_t p_index, Core_Material& p_material);

		Core_Material* GetMaterialAtIndex(uint8_t p_index);

		void RemoveMaterialAtIndex(uint8_t p_index);

		void RemoveMaterialByInstance(Core_Material& p_instance);

		void RemoveAllMaterials();

		void UpdateMaterialList();

		void SetUserMatrixElement(uint32_t p_row, uint32_t p_column, float p_value);

		float GetUserMatrixElement(uint32_t p_row, uint32_t p_column) const;

		const glm::mat4& GetUserMatrix() const;

		const MaterialList& GetMaterials() const;

		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnInspector(UI::UI_WidgetContainer& p_root) override;

	private:
		MaterialList m_materials;
		MaterialField m_materialFields;
		std::array<std::string, MAX_MATERIAL_COUNT> m_materialNames;
		glm::mat4 m_userMatrix;
	};
}


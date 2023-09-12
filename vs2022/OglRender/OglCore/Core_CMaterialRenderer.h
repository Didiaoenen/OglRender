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

		Core_CMaterialRenderer(Core_Actor& pOwner);

		std::string GetName() override;

		void FillWithMaterial(Core_Material& pMaterial);

		void SetMaterialAtIndex(uint8_t pIndex, Core_Material& pMaterial);

		Core_Material* GetMaterialAtIndex(uint8_t pIndex);

		void RemoveMaterialAtIndex(uint8_t pIndex);

		void RemoveMaterialByInstance(Core_Material& pInstance);

		void RemoveAllMaterials();

		void UpdateMaterialList();

		void SetUserMatrixElement(uint32_t pRow, uint32_t pColumn, float pValue);

		float GetUserMatrixElement(uint32_t pRow, uint32_t pColumn) const;

		const glm::mat4& GetUserMatrix() const;

		const MaterialList& GetMaterials() const;

		virtual void OnSerialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode) override;

		virtual void OnDeserialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode) override;

		virtual void OnInspector(UI::UI_WidgetContainer& pRoot) override;

	private:
		MaterialList mMaterials;
		MaterialField mMaterialFields;
		std::array<std::string, MAX_MATERIAL_COUNT> mMaterialNames;
		glm::mat4 mUserMatrix;
	};
}


#pragma once

#include <map>
#include <any>

#include <OglRenderer/Render_Shader.h>
#include <OglRenderer/Render_Texture.h>

#include "Core_ISerializable.h"

namespace Core
{
	class Core_Material : public Core_ISerializable
	{
	public:
		void SetShader(Render::Render_Shader* p_shader);

		void FillUniform();

		void Bind(Render::Render_Texture* p_emptyTexture);

		void UnBind();

		template<typename T> void Set(const std::string pKey, const T& pValue);

		template<typename T> const T& Get(const std::string pKey);

		Render::Render_Shader*& GetShader();

		bool HasShader() const;

		void SetBlendable(bool p_blendable);

		void SetBackfaceCulling(bool p_backfaceCulling);

		void SetFrontfaceCulling(bool p_frontfaceCulling);

		void SetDepthTest(bool p_depthTest);

		void SetDepthWriting(bool p_depthWriting);

		void SetColorWriting(bool p_colorWriting);

		void SetGPUInstances(int p_instances);

		bool IsBlendable() const;

		bool HasBackfaceCulling() const;

		bool HasFrontfaceCulling() const;

		bool HasDepthTest() const;

		bool HasDepthWriting() const;

		bool HasColorWriting() const;

		int GetGPUInstances() const;

		uint8_t GenerateStateMask() const;

		std::map<std::string, std::any>& GetUniformsData();

		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		const std::string path;

	private:
		Render::Render_Shader* m_shader = nullptr;
		std::map<std::string, std::any> m_uniformsData;

		bool m_blendable = false;
		bool m_backfaceCulling = true;
		bool m_frontfaceCulling = false;
		bool m_depthTest = true;
		bool m_depthWriting = true;
		bool m_colorWriting = true;
		int m_gpuInstances = 1;
	};
}
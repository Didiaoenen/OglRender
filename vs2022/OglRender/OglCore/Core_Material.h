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
		void SetShader(Render::Render_Shader* pShader);

		void FillUniform();

		void Bind(Render::Render_Texture* pEmptyTexture, const std::string& pProgramName);

		void UnBind(const std::string& pProgramName);

		template<typename T> void Set(const std::string pKey, const T& pValue);

		template<typename T> const T& Get(const std::string pKey);

		Render::Render_Shader*& GetShader();

		bool HasShader() const;

		void SetBlendable(bool pBlendable);

		void SetBackfaceCulling(bool pBackfaceCulling);

		void SetFrontfaceCulling(bool pFrontfaceCulling);

		void SetDepthTest(bool pDepthTest);

		void SetDepthWriting(bool pDepthWriting);

		void SetColorWriting(bool pColorWriting);

		void SetGPUInstances(int pInstances);

		bool IsBlendable() const;

		bool HasBackfaceCulling() const;

		bool HasFrontfaceCulling() const;

		bool HasDepthTest() const;

		bool HasDepthWriting() const;

		bool HasColorWriting() const;

		int GetGPUInstances() const;

		uint8_t GenerateStateMask() const;

		std::map<std::string, std::any>& GetUniformsData();

		virtual void OnSerialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode) override;

		virtual void OnDeserialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode) override;

		const std::string mPath{ "" };

	private:
		Render::Render_Shader* mShader{ nullptr };
		std::map<std::string, std::any> mUniformsData;

		bool mBlendable{ false };
		bool mBackfaceCulling{ true };
		bool mFrontfaceCulling{ false };
		bool mDepthTest{ true };
		bool mDepthWriting{ true };
		bool mColorWriting{ true };
		int mGpuInstances{ 1 };
	};
}

#include "Core_Material.inl"
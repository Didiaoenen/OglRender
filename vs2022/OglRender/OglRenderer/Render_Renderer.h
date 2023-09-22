#pragma once

#include <optional>

#include "Render_Model.h"
#include "Render_Driver.h"
#include "Render_Camera.h"
#include "Render_Shader.h"
#include "Render_ECullFace.h"
#include "Render_EOperation.h"
#include "Render_EPrimitiveMode.h"
#include "Render_EPixelDataType.h"
#include "Render_ECullingOptions.h"
#include "Render_EPixelDataFormat.h"
#include "Render_ERasterizationMode.h"
#include "Render_ERenderingCapability.h"
#include "Render_EComparaisonAlgorithm.h"

namespace Render
{
	class Render_Renderer
	{
	public:

		struct FrameInfo
		{
			uint64_t batchCount = 0;
			uint64_t instanceCount = 0;
			uint64_t polyCount = 0;
		};

		Render_Renderer(Render_Driver& pDriver);

		~Render_Renderer() = default;

		void SetClearColor(float pRed, float pGreen, float pBlue, float pAlpha = 1.0f);

		void Clear(bool pColorBuffer = true, bool pDepthBuffer = true, bool pStencilBuffer = true);

		void Clear(Render_Camera& pCamera, bool pColorBuffer = true, bool pDepthBuffer = true, bool pStencilBuffer = true);

		void SetRasterizationLinesWidth(float pWidth);

		void SetRasterizationMode(ERasterizationMode pRasterizationMode);

		void SetCapability(ERenderingCapability pCapability, bool pValue);

		bool GetCapability(ERenderingCapability pCapability) const;

		void SetStencilAlgorithm(EComparaisonAlgorithm pAlgorithm, int32_t pReference, uint32_t pMask);

		void SetDepthAlgorithm(EComparaisonAlgorithm pAlgorithm);

		void SetStencilMask(uint32_t pMask);

		void SetStencilOperations(EOperation pStencilFail = EOperation::KEEP, EOperation pDepthFail = EOperation::KEEP, EOperation pBothPass = EOperation::KEEP);

		void SetCullFace(ECullFace pCullFace);

		void SetDepthWriting(bool pEnable);

		void SetColorWriting(bool pEnableRed, bool pEnableGreen, bool pEnableBlue, bool pEnableAlpha);

		void SetColorWriting(bool pEnable);

		void SetViewPort(uint32_t pX, uint32_t pY, uint32_t pWidth, uint32_t pHeight);

		void ReadPixels(uint32_t pX, uint32_t pY, uint32_t pWidth, uint32_t pHeight, EPixelDataFormat pFormat, EPixelDataType pType, void* pData);

		bool GetBool(GLenum pParameter);

		bool GetBool(GLenum pParameter, uint32_t pIndex);

		int GetInt(GLenum pParameter);

		int GetInt(GLenum pParameter, uint32_t pIndex);

		float GetFloat(GLenum pParameter);

		float GetFloat(GLenum pParameter, uint32_t pIndex);

		double GetDouble(GLenum pParameter);

		double GetDouble(GLenum pParameter, uint32_t pIndex);

		int64_t GetInt64(GLenum pParameter);

		int64_t GetInt64(GLenum pParameter, uint32_t pIndex);

		std::string GetString(GLenum pParameter);

		std::string GetString(GLenum pParameter, uint32_t pIndex);

		void ClearFrameInfo();

		void Draw(Render_IMesh& pMesh, EPrimitiveMode pPrimitiveMode = EPrimitiveMode::TRIANGLES, uint32_t pInstances = 1);

		std::vector<std::reference_wrapper<Render_Mesh>> GetMeshesInFrustum
		(
			const Render_Model& pModel,
			const Render_BoundingSphere& pModelBoundingSphere,
			const Maths::Maths_FTransform& pModelTransform,
			const Render_Frustum& pFrustum,
			ECullingOptions pCullingOptions
		);

		uint8_t FetchGLState();

		void ApplyStateMask(uint8_t pMask);

		void SetState(uint8_t pState);

		const FrameInfo& GetFrameInfo() const;

	private:
		Render_Driver&	mDriver;
		FrameInfo		mFrameInfo;
		uint8_t			mState;
	};
}


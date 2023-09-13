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

		Render_Renderer(Render_Driver& p_driver);

		~Render_Renderer() = default;

		void SetClearColor(float p_red, float p_green, float p_blue, float p_alpha = 1.0f);

		void Clear(bool p_colorBuffer = true, bool p_depthBuffer = true, bool p_stencilBuffer = true);

		void Clear(Render_Camera& p_camera, bool p_colorBuffer = true, bool p_depthBuffer = true, bool p_stencilBuffer = true);

		void SetRasterizationLinesWidth(float p_width);

		void SetRasterizationMode(ERasterizationMode p_rasterizationMode);

		void SetCapability(ERenderingCapability p_capability, bool pValue);

		bool GetCapability(ERenderingCapability p_capability) const;

		void SetStencilAlgorithm(EComparaisonAlgorithm p_algorithm, int32_t pReference, uint32_t p_mask);

		void SetDepthAlgorithm(EComparaisonAlgorithm p_algorithm);

		void SetStencilMask(uint32_t p_mask);

		void SetStencilOperations(EOperation p_stencilFail = EOperation::KEEP, EOperation p_depthFail = EOperation::KEEP, EOperation p_bothPass = EOperation::KEEP);

		void SetCullFace(ECullFace p_cullFace);

		void SetDepthWriting(bool pEnable);

		void SetColorWriting(bool p_enableRed, bool p_enableGreen, bool p_enableBlue, bool p_enableAlpha);

		void SetColorWriting(bool pEnable);

		void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

		void ReadPixels(uint32_t x, uint32_t y, uint32_t width, uint32_t height, EPixelDataFormat format, EPixelDataType type, void* data);

		bool GetBool(GLenum p_parameter);

		bool GetBool(GLenum p_parameter, uint32_t pIndex);

		int GetInt(GLenum p_parameter);

		int GetInt(GLenum p_parameter, uint32_t pIndex);

		float GetFloat(GLenum p_parameter);

		float GetFloat(GLenum p_parameter, uint32_t pIndex);

		double GetDouble(GLenum p_parameter);

		double GetDouble(GLenum p_parameter, uint32_t pIndex);

		int64_t GetInt64(GLenum p_parameter);

		int64_t GetInt64(GLenum p_parameter, uint32_t pIndex);

		std::string GetString(GLenum p_parameter);

		std::string GetString(GLenum p_parameter, uint32_t pIndex);

		void ClearFrameInfo();

		void Draw(Render_IMesh& p_mesh, EPrimitiveMode p_primitiveMode = EPrimitiveMode::TRIANGLES, uint32_t p_instances = 1);

		std::vector<std::reference_wrapper<Render_Mesh>> GetMeshesInFrustum
		(
			const Render_Model& pModel,
			const Render_BoundingSphere& p_modelBoundingSphere,
			const Maths::Maths_FTransform& p_modelTransform,
			const Render_Frustum& p_frustum,
			ECullingOptions p_cullingOptions
		);

		uint8_t FetchGLState();

		void ApplyStateMask(uint8_t p_mask);

		void SetState(uint8_t p_state);

		const FrameInfo& GetFrameInfo() const;

	private:
		Render_Driver&	mDriver;
		FrameInfo		m_frameInfo;
		uint8_t			m_state;
	};
}


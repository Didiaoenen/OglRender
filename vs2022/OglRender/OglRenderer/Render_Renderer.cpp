#include <functional>

#include "Render_Renderer.h"

Render::Render_Renderer::Render_Renderer(Render_Driver& pDriver) :
	mDriver(pDriver), mState(0)
{
}

void Render::Render_Renderer::SetClearColor(float pRed, float pGreen, float pBlue, float pAlpha)
{
	glClearColor(pRed, pGreen, pBlue, pAlpha);
}

void Render::Render_Renderer::Clear(bool pColorBuffer, bool pDepthBuffer, bool pStencilBuffer)
{
	glClear
	(
		(pColorBuffer ? GL_COLOR_BUFFER_BIT : 0) |
		(pDepthBuffer ? GL_DEPTH_BUFFER_BIT : 0) |
		(pStencilBuffer ? GL_STENCIL_BUFFER_BIT : 0)
	);
}

void Render::Render_Renderer::Clear(Render_Camera& pCamera, bool pColorBuffer, bool pDepthBuffer, bool pStencilBuffer)
{
	GLfloat previousClearColor[4];
	glGetFloatv(GL_COLOR_CLEAR_VALUE, previousClearColor);

	const glm::vec3 cameraClearColor = pCamera.GetClearColor();
	SetClearColor(cameraClearColor.x, cameraClearColor.y, cameraClearColor.z, 1.0f);
	Clear(pColorBuffer, pDepthBuffer, pStencilBuffer);

	SetClearColor(previousClearColor[0], previousClearColor[1], previousClearColor[2], previousClearColor[3]);
}

void Render::Render_Renderer::SetRasterizationLinesWidth(float pWidth)
{
	glLineWidth(pWidth);
}

void Render::Render_Renderer::SetRasterizationMode(ERasterizationMode pRasterizationMode)
{
	glPolygonMode(GL_FRONT_AND_BACK, static_cast<GLenum>(pRasterizationMode));
}

void Render::Render_Renderer::SetCapability(ERenderingCapability pCapability, bool pValue)
{
	(pValue ? glEnable : glDisable)(static_cast<GLenum>(pCapability));
}

bool Render::Render_Renderer::GetCapability(ERenderingCapability pCapability) const
{
	return glIsEnabled(static_cast<GLenum>(pCapability));
}

void Render::Render_Renderer::SetStencilAlgorithm(EComparaisonAlgorithm pAlgorithm, int32_t pReference, uint32_t pMask)
{
	glStencilFunc(static_cast<GLenum>(pAlgorithm), pReference, pMask);
}

void Render::Render_Renderer::SetDepthAlgorithm(EComparaisonAlgorithm pAlgorithm)
{
	glDepthFunc(static_cast<GLenum>(pAlgorithm));
}

void Render::Render_Renderer::SetStencilMask(uint32_t pMask)
{
	glStencilMask(pMask);
}

void Render::Render_Renderer::SetStencilOperations(EOperation pStencilFail, EOperation pDepthFail, EOperation pBothPass)
{
	glStencilOp(static_cast<GLenum>(pStencilFail), static_cast<GLenum>(pDepthFail), static_cast<GLenum>(pBothPass));
}

void Render::Render_Renderer::SetCullFace(ECullFace pCullFace)
{
	glCullFace(static_cast<GLenum>(pCullFace));
}

void Render::Render_Renderer::SetDepthWriting(bool pEnable)
{
	glDepthMask(pEnable);
}

void Render::Render_Renderer::SetColorWriting(bool pEnableRed, bool pEnableGreen, bool pEnableBlue, bool pEnableAlpha)
{
	glColorMask(pEnableRed, pEnableGreen, pEnableBlue, pEnableAlpha);
}

void Render::Render_Renderer::SetColorWriting(bool pEnable)
{
	glColorMask(pEnable, pEnable, pEnable, pEnable);
}

void Render::Render_Renderer::SetViewPort(uint32_t pX, uint32_t pY, uint32_t pWidth, uint32_t pHeight)
{
	glViewport(pX, pY, pWidth, pHeight);
}

void Render::Render_Renderer::ReadPixels(uint32_t pX, uint32_t pY, uint32_t pWidth, uint32_t pHeight, EPixelDataFormat pFormat, EPixelDataType pType, void* pData)
{
	glReadPixels(pX, pY, pWidth, pHeight, static_cast<GLenum>(pFormat), static_cast<GLenum>(pType), pData);
}

bool Render::Render_Renderer::GetBool(GLenum pParameter)
{
	GLboolean result;
	glGetBooleanv(pParameter, &result);
	return static_cast<bool>(result);
}

bool Render::Render_Renderer::GetBool(GLenum pParameter, uint32_t pIndex)
{
	GLboolean result;
	glGetBooleani_v(pParameter, pIndex, &result);
	return static_cast<bool>(result);
}

int Render::Render_Renderer::GetInt(GLenum pParameter)
{
	GLint result;
	glGetIntegerv(pParameter, &result);
	return static_cast<int>(result);
}

int Render::Render_Renderer::GetInt(GLenum pParameter, uint32_t pIndex)
{
	GLint result;
	glGetIntegeri_v(pParameter, pIndex, &result);
	return static_cast<int>(result);
}

float Render::Render_Renderer::GetFloat(GLenum pParameter)
{
	GLfloat result;
	glGetFloatv(pParameter, &result);
	return static_cast<float>(result);
}

float Render::Render_Renderer::GetFloat(GLenum pParameter, uint32_t pIndex)
{
	GLfloat result;
	glGetFloati_v(pParameter, pIndex, &result);
	return static_cast<float>(result);
}

double Render::Render_Renderer::GetDouble(GLenum pParameter)
{
	GLdouble result;
	glGetDoublev(pParameter, &result);
	return static_cast<double>(result);
}

double Render::Render_Renderer::GetDouble(GLenum pParameter, uint32_t pIndex)
{
	GLdouble result;
	glGetDoublei_v(pParameter, pIndex, &result);
	return static_cast<double>(result);
}

int64_t Render::Render_Renderer::GetInt64(GLenum pParameter)
{
	GLint64 result;
	glGetInteger64v(pParameter, &result);
	return static_cast<int64_t>(result);
}

int64_t Render::Render_Renderer::GetInt64(GLenum pParameter, uint32_t pIndex)
{
	GLint64 result;
	glGetInteger64i_v(pParameter, pIndex, &result);
	return static_cast<int64_t>(result);
}

std::string Render::Render_Renderer::GetString(GLenum pParameter)
{
	const GLubyte* result = glGetString(pParameter);
	return result ? reinterpret_cast<const char*>(result) : std::string();
}

std::string Render::Render_Renderer::GetString(GLenum pParameter, uint32_t pIndex)
{
	const GLubyte* result = glGetStringi(pParameter, pIndex);
	return result ? reinterpret_cast<const char*>(result) : std::string();
}

void Render::Render_Renderer::ClearFrameInfo()
{
	mFrameInfo.batchCount = 0;
	mFrameInfo.instanceCount = 0;
	mFrameInfo.polyCount = 0;
}

void Render::Render_Renderer::Draw(Render_IMesh& pMesh, EPrimitiveMode pPrimitiveMode, uint32_t pInstances)
{
	if (pInstances > 0)
	{
		++mFrameInfo.batchCount;
		mFrameInfo.instanceCount += pInstances;
		mFrameInfo.polyCount += (pMesh.GetIndexCount() / 3) * pInstances;

		pMesh.Bind();

		if (pMesh.GetIndexCount() > 0)
		{
			/* With EBO */
			if (pInstances == 1)
			{
				glDrawElements(static_cast<GLenum>(pPrimitiveMode), pMesh.GetIndexCount(), GL_UNSIGNED_INT, nullptr);
			}
			else
			{
				glDrawElementsInstanced(static_cast<GLenum>(pPrimitiveMode), pMesh.GetIndexCount(), GL_UNSIGNED_INT, nullptr, pInstances);
			}
		}
		else
		{
			/* Without EBO */
			if (pInstances == 1)
			{
				glDrawArrays(static_cast<GLenum>(pPrimitiveMode), 0, pMesh.GetVertexCount());
			}
			else
			{
				glDrawArraysInstanced(static_cast<GLenum>(pPrimitiveMode), 0, pMesh.GetVertexCount(), pInstances);
			}
		}

		pMesh.Unbind();
	}
}

std::vector<std::reference_wrapper<Render::Render_Mesh>> Render::Render_Renderer::GetMeshesInFrustum(const Render_Model& pModel, const Render_BoundingSphere& pModelBoundingSphere, const Maths::Maths_FTransform& pModelTransform, const Render_Frustum& pFrustum, ECullingOptions pCullingOptions)
{
	const bool frustumPerModel = IsFlagSet(ECullingOptions::FRUSTUM_PER_MODEL, pCullingOptions);

	if (!frustumPerModel || pFrustum.BoundingSphereInFrustum(pModelBoundingSphere, pModelTransform))
	{
		std::vector<std::reference_wrapper<Render_Mesh>> result;

		const bool frustumPerMesh = IsFlagSet(ECullingOptions::FRUSTUM_PER_MESH, pCullingOptions);

		const auto& meshes = pModel.GetMeshes();

		for (auto mesh : meshes)
		{
			if (meshes.size() == 1 || !frustumPerMesh || pFrustum.BoundingSphereInFrustum(mesh->GetBoundingSphere(), pModelTransform))
			{
				result.emplace_back(*mesh);
			}
		}

		return result;
	}

	return {};
}

uint8_t Render::Render_Renderer::FetchGLState()
{
	uint8_t result = 0;

	GLboolean cMask[4];
	glGetBooleanv(GL_COLOR_WRITEMASK, cMask);

	if (GetBool(GL_DEPTH_WRITEMASK))						result |= 0b0000'0001;
	if (cMask[0])											result |= 0b0000'0010;
	if (GetCapability(ERenderingCapability::BLEND))			result |= 0b0000'0100;
	if (GetCapability(ERenderingCapability::CULL_FACE))		result |= 0b0000'1000;
	if (GetCapability(ERenderingCapability::DEPTH_TEST))	result |= 0b0001'0000;

	switch (static_cast<ECullFace>(GetInt(GL_CULL_FACE)))
	{
	case ECullFace::BACK:				result |= 0b0010'0000; break;
	case ECullFace::FRONT:				result |= 0b0100'0000; break;
	case ECullFace::FRONT_AND_BACK:		result |= 0b0110'0000; break;
	}

	return result;
}

void Render::Render_Renderer::ApplyStateMask(uint8_t pMask)
{
	if (pMask != mState)
	{
		if ((pMask & 0x01) != (mState & 0x01))	SetDepthWriting(pMask & 0x01);
		if ((pMask & 0x02) != (mState & 0x02))	SetColorWriting(pMask & 0x02);
		if ((pMask & 0x04) != (mState & 0x04))	SetCapability(ERenderingCapability::BLEND, pMask & 0x04);
		if ((pMask & 0x08) != (mState & 0x08))	SetCapability(ERenderingCapability::CULL_FACE, pMask & 0x8);
		if ((pMask & 0x10) != (mState & 0x10))	SetCapability(ERenderingCapability::DEPTH_TEST, pMask & 0x10);

		if ((pMask & 0x08) && ((pMask & 0x20) != (mState & 0x20) || (pMask & 0x40) != (mState & 0x40)))
		{
			int backBit = pMask & 0x20;
			int frontBit = pMask & 0x40;
			SetCullFace(backBit && frontBit ? ECullFace::FRONT_AND_BACK : (backBit ? ECullFace::BACK : ECullFace::FRONT));
		}

		mState = pMask;
	}
}

void Render::Render_Renderer::SetState(uint8_t pState)
{
	mState = pState;
}

const Render::Render_Renderer::FrameInfo& Render::Render_Renderer::GetFrameInfo() const
{
	return mFrameInfo;
}

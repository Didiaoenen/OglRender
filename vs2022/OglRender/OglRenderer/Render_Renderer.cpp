#include <functional>

#include "Render_Renderer.h"

Render::Render_Renderer::Render_Renderer(Render_Driver& pDriver) :
	mDriver(pDriver), m_state(0)
{
}

void Render::Render_Renderer::SetClearColor(float p_red, float p_green, float p_blue, float p_alpha)
{
}

void Render::Render_Renderer::Clear(bool p_colorBuffer, bool p_depthBuffer, bool p_stencilBuffer)
{
}

void Render::Render_Renderer::Clear(Render_Camera& pCamera, bool p_colorBuffer, bool p_depthBuffer, bool p_stencilBuffer)
{
}

void Render::Render_Renderer::SetRasterizationLinesWidth(float p_width)
{
}

void Render::Render_Renderer::SetRasterizationMode(ERasterizationMode p_rasterizationMode)
{
}

void Render::Render_Renderer::SetCapability(ERenderingCapability p_capability, bool pValue)
{
}

bool Render::Render_Renderer::GetCapability(ERenderingCapability p_capability) const
{
	return false;
}

void Render::Render_Renderer::SetStencilAlgorithm(EComparaisonAlgorithm p_algorithm, int32_t pReference, uint32_t p_mask)
{
}

void Render::Render_Renderer::SetDepthAlgorithm(EComparaisonAlgorithm p_algorithm)
{
}

void Render::Render_Renderer::SetStencilMask(uint32_t p_mask)
{
}

void Render::Render_Renderer::SetStencilOperations(EOperation p_stencilFail, EOperation p_depthFail, EOperation p_bothPass)
{
}

void Render::Render_Renderer::SetCullFace(ECullFace p_cullFace)
{
}

void Render::Render_Renderer::SetDepthWriting(bool pEnable)
{
}

void Render::Render_Renderer::SetColorWriting(bool p_enableRed, bool p_enableGreen, bool p_enableBlue, bool p_enableAlpha)
{
}

void Render::Render_Renderer::SetColorWriting(bool pEnable)
{
}

void Render::Render_Renderer::SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
}

void Render::Render_Renderer::ReadPixels(uint32_t x, uint32_t y, uint32_t width, uint32_t height, EPixelDataFormat format, EPixelDataType type, void* data)
{
}

bool Render::Render_Renderer::GetBool(GLenum p_parameter)
{
	return false;
}

bool Render::Render_Renderer::GetBool(GLenum p_parameter, uint32_t pIndex)
{
	return false;
}

int Render::Render_Renderer::GetInt(GLenum p_parameter)
{
	return 0;
}

int Render::Render_Renderer::GetInt(GLenum p_parameter, uint32_t pIndex)
{
	return 0;
}

float Render::Render_Renderer::GetFloat(GLenum p_parameter)
{
	return 0.0f;
}

float Render::Render_Renderer::GetFloat(GLenum p_parameter, uint32_t pIndex)
{
	return 0.0f;
}

double Render::Render_Renderer::GetDouble(GLenum p_parameter)
{
	return 0.0;
}

double Render::Render_Renderer::GetDouble(GLenum p_parameter, uint32_t pIndex)
{
	return 0.0;
}

int64_t Render::Render_Renderer::GetInt64(GLenum p_parameter)
{
	return 0;
}

int64_t Render::Render_Renderer::GetInt64(GLenum p_parameter, uint32_t pIndex)
{
	return 0;
}

std::string Render::Render_Renderer::GetString(GLenum p_parameter)
{
	return std::string();
}

std::string Render::Render_Renderer::GetString(GLenum p_parameter, uint32_t pIndex)
{
	return std::string();
}

void Render::Render_Renderer::ClearFrameInfo()
{
}

void Render::Render_Renderer::Draw(Render_IMesh& pMesh, EPrimitiveMode p_primitiveMode, uint32_t p_instances)
{
}

std::vector<std::reference_wrapper<Render::Render_Mesh>> Render::Render_Renderer::GetMeshesInFrustum(const Render_Model& pModel, const Render_BoundingSphere& p_modelBoundingSphere, const Maths::Maths_FTransform& p_modelTransform, const Render_Frustum& pFrustum, ECullingOptions p_cullingOptions)
{
	return std::vector<std::reference_wrapper<Render_Mesh>>();
}

uint8_t Render::Render_Renderer::FetchGLState()
{
	return 0;
}

void Render::Render_Renderer::ApplyStateMask(uint8_t p_mask)
{
}

void Render::Render_Renderer::SetState(uint8_t pState)
{
}

const Render::Render_Renderer::FrameInfo& Render::Render_Renderer::GetFrameInfo() const
{
	return m_frameInfo;
}

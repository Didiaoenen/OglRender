
#include <GL/glew.h>

#include "Render_Shader.h"
#include "Render_UniformBuffer.h"

Render::Render_UniformBuffer::Render_UniformBuffer(size_t pSize, uint32_t pBindingPoint, uint32_t pOffset, EAccessSpecifier pAccessSpecifier)
{
	glGenBuffers(1, &mBufferID);
	glBindBuffer(GL_UNIFORM_BUFFER, mBufferID);
	glBufferData(GL_UNIFORM_BUFFER, pSize, nullptr, static_cast<GLint>(pAccessSpecifier));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, pBindingPoint, mBufferID, pOffset, pSize);
}

Render::Render_UniformBuffer::~Render_UniformBuffer()
{
	glDeleteBuffers(1, &mBufferID);
}

void Render::Render_UniformBuffer::Bind()
{
	glBindBuffer(GL_UNIFORM_BUFFER, mBufferID);
}

void Render::Render_UniformBuffer::Unbind()
{
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

uint32_t Render::Render_UniformBuffer::GetID() const
{
	return mBufferID;
}

void Render::Render_UniformBuffer::BindBlockToShader(Render_Shader& pShader, uint32_t pUniformBlockLocation, uint32_t pBindingPoint)
{
	for (auto it = pShader.mPrograms.begin(); it != pShader.mPrograms.end(); it++)
	{
		auto& [name, program] = *it;
		glUniformBlockBinding(program.mId, pUniformBlockLocation, pBindingPoint);
	}
}

void Render::Render_UniformBuffer::BindBlockToShader(Render_Shader& pShader, const std::string& pName, uint32_t pBindingPoint)
{
	for (auto it = pShader.mPrograms.begin(); it != pShader.mPrograms.end(); it++)
	{
		auto& [name, program] = *it;
		glUniformBlockBinding(program.mId, GetBlockLocation(program, pName), pBindingPoint);
	}
}

uint32_t Render::Render_UniformBuffer::GetBlockLocation(Render_Program& pProgram, const std::string& pName)
{
	return glGetUniformBlockIndex(pProgram.mId, pName.c_str());
}

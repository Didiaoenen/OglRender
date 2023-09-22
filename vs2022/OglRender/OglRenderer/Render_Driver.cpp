#include <GL/glew.h>

#include "Render_Driver.h"

Render::Render_Driver::Render_Driver(const Render_DriverSettings& pDriverSettings)
{
	InitGlew();

	mIsActive = true;

	if (pDriverSettings.debugMode)
	{
		GLint flags;
		glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
		if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
		{
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(GLDebugMessageCallback, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		}
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glCullFace(GL_BACK);
}

bool Render::Render_Driver::IsActive() const
{
	return mIsActive;
}

void Render::Render_Driver::InitGlew()
{
	const GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		std::string message = "Error Init GLEW: ";
		std::string glewError = reinterpret_cast<const char*>(glewGetErrorString(error));
		//OVLOG_INFO(message + glewError);
	}
}

void Render::Render_Driver::GLDebugMessageCallback(uint32_t pSource, uint32_t pType, uint32_t pId, uint32_t pSeverity, int32_t pLength, const char* pMessage, const void* pUserParam)
{
	if (pId == 131169 || pId == 131185 || pId == 131218 || pId == 131204) return;

	std::string output;

	output += "OpenGL Debug Message:\n";
	output += "Debug message (" + std::to_string(pId) + "): " + pMessage + "\n";

	switch (pSource)
	{
		case GL_DEBUG_SOURCE_API:				output += "Source: API";				break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		output += "Source: Window System";		break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER:	output += "Source: Shader Compiler";	break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:		output += "Source: Third Party";		break;
		case GL_DEBUG_SOURCE_APPLICATION:		output += "Source: Application";		break;
		case GL_DEBUG_SOURCE_OTHER:				output += "Source: Other";				break;
	}

	output += "\n";

	switch (pType)
	{
		case GL_DEBUG_TYPE_ERROR:               output += "Type: Error";				break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: output += "Type: Deprecated Behavior"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  output += "Type: Undefined Behavior";	break;
		case GL_DEBUG_TYPE_PORTABILITY:         output += "Type: Portability";			break;
		case GL_DEBUG_TYPE_PERFORMANCE:         output += "Type: Performance";			break;
		case GL_DEBUG_TYPE_MARKER:              output += "Type: Marker";				break;
		case GL_DEBUG_TYPE_PUSH_GROUP:          output += "Type: Push Group";			break;
		case GL_DEBUG_TYPE_POP_GROUP:           output += "Type: Pop Group";			break;
		case GL_DEBUG_TYPE_OTHER:               output += "Type: Other";				break;
	}

	output += "\n";

	switch (pSeverity)
	{
	case GL_DEBUG_SEVERITY_HIGH:			output += "Severity: High";				break;
	case GL_DEBUG_SEVERITY_MEDIUM:			output += "Severity: Medium";			break;
	case GL_DEBUG_SEVERITY_LOW:				output += "Severity: Low";				break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:	output += "Severity: Notification";		break;
	}

	//switch (severity)
	{
		//case GL_DEBUG_SEVERITY_HIGH:			OVLOG_ERROR(output);	break;
		//case GL_DEBUG_SEVERITY_MEDIUM:			OVLOG_WARNING(output);	break;
		//case GL_DEBUG_SEVERITY_LOW:				OVLOG_INFO(output);		break;
		//case GL_DEBUG_SEVERITY_NOTIFICATION:	OVLOG_INFO(output);			break;
	}
}

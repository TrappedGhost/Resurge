#include "rgpch.h"
#include "OpenGLContext.h"

#include<GLFW/glfw3.h>
#include<glad/glad.h>
#include<gl/GL.h>

namespace Resug
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		RG_CORE_ASSERT(windowHandle, "window is null");
	}
	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		RG_CORE_ASSERT(status, "Failed to load glad");

		RG_CORE_INFO("Vendor: {0}", (const char*)glGetString(GL_VENDOR));
		RG_CORE_INFO("Renderer: {0}", (const char*)glGetString(GL_RENDERER));
		RG_CORE_INFO("Version: {0}", (const char*)glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{

		glfwSwapBuffers(m_WindowHandle);
	}
}
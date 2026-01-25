#pragma once

#include"Resug/Core.h"
#include"Resug/Log.h"
#include"Resug/Renderer/GraphicsContext.h"
 
struct GLFWwindow;

namespace Resug
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};
}


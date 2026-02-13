#pragma once

#include"Resug/Core/Core.h"
#include"Resug/Core/Log.h"
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


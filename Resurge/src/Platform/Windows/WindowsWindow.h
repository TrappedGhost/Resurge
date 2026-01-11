#pragma once

#include "Resug/Window.h"
#include"Resug/Event/ApplicationEvent.h"
#include <GLFW/glfw3.h>
//struct GLFWwindow;
namespace Resug
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow() override;
		virtual void OnUpdate() override;
		virtual inline uint GetWidth()const override { return m_Data.Width; }
		virtual inline uint GetHeight()const override { return m_Data.Heigth; }
		virtual inline void* GetNativeWindow()const override { return m_Window; }
		virtual inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSync()const override;

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();


		GLFWwindow* m_Window;
		struct WindowData
		{
			std::string Title;
			uint Heigth;
			uint Width;
			bool VSync;

			EventCallbackFn EventCallback;
		};
		WindowData m_Data;
	};

}


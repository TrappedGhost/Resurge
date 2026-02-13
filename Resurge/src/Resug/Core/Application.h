#pragma once
#include"Core.h"

#include"Window.h" 
#include"Resug/Event/Event.h"
#include"Resug/Event/ApplicationEvent.h"
#include"Resug/Event/MouseEvent.h"
#include"LayerStack.h"

#include"Resug/Renderer/Shader.h"
#include "Resug/ImGui/ImGuiLayer.h"
#include"Resug/Renderer/VertexArray.h"
#include"Resug/Renderer/Buffer.h"

#include"Resug/Renderer/OrthographicCamera.h"

namespace Resug {
	class RESUG_API Application
	{
	public:
		Application();
		~Application();
		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverLayer(Layer* layer);
		inline static Application& Get() { return *s_Instance;}
		inline Window& GetWindow() { return *m_Window; }
	private:

		bool OnWindowClose(WindowCloseEvent& event);
		bool OnWindowResize(WindowResizeEvent& event);
		
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;

		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;

		float m_LastFrameTime;
	private:
		static Application* s_Instance;
	};

	//define in client;
	Resug::Application* CreateApplication();
}


#pragma once
#include"Core.h"

#include"Window.h" 
#include "Event/Event.h"
#include"Event/ApplicationEvent.h"
#include"Event/MouseEvent.h"
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
		
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;

		bool m_Running = true;
		LayerStack m_LayerStack;

		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_VertexArray;

		std::shared_ptr<Shader> m_BlueShader;
		std::shared_ptr<VertexArray> m_SquareVA;

		OrthographicCamera m_Camera;
	private:
		static Application* s_Instance;
	};

	//define in client;
	Resug::Application* CreateApplication();
}


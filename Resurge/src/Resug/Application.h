#pragma once
#include"Core.h"
#include "Event/Event.h"
#include"Event/ApplicationEvent.h"
#include"Event/MouseEvent.h"
#include"Window.h" 
#include"LayerStack.h"
#include"Resug/Input.h"

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
		bool m_Running = true;
		LayerStack m_LayerStack;
		
		static Application* s_Instance;
	};

	//define in client;
	Resug::Application* CreateApplication();
}


#pragma once
#include"Core.h"
#include "Event/Event.h"
#include"Event/ApplicationEvent.h"
#include"Event/MouseEvent.h"
#include"Window.h" 
#include"LayerStack.h"
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

	private:

		bool OnWindowClose(WindowCloseEvent& event);
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};

	//define in client;
	Resug::Application* CreateApplication();
}


#pragma once
#include"Core.h"
#include "Event/Event.h"
#include"Event/ApplicationEvent.h"
#include"Window.h" 
namespace Resug {
	class RESUG_API Application
	{
	public:
		Application();
		~Application();
		void Run();

		void OnEvent(Event& e);
	private:

		bool OnWindowClose(WindowCloseEvent& event);
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	//define in client;
	Resug::Application* CreateApplication();
}


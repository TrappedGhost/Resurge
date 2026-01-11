#include"rgpch.h"
#include "Application.h"
#include"Log.h"
//#include"GLFW/glfw3.h"
#include"glad/glad.h"


namespace Resug 
{


	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		RG_CORE_ASSERT(!instance, "applicaion has have");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
	}
	Application::~Application()
	{

	} 
	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}
	void Application::PushOverLayer(Layer* overLayer)
	{
		m_LayerStack.PushOverLayer(overLayer);
		overLayer->OnAttach();
	}
	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

		//RG_CORE_INFO("{0}", e.ToString());

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{
		RG_CLIENT_INFO("start");
	
		while (m_Running)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			

			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate();
			}
			
			m_Window->OnUpdate();

		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}
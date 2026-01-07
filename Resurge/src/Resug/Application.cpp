#include"rgpch.h"
#include "Application.h"
#include"Log.h"
//#include"GLFW/glfw3.h"
#include"glad/glad.h"

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

namespace Resug 
{
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
	}
	Application::~Application()
	{

	} 
	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}
	void Application::PushOverLayer(Layer* overLayer)
	{
		m_LayerStack.PushOverLayer(overLayer);
	}
	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

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
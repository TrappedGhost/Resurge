#include"rgpch.h"
#include "Application.h"
#include"Log.h"

#include"GLFW/glfw3.h"

#include"Resug/Core/Input.h"

#include"Resug/Renderer/Renderer.h"

#include"Resug/Core/Timestep.h"

namespace Resug 
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		RG_PROFILE_FUNCTION();

		RG_CORE_ASSERT(!s_Instance, "applicaion has have");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(RG_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverLayer(m_ImGuiLayer);
	}

	Application::~Application()
	{

	} 

	void Application::PushLayer(Layer* layer)
	{
		RG_PROFILE_FUNCTION();
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverLayer(Layer* overLayer)
	{
		RG_PROFILE_FUNCTION();
		m_LayerStack.PushOverLayer(overLayer);
		overLayer->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(RG_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(RG_BIND_EVENT_FN(Application::OnWindowResize));

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
			
			float time = glfwGetTime();
			Timestep ts = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				for (Layer* layer : m_LayerStack)
				{
					layer->OnUpdate(ts);
				}
			}
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
			{
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();

		}
	}

	void Application::Close()
	{

	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
		}

		m_Minimized = false;
		Renderer::WindowResize(e.GetWidth(),e.GetHeight());

		return false;
	}
}
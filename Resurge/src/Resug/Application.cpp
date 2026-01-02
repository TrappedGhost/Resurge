#include "Application.h"
#include"Log.h"
#include"Event/ApplicationEvent.h"

namespace Resug 
{
	Application::Application()
	{

	}
	Application::~Application()
	{

	}

	void Application::Run()
	{
		WindowResizeEvent e(1000, 1000);
		RG_CLIENT_TRACE(e.ToString());
		while (true);
	}
}
#pragma once
#include"Core.h"

namespace Resug {
	class RESUG_API Application
	{
	public:
		Application();
		~Application();
		void Run();

	};

	//define in client;
	Resug::Application* CreateApplication();
}


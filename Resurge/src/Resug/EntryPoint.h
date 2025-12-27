#pragma once


#ifdef RG_PLATFORM_WINDOWS

extern Resug::Application* Resug::CreateApplication();

int main()
{
	printf("Hello");
	auto app = Resug::CreateApplication();
	app->Run();
	delete app;

}

#endif // RG_PLATFORM_WINDOWS

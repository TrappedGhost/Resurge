#pragma once


#ifdef RG_PLATFORM_WINDOWS

extern Resug::Application* Resug::CreateApplication();

int main(int argc, char** argv)
{
	Resug::Log::Init();
	int a = 12;
	RG_CORE_INFO("hello world;{0}",a);
	auto app = Resug::CreateApplication();
	app->Run();
	delete app;

}

#endif // RG_PLATFORM_WINDOWS

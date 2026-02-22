#pragma once


#ifdef RG_PLATFORM_WINDOWS

extern Resug::Application* Resug::CreateApplication();

int main(int argc, char** argv)
{
	Resug::Log::Init();

	RG_PROFILE_BEGIN_SESSION("Startup", "ResugProfile-Startup.json");
	auto app = Resug::CreateApplication();
	RG_PROFILE_END_SESSION();
	
	RG_PROFILE_BEGIN_SESSION("Runtime", "ResugProfile-Runtime.json");
	app->Run();
	RG_PROFILE_END_SESSION();

	RG_PROFILE_BEGIN_SESSION("Shutdown", "ResugProfile-Shutdown.json");
	delete app;
	RG_PROFILE_END_SESSION();

}

#endif // RG_PLATFORM_WINDOWS

#pragma once

#ifdef RG_PLATFORM_WINDOWS
	#ifdef RG_BUILD_DLL
		#define RESUG_API __declspec(dllexport) 
	#else 
		#define RESUG_API __declspec(dllimport) 
	#endif // RG_BUILD_DLL
#else 
	#error only one.
#endif // RG_PLATFORM_WINDOWS

#define BIT(x) (1<<x)
#define uint unsigned int



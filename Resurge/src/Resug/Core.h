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


#ifdef RG_ENABLE_ASSERTS
	#define RG_CLIENT_ASSERT(x, ...) { if(!(x)){HZ_CLIENT_ERROR("Assertion Failed: {0}",__VA_ARGS__);__debugbreak();}}	
	#define RG_CORE_ASSERT(x, ...) { if(!(x)){HZ_CORE_ERROR("Assertion Failed: {0}",__VA_ARGS__);__debugbreak();}}	
#else
	#define RG_CLIENT_ASSERT(x,...)
	#define RG_CORE_ASSERT(x,...)
#endif // RG_ENABLE_ASSERTS



#define BIT(x) (1<<x)
#define uint unsigned int



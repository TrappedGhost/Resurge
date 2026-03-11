#pragma once

#include<memory>

#ifdef RG_PLATFORM_WINDOWS
#if RG_DYNAMIC_LINK
	#ifdef RG_BUILD_DLL
			#define RESUG_API __declspec(dllexport) 
		#else 
			#define RESUG_API __declspec(dllimport) 
		#endif // RG_BUILD_DLL
#else
#define RESUG_API
#endif
#else 
	#error only one.
#endif // RG_PLATFORM_WINDOWS


#ifdef RG_ENABLE_ASSERTS
	#define RG_CLIENT_ASSERT(x, ...) { if(!(x)){RG_CLIENT_ERROR("Assertion Failed: {0}",__VA_ARGS__);__debugbreak();}}	
	#define RG_CORE_ASSERT(x, ...) { if(!(x)){RG_CORE_ERROR("Assertion Failed: {0}",__VA_ARGS__);__debugbreak();}}	
#else
	#define RG_CLIENT_ASSERT(x,...)
	#define RG_CORE_ASSERT(x,...)
#endif // RG_ENABLE_ASSERTS



#define BIT(x) (1<<x)

typedef unsigned int uint;

#define RG_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)


namespace Resug
{
	typedef unsigned int uint;
	
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}


}



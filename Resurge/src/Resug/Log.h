#pragma once

#include<memory>
#include"Core.h"
#include"spdlog/spdlog.h"

namespace Resug 
{
	class RESUG_API Log
	{
	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger> GetCoreLogger(){return s_CoreLogger;}
		inline static std::shared_ptr<spdlog::logger> GetClientLogger(){return s_CoreLogger;}
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}
// Core log macros
#define RG_CORE_TRACE(...)  ::Resug::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define RG_CORE_INFO(...)   ::Resug::Log::GetCoreLogger()->info(__VA_ARGS__)
#define RG_CORE_WARN(...)   ::Resug::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define RG_CORE_ERROR(...)  ::Resug::Log::GetCoreLogger()->error(__VA_ARGS__)
#define RG_CORE_FATAL(...)  ::Resug::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define RG_CLIENT_TRACE(...)  ::Resug::Log::GetClientLogger()->trace(__VA_ARGS__)
#define RG_CLIENT_INFO(...)   ::Resug::Log::GetClientLogger()->info(__VA_ARGS__)
#define RG_CLIENT_WARN(...)   ::Resug::Log::GetClientLogger()->warn(__VA_ARGS__)
#define RG_CLIENT_ERROR(...)  ::Resug::Log::GetClientLogger()->error(__VA_ARGS__)
#define RG_CLIENT_FATAL(...)  ::Resug::Log::GetClientLogger()->fatal(__VA_ARGS__)


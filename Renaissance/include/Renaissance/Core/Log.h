#pragma once

#include <memory>

#include "Core.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"

namespace Renaissance
{
	class Log
	{
	public: 
		Log();
		~Log();

		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return sCoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return sClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> sCoreLogger;
		static std::shared_ptr<spdlog::logger> sClientLogger;
	};
}

#ifdef REN_BUILD_SHIPPING
	// core logging macros
	#define REN_CORE_ERROR(...) 
	#define REN_CORE_WARN(...)	
	#define REN_CORE_INFO(...)	
	#define REN_CORE_TRACE(...) 
	#define REN_CORE_FATAL(...) 
	
	// client logging macros
	#define REN_ERROR(...) 
	#define REN_WARN(...)  
	#define REN_INFO(...)  
	#define REN_TRACE(...) 
	#define REN_FATAL(...) 
#else
	// core logging macros
	#define REN_CORE_ERROR(...) ::Renaissance::Log::GetCoreLogger()->error(__VA_ARGS__)
	#define REN_CORE_WARN(...)	::Renaissance::Log::GetCoreLogger()->warn(__VA_ARGS__)
	#define REN_CORE_INFO(...)	::Renaissance::Log::GetCoreLogger()->info(__VA_ARGS__)
	#define REN_CORE_TRACE(...) ::Renaissance::Log::GetCoreLogger()->trace(__VA_ARGS__)
	#define REN_CORE_FATAL(...) ::Renaissance::Log::GetCoreLogger()->fatal(__VA_ARGS__)
	
	// client logging macros
	#define REN_ERROR(...) ::Renaissance::Log::GetClientLogger()->error(__VA_ARGS__)
	#define REN_WARN(...)  ::Renaissance::Log::GetClientLogger()->warn(__VA_ARGS__)
	#define REN_INFO(...)  ::Renaissance::Log::GetClientLogger()->info(__VA_ARGS__)
	#define REN_TRACE(...) ::Renaissance::Log::GetClientLogger()->trace(__VA_ARGS__)
	#define REN_FATAL(...) ::Renaissance::Log::GetClientLogger()->fatal(__VA_ARGS__)
#endif
#pragma once

#include <memory>
#include "Renaissance/Core/PlatformDetection.h"

#if defined(REN_PLATFORM_WINDOWS)
	#ifdef REN_BUILD_DLL
		#define REN_API __declspec(dllexport)
	#else 
		#define REN_API __declspec(dllimport)
	#endif
#elif defined(REN_PLATFORM_OSX)

#elif defined(REN_PLATFORM_LINUX)

#else
	#error Renaissance engine is only compatible with Windows at this moment.
#endif

#define BIT(x) (1 << x)

namespace Renaissance
{
	template<typename T>
	using UniquePtr = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr UniquePtr<T> MakeUnique(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args));
	}
	
	template<typename T>
	using SharedPtr = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr SharedPtr<T> MakeShared(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args));
	}
}

#include "Renaissance/Core/Log.h"
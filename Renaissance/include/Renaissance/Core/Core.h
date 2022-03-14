#pragma once

#include <memory>
#include <string>
#include "Renaissance/Core/PlatformDetection.h"

#if REN_DEBUG
	#if defined (REN_PLATFORM_WINDOWS)
		#define REN_DEBUGBREAK() __debugbreak()
	#elif defined (REN_PLATFORM_LINUX)
		#include <signal.h>
		#define REN_DEBUGBREAK() raise(SIGTRAP)
	#elif defined (REN_PLATFORM_OSX)
		#define REN_DEBUGBREAK() __debugbreak()
	#else
		#error "Debugbreak not supported on current platform."
	#endif
	#define REN_ENABLE_ASSERTS
#else
	#define REN_DEBUGBREAK()
#endif

#define REN_STRINGIFY_MACRO(x) #x
#define REN_EXPAND_MACRO(x) x

#define BIT(x) (1 << x)

#define REN_BIND_EVENT_OBJ(obj, fn) [obj](auto&&... args) -> decltype(auto) { return obj->fn(std::forward<decltype(args)>(args)...); }
#define REN_BIND_EVENT(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Renaissance
{
	template<typename T>
	using UniquePtr = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr UniquePtr<T> MakeUnique(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}
	
	template<typename T>
	using SharedPtr = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr SharedPtr<T> MakeShared(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using WeakPtr = std::weak_ptr<T>;

	typedef std::string String;
}

#include "Renaissance/Core/Log.h"
#include "Renaissance/Core/Assert.h"
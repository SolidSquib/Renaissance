#pragma once

#include "Renaissance/Core/Core.h"
#include "Renaissance/Core/Log.h"
#include <filesystem>

#ifdef REN_ENABLE_ASSERTS

// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
#define REN_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { REN##type##ERROR(msg, __VA_ARGS__); REN_DEBUGBREAK(); } }
#define REN_INTERNAL_ASSERT_WITH_MSG(type, check, ...) REN_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
#define REN_INTERNAL_ASSERT_NO_MSG(type, check) REN_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", REN_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

#define REN_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define REN_INTERNAL_ASSERT_GET_MACRO(...) REN_EXPAND_MACRO( REN_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, REN_INTERNAL_ASSERT_WITH_MSG, REN_INTERNAL_ASSERT_NO_MSG) )

// Currently accepts at least the condition and one additional parameter (the message) being optional
#define REN_ASSERT(...) REN_EXPAND_MACRO( REN_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
#define REN_CORE_ASSERT(...) REN_EXPAND_MACRO( REN_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
#define REN_ASSERT(...)
#define REN_CORE_ASSERT(...)
#endif

#include "RenaissancePCH.h"
#include "Renaissance/Core/Log.h"

namespace Renaissance
{
	std::shared_ptr<spdlog::logger> Log::sCoreLogger;
	std::shared_ptr<spdlog::logger> Log::sClientLogger;

	Log::Log()
	{

	}

	Log::~Log()
	{

	}

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		
		sCoreLogger = spdlog::stdout_color_mt("RENAISSANCE");
		sCoreLogger->set_level(spdlog::level::trace);

		sClientLogger = spdlog::stdout_color_mt("APP");
		sClientLogger->set_level(spdlog::level::trace);
	}
}


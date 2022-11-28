#pragma once

#include "Renaissance/Core/Core.h"

namespace Renaissance
{
	class FileSystem
	{
	public:
		static std::filesystem::path GetAppContentDirectory();
		static std::filesystem::path GetEngineContentDirectory();
		static std::string GetAppContentDirectoryString();
		static std::string GetEngineContentDirectoryString();

		static std::filesystem::path GetPathRelativeToAppContent(const std::filesystem::path& relativePath);
		static std::filesystem::path GetPathRelativeToEngineContent(const std::filesystem::path& relativePath);
		static std::string GetPathStringRelativeToAppContent(const std::filesystem::path& relativePath);
		static std::string GetPathStringRelativeToEngineContent(const std::filesystem::path& relativePath);
	};

	std::filesystem::path operator"" _engine(const char* string, std::size_t size) noexcept;
	std::filesystem::path operator"" _app(const char* string, std::size_t size) noexcept;
	std::string operator"" _sengine(const char* string, std::size_t size) noexcept;
	std::string operator"" _sapp(const char* string, std::size_t size) noexcept;
}
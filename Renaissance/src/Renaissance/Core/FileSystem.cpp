#include "RenaissancePCH.h"
#include "Renaissance/Core/FileSystem.h"

namespace Renaissance
{
	// #TODO filesystem: these paths should be better cached and should not rely on the __FILE__ macro since I think the use of it here is making some assumptions.

	std::filesystem::path FileSystem::GetAppContentDirectory()
	{
		std::filesystem::path path = std::filesystem::current_path() / "assets";
		std::string pathstring = path.string();
		REN_CORE_TRACE("app path is {0}", pathstring.c_str());
		return path;
	}

	std::filesystem::path FileSystem::GetEngineContentDirectory()
	{
		std::filesystem::path sourcePath(__FILE__);
		sourcePath = sourcePath.parent_path() / "../../../assets";
		sourcePath = std::filesystem::absolute(sourcePath);
		return sourcePath;
	}

	std::string FileSystem::GetAppContentDirectoryString()
	{
		return GetAppContentDirectory().string();
	}

	std::string FileSystem::GetEngineContentDirectoryString()
	{
		return GetEngineContentDirectory().string();
	}

	std::filesystem::path FileSystem::GetPathRelativeToAppContent(const std::filesystem::path& relativePath)
	{
		return GetAppContentDirectory() / relativePath;
	}

	std::filesystem::path FileSystem::GetPathRelativeToEngineContent(const std::filesystem::path& relativePath)
	{
		return GetEngineContentDirectory() / relativePath;
	}

	std::string FileSystem::GetPathStringRelativeToAppContent(const std::filesystem::path& relativePath)
	{
		return GetPathRelativeToAppContent(relativePath).string();
	}

	std::string FileSystem::GetPathStringRelativeToEngineContent(const std::filesystem::path& relativePath)
	{
		return GetPathRelativeToEngineContent(relativePath).string();
	}

	std::filesystem::path operator"" _engine(const char* string, std::size_t size) noexcept
	{
		return FileSystem::GetEngineContentDirectory() / string;
	}

	std::filesystem::path operator"" _app(const char* string, std::size_t size) noexcept
	{
		return FileSystem::GetAppContentDirectory() / string;
	}

	std::string operator"" _sengine(const char* string, std::size_t size) noexcept
	{
		std::filesystem::path path = FileSystem::GetEngineContentDirectory() / string;
		return path.string();
	}

	std::string operator"" _sapp(const char* string, std::size_t size) noexcept
	{
		std::filesystem::path path = FileSystem::GetAppContentDirectory() / string;
		return path.string();
	}
}
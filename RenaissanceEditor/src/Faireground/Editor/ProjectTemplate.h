#pragma once

#include "Renaissance/Core/Core.h"

class ProjectTemplate
{
public:
	ProjectTemplate(const char* path);

	std::string GetName() const { return mName; }
	std::string GetFilePath() const { return mFilePath; }
	std::vector<std::string> GetFolders() const { return mFolders; }

private:
	std::string mName;
	std::string mFilePath;
	std::vector<std::string> mFolders;
};


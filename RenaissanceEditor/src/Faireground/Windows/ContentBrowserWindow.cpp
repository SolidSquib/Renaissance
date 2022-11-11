#include "FairegroundPCH.h"
#include "Faireground/Windows/ContentBrowserWindow.h"

#include "IconsFontAwesome5.h"
#include "imgui_internal.h"

namespace Renaissance
{
	static const std::filesystem::path sAssetsDirectory = "assets"; // #TODO replace this when we have projects.
	ImFont* ContentBrowserWindow::IconFontLarge = nullptr;

	ContentBrowserWindow::ContentBrowserWindow(uint32_t index)
		: mContentBrowserIndex(index), mActivePath(sAssetsDirectory)
	{
		mContentBrowserName = index == 0 ? "Content Browser" : "Content Browser " + std::to_string(mContentBrowserIndex + 1);

		mFolderIcon = Graphics::TextureLibrary::GetGlobal().Get<Graphics::Texture2D>("FolderOpenIcon");
		mImageIcon= Graphics::TextureLibrary::GetGlobal().Get<Graphics::Texture2D>("ImageIcon");
		mFileIcon = Graphics::TextureLibrary::GetGlobal().Get<Graphics::Texture2D>("FileIcon");
	}

	void ContentBrowserWindow::OnUIRender()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin(mContentBrowserName.c_str(), &mOpen);
		{
			const bool upDirectoryEnabled = mActivePath != sAssetsDirectory;

			ImGui::PushStyleColor(ImGuiCol_Button, { 0,0,0,0 });

			if (!upDirectoryEnabled)
			{
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);				
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0,0,0,0 });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0,0,0,0 });
			}

			ImGui::Spacing();
			if (ImGui::ButtonEx(ICON_FA_LEVEL_UP_ALT, {0,0}, upDirectoryEnabled ? 0 : ImGuiItemFlags_Disabled))
			{
				mActivePath = mActivePath.parent_path();
				RefreshFolderView();
			} ImGui::SameLine();
			
			if (!upDirectoryEnabled)
			{
				ImGui::PopStyleVar();
				ImGui::PopStyleColor(3);
			}
			else
			{
				ImGui::PopStyleColor();
			}
				

			ImGui::Text("/"); ImGui::SameLine();

			std::filesystem::path previousPath;
			for (auto component : mActivePath)
			{			
				std::filesystem::path path = previousPath / component;

				String str = component.string();
				previousPath = path;			

				if (path == mActivePath)
				{
					ImGui::Text(str.c_str()); ImGui::SameLine();
				}
				else
				{
					if (ImGui::Button(str.c_str()))
					{
						mActivePath = path;
						RefreshFolderView();
						break;
					} ImGui::SameLine();
				}

				ImGui::Text("/");
				if (path != mActivePath)
					ImGui::SameLine();
			}
			
			const bool areSettingsOpen = DrawBrowserSettings();
			ImGui::Separator();

			{
				ImGui::BeginChild("Folder View", { 0,0 }, false, ImGuiWindowFlags_NoMove);
				{
					const bool shouldDrawAsList = mIconSize <= 30;

					if (!shouldDrawAsList)
					{
						float cellSize = (float)(mIconSize + (mIconPadding * 2));
						float panelWidth = ImGui::GetContentRegionAvailWidth();
						int columnsCount = (int)(panelWidth / cellSize);
						if (columnsCount < 1)
							columnsCount = 1;

						ImGui::Columns(columnsCount, "FolderViewColumns", false);
					}

					for (auto& path : mActivePathDirectories)
					{
						DrawIconForPath(path, shouldDrawAsList);

						if (!shouldDrawAsList)
							ImGui::NextColumn();
					}

					for (auto& path : mActivePathFiles)
					{
						DrawIconForPath(path, shouldDrawAsList);

						if (!shouldDrawAsList)
							ImGui::NextColumn();
					}

					if (!shouldDrawAsList)
						ImGui::Columns(1);
				}
				ImGui::EndChild();
			}
		}
		ImGui::End();
		ImGui::PopStyleVar();
	}

	void ContentBrowserWindow::Open()
	{
		RefreshFolderView();

		if (!IconFontLarge)
		{
			ImGuiIO& io = ImGui::GetIO();
			static const ImWchar iconRanges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
			ImFontConfig iconConfig;
			iconConfig.MergeMode = false;
			iconConfig.PixelSnapH = true;
			IconFontLarge = io.Fonts->AddFontFromFileTTF("../Renaissance/assets/fonts/" FONT_ICON_FILE_NAME_FAS, 64.0f, &iconConfig, iconRanges);
		}
	}

	void ContentBrowserWindow::Close()
	{

	}
	
	void ContentBrowserWindow::RefreshFolderView()
	{
		mActivePathDirectories.clear();
		mActivePathFiles.clear();

		for (auto& path : std::filesystem::directory_iterator(mActivePath))
		{
			if (path.is_directory())
			{
				mActivePathDirectories.emplace_back(path);
			}
			else
			{
				mActivePathFiles.emplace_back(path);
			}
		}
	}

	bool ContentBrowserWindow::DrawBrowserSettings()
	{
		bool wasOpened = false;

		float textWidth = ImGui::CalcTextSize(ICON_FA_ELLIPSIS_H).x;

		ImGuiStyle& style = ImGui::GetStyle();

		ImGui::SameLine(ImGui::GetWindowWidth());
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() - textWidth - (style.FramePadding.x * 2.f));

		ImGui::PushStyleColor(ImGuiCol_Button, { 0,0,0,0 });
		if (ImGui::Button(ICON_FA_ELLIPSIS_H))
		{
			wasOpened = true;
			ImGui::OpenPopup("BrowserSettings");
		}
		ImGui::PopStyleColor();

		if (ImGui::BeginPopup("BrowserSettings"))
		{
			wasOpened = true;
			ImGui::TextColored({ 0.5f, 0.5f, 0.5f, 1.0f }, "Icons");
			ImGui::Separator();

			ImGui::SliderInt("Size", &mIconSize, 30, 64);
			ImGui::SliderInt("Padding", &mIconPadding, 10, 50);

			ImGui::EndPopup();
		}

		return wasOpened;
	}

	void ContentBrowserWindow::DrawIconForPath(const std::filesystem::path& path, bool asListItem)
	{
		float iconSize = (float)(mIconSize + (mIconPadding * 2));
		
		String iconString = ICON_FA_FILE;
				
		if (path.has_extension())
		{
			std::filesystem::path extension = path.extension();
			
			if (extension == ".png"
				|| extension == ".tga"
				|| extension == ".jpeg"
				|| extension == ".jpg"
				|| extension == ".bmp")
			{
				iconString = ICON_FA_FILE_IMAGE;
			}
			else if (extension == ".glsl"
				|| extension == ".hlsl"
				|| extension == ".txt")
			{
				iconString = ICON_FA_FILE_CODE;
			}
		}
		else if (std::filesystem::is_directory(path))
		{
			iconString = ICON_FA_FOLDER;
		}

		ImGui::PushStyleColor(ImGuiCol_Button, { 0,0,0,0 });

		if (asListItem)
		{
			std::string folderName = path.filename().string();
			ImGui::Button(iconString.append(" ").append(folderName).c_str());
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && std::filesystem::is_directory(path))
			{
				mActivePath = path;
				RefreshFolderView();
			}
		}
		else
		{
			ImGui::PushFont(IconFontLarge);
			ImGui::SetWindowFontScale(mIconSize / IconFontLarge->FontSize);
			ImGui::Button(iconString.c_str(), {iconSize, iconSize});
			ImGui::SetWindowFontScale(1.0f);
			ImGui::PopFont();
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && std::filesystem::is_directory(path))
			{
				mActivePath = path;
				RefreshFolderView();
			}

			std::string folderName = path.filename().string();
			float textWidth = ImGui::CalcTextSize(folderName.c_str()).x;
			//ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ((iconSize - textWidth) * 0.5f));
			ImGui::TextWrapped(folderName.c_str());
		}

		ImGui::PopStyleColor();
	}
}
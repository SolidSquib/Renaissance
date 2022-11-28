#include "FairegroundPCH.h"
#include "Faireground/Windows/EditorToolbarWindow.h"
#include "Faireground/EditorLayer.h"

#include "IconsFontAwesome5.h"
#include "imgui_internal.h"

namespace Renaissance
{
	void EditorToolbarWindow::OnUIRender()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, { 64.0f, 64.0f });
		ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
		ImGui::Begin("Toolbar", &mOpen, ImGuiWindowFlags_NoScrollbar);
		{
			float cellSize = mButtonSize + (mButtonPadding * 2);
			float panelWidth = ImGui::GetContentRegionAvail().x;
			int columnsCount = (int)(panelWidth / cellSize);
			if (columnsCount < 1)
				columnsCount = 1;

			float useButtonSize = ImGui::GetContentRegionAvail().x / columnsCount;

			ImGui::Columns(columnsCount, "ToolbarColumns", false);
			SharedPtr<Scene> activeScene = EditorLayer::GetActiveScene().lock();
			
			const bool isSceneLoaded = activeScene.get();
			const bool isEditing = EditorLayer::GetUpdateState() == EditorLayer::EEditorUpdateMode::Editor;

			auto DrawButton = [&](bool enable, auto function) {

				if (!enable)
				{
					ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0,0,0,0 });
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0,0,0,0 });
				}

				function();

				if (!enable)
				{
					ImGui::PopStyleVar();
					ImGui::PopStyleColor(2);
				}
			};

			DrawButton(isSceneLoaded && isEditing, [&]() {
				
				if (ImGui::ButtonEx(ICON_FA_SAVE, { useButtonSize,useButtonSize }, isSceneLoaded ? 0 : ImGuiItemFlags_Disabled))
				{
					std::ofstream output("assets/cerealTestScene.json");
					cereal::JSONOutputArchive writer(output);
					Scene& sceneRef = *(activeScene.get());
					writer(cereal::make_nvp("Scene", sceneRef));
				}
			});
			
			DrawButton(isEditing, [&]() {
				
				if (ImGui::ButtonEx(ICON_FA_FOLDER_OPEN, { useButtonSize, useButtonSize }))
				{
					std::ifstream input("assets/cerealTestScene.json");
					cereal::JSONInputArchive writer(input);
					SharedPtr<Scene> newScene = MakeShared<Scene>();
					Scene& sceneRef = *(newScene.get());
					writer(cereal::make_nvp("Scene", sceneRef));
				}
			});

			DrawButton(isSceneLoaded, [&]() {

				switch (EditorLayer::GetUpdateState())
				{
				case EditorLayer::EEditorUpdateMode::Editor:
					if (ImGui::ButtonEx(ICON_FA_PLAY, { useButtonSize,useButtonSize }, isSceneLoaded ? 0 : ImGuiItemFlags_Disabled))
					{
						EditorLayer::StartPlayInEditor();
					}
					break;

				case EditorLayer::EEditorUpdateMode::Play:
				case EditorLayer::EEditorUpdateMode::Simulate:
					if (ImGui::ButtonEx(ICON_FA_STOP, { useButtonSize,useButtonSize }, isSceneLoaded ? 0 : ImGuiItemFlags_Disabled))
					{
						EditorLayer::StopPlayAndSimulate();
					}
					break;
				}
			});
			
			ImGui::NextColumn();
		}
		ImGui::End();
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor();
	}
}
#pragma once

#include "PropertyEditorWindow.h"

#include "Faireground/EditorLayer.h"
#include "IconsFontAwesome5.h"

#include <glm/gtc/type_ptr.hpp>

// for InputText that uses std::string
#include "misc/cpp/imgui_stdlib.h"

namespace Renaissance
{
	class PropertyEditor
	{
	public:
		static ImGuiTreeNodeFlags sComponentTreeNodeFlags;

		static void DrawVector2Control(const String& label, Math::Vector2& values, float resetValue = 0.0f);
		static void DrawVector3Control(const String& label, Math::Vector3& values, float resetValue = 0.0f);

		template<typename T>
		static void DrawComponentSettingsButton(const String& id, T& component, Entity& entity, bool removable)
		{
			ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);

			ImGui::PushID(id.c_str());

			if (ImGui::Button("+", ImVec2(25.f, 19.0f)))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Component", nullptr, nullptr, removable))
				{
					entity.RemoveComponent<T>();
				}
			
				ImGui::EndPopup();
			}

			ImGui::PopID();
		}

		template<typename T>
		static void DrawComponent(T& component, Entity& entity)
		{
			REN_CORE_ASSERT(!"No explicit template instantiation found!");
		}

		template<>
		static void DrawComponent<TagComponent>(TagComponent& component, Entity& entity)
		{
			if (ImGui::CollapsingHeader("Identifier Component", sComponentTreeNodeFlags))
			{
				DrawComponentSettingsButton("IdentifierComponentSettings", component, entity, false);
								
				ImGui::Columns(2, "Columns");
				ImGui::Text("Name");
				ImGui::NextColumn();
				ImGui::InputText("##Name", &component.Tag);
				ImGui::NextColumn();

				ImGui::Columns(1);
			}
			else
			{
				DrawComponentSettingsButton("IdentifierComponentSettings", component, entity, false);
			}

			ImGui::Spacing();
		}

		template<>
		static void DrawComponent<TransformComponent>(TransformComponent& component, Entity& entity)
		{
			if (ImGui::CollapsingHeader("Transform Component", sComponentTreeNodeFlags))
			{
				DrawComponentSettingsButton("TransformComponentSettings", component, entity, false);

				DrawVector3Control("Location", component.Location, 0.0f);
				DrawVector3Control("Rotation", component.Rotation, 0.0f);
				DrawVector3Control("Scale", component.Scale, 1.0f);
			}
			else
			{
				DrawComponentSettingsButton("TransformComponentSettings", component, entity, false);
			}

			ImGui::Spacing();
		}

		template<>
		static void DrawComponent<SpriteRendererComponent>(SpriteRendererComponent& component, Entity& entity)
		{
			if (ImGui::CollapsingHeader("Sprite Renderer Component", sComponentTreeNodeFlags))
			{
				DrawComponentSettingsButton("SpriteRendererComponentSettings", component, entity, true);

				ImGui::Columns(2, "Columns");
				ImGui::Text("Tint");
				ImGui::NextColumn();
				ImGui::ColorEdit4("##Tint", glm::value_ptr(component.Color));
				ImGui::NextColumn();

				ImGui::Text("Sprite");
				ImGui::NextColumn();
				ImVec2 uv[2] = { { component.Texture.MinCoord.x, component.Texture.MaxCoord.y }, { component.Texture.MaxCoord.x, component.Texture.MinCoord.y } };
				ImVec4 color = { component.Color.r, component.Color.g, component.Color.b, component.Color.a };
			
				SharedPtr<Graphics::Texture2D> useTexture = component.Texture.Texture ? component.Texture.Texture : Graphics::TextureLibrary::GetGlobal().Get<Graphics::Texture2D>("DEFAULT");
				ImGui::Image((ImTextureID)(uint64_t)useTexture->GetRendererId(), { 64.0f, 64.0f }, uv[0], uv[1], color);
				
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* Payload = ImGui::AcceptDragDropPayload(DRAG_CONTEXT_PATH_TEX))
					{
						std::filesystem::path* filepath = reinterpret_cast<std::filesystem::path*>(Payload->Data);
						component.Texture.Texture = Graphics::TextureLibrary::GetGlobal().FindOrLoad<Graphics::Texture2D>(*filepath);
					}

					ImGui::EndDragDropTarget();
				}

				if (component.Texture.Texture)
				{
					ImGui::PushStyleColor(ImGuiCol_Button, { 0,0,0,0 });
					ImGui::SameLine();
					if (ImGui::Button(ICON_FA_TIMES))
					{
						component.Texture.Texture = nullptr;
					}
					ImGui::PopStyleColor();
				}

				ImGui::NextColumn();

				ImGui::Text("MinCoord");
				ImGui::NextColumn();
				DrawVector2Control("##MinCoord", component.Texture.MinCoord);
				ImGui::NextColumn();

				ImGui::Text("MaxCoord");
				ImGui::NextColumn();
				DrawVector2Control("##MaxCoord", component.Texture.MaxCoord, 1.0f);
				ImGui::NextColumn();

				ImGui::Text("Size");
				ImGui::NextColumn();
				DrawVector2Control("##Size", component.Size, 1.0f);
				ImGui::NextColumn();

				ImGui::Text("Tiling");
				ImGui::NextColumn();
				DrawVector2Control("##Tiling", component.TilingFactor, 1.0f);
				ImGui::NextColumn();

				ImGui::Columns(1);
			}
			else
			{
				DrawComponentSettingsButton("SpriteRendererComponentSettings", component, entity, true);
			}

			ImGui::Spacing();
		}

		template<>
		static void DrawComponent<CameraComponent>(CameraComponent& component, Entity& entity)
		{
			if (ImGui::CollapsingHeader("Camera Component", sComponentTreeNodeFlags))
			{
				DrawComponentSettingsButton("CameraComponentSettings", component, entity, true);

				Math::Vector2 viewportSize = { component.GetViewportWidth(), component.GetViewportHeight() };
				float nearClipping = component.GetNearClipping();
				float farClipping = component.GetFarClipping();
				bool isOrtho = component.IsOrthographic();
				float orthoScale = component.GetOrthoScale();
				float fov = component.GetFieldOfView();

				ImGui::Columns(2, "Columns");
				ImGui::Text("Viewport Size");
				ImGui::NextColumn();
				DrawVector2Control("##ViewportSize", viewportSize);
				ImGui::NextColumn();

				ImGui::Text("Near Clipping");
				ImGui::NextColumn();
				ImGui::DragFloat("##NearClipping", &nearClipping, 0.1f, 0.0f, farClipping, "%.3f");
				ImGui::NextColumn();

				ImGui::Text("Far Clipping");
				ImGui::NextColumn();
				ImGui::DragFloat("##FarClipping", &farClipping, 0.1f, nearClipping, FLT_MAX, "%.3f");
				ImGui::NextColumn();

				ImGui::Text("Project Orthographic");
				ImGui::NextColumn();
				if (ImGui::Checkbox("##IsOrthographic", &isOrtho))
				{
					if (isOrtho)
					{
						component.SetOrthographic();
					}
					else
					{
						component.SetPerspective();
					}
				}
				ImGui::NextColumn();

				if (component.IsOrthographic())
				{
					ImGui::Text("Ortho Scale");
					ImGui::NextColumn();
					ImGui::DragFloat("##OrthoScale", &orthoScale, 0.1f, 0.001f, FLT_MAX, "%.3f");
					ImGui::NextColumn();
				}
				else
				{
					ImGui::Text("Field of View");
					ImGui::NextColumn();
					ImGui::DragFloat("##FieldOfView", &fov, 1.0f, 0.0f, 360.f, "%.0f", ImGuiSliderFlags_Logarithmic);
					ImGui::NextColumn();
				}

				ImGui::Text("Main Camera?");
				ImGui::NextColumn();
				if (ImGui::Checkbox("##MainCamera", &component.MainCamera))
				{
					REN_CORE_INFO("clicked checkbox");
				}
				ImGui::NextColumn();

				ImGui::Columns(1);

				component.SetViewportSize(viewportSize.x, viewportSize.y);
				component.SetNearClipping(nearClipping);
				component.SetFarClipping(farClipping);
				component.SetOrthoScale(orthoScale);
				component.SetFieldOfView(fov);
			}
			else
			{
				DrawComponentSettingsButton("CameraComponentSettings", component, entity, true);
			}

			ImGui::Spacing();
		}

		template<>
		static void DrawComponent<NativeScriptComponent>(NativeScriptComponent& component, Entity& entity)
		{
			if (ImGui::CollapsingHeader("Native Script Component", sComponentTreeNodeFlags))
			{
				DrawComponentSettingsButton("NativeScriptComponentSettings", component, entity, true);
			}
			else
			{
				DrawComponentSettingsButton("NativeScriptComponentSettings", component, entity, true);
			}

			ImGui::Spacing();
		}
	};
}

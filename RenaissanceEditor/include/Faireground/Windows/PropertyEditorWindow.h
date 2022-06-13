#pragma once

#include "Faireground/Windows/EditorWindow.h"
#include "Renaissance/Scene/Entity.h"
#include "Renaissance/Scene/Component.h"

namespace Renaissance
{
	class PropertyEditor
	{
		static void DrawTransform(TransformComponent& transformComponent);
		static void DrawSpriteRenderer(SpriteRendererComponent& transformComponent);
		static void DrawCamera(CameraComponent& transformComponent);
		static void DrawNativeScript(NativeScriptComponent& transformComponent);

		friend class PropertyEditorWindow;
	};

	class PropertyEditorWindow : public EditorWindow
	{
	public:
		PropertyEditorWindow() = default;

		virtual void Open() override;
		virtual void Close() override;
		virtual void OnUIRender() override;
		virtual void OnUpdate(float deltaTime) override;

		void OnEntitySelectionChanged(const Entity& newSelection);

		virtual bool WantsToClose() const override { return !mOpen; }

	private:
		bool mOpen = true;
		Entity mSelectedEntity;
		Connection<void(const Entity&)> mOnSelectedEntityChangedDelegate;
	};
}
#pragma once

#include "Renaissance/Core/Core.h"
#include "Renaissance/Core/Layer.h"

namespace Renaissance
{
	class LayerStack
	{
	public:
		LayerStack() {}
		~LayerStack();

		template <typename T, typename ... Args>
		WeakPtr<T> CreateNewLayer(Args&& ... args)
		{
			SharedPtr<T> newLayer = MakeShared<T>(std::forward<Args>(args)...);
			mActiveLayers.emplace(Begin() + mActiveLayerInsertOffset++, newLayer);
			newLayer->OnAttached();
			return newLayer;
		}

		template <typename T, typename ... Args>
		WeakPtr<T> CreateNewOverlay(Args&& ... args)
		{
			SharedPtr<T> newOverlay = MakeShared<T>(std::forward<Args>(args)...);
			mActiveLayers.emplace_back(newOverlay);
			newOverlay->OnAttached();
			return newOverlay;
		}

		void EnableLayer(const WeakPtr<Layer>& layer);
		void DisableLayer(const WeakPtr<Layer>& layer);
		void EnableOverlay(const WeakPtr<Layer>& overlay);
		void DisableOverlay(const WeakPtr<Layer>& overlay);
		void OnUpdate(float deltaTime);
		void RenderUI();
		void OnEvent(Events::Event& e);

	private:
		std::vector<SharedPtr<Layer>> mActiveLayers;
		std::vector<SharedPtr<Layer>> mDormantLayers;
		int mActiveLayerInsertOffset = 0;
		int mDormantLayerInsertOffset = 0;
	};
}
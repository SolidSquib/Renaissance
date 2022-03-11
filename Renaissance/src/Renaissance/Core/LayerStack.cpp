#include "RenaissancePCH.h"
#include "Renaissance/Core/LayerStack.h"

namespace Renaissance
{
	LayerStack::~LayerStack()
	{
		for (auto& it : mActiveLayers)
		{
			it->OnDetached();
		}
		
		mActiveLayers.clear();
		mDormantLayers.clear();
	}

	void LayerStack::EnableLayer(const WeakPtr<Layer>& layer)
	{
		auto it = std::find(mDormantLayers.begin(), mDormantLayers.begin() + mDormantLayerInsertOffset, layer.lock());
		if (it != mDormantLayers.begin() + mDormantLayerInsertOffset)
		{
			(*it)->OnAttached();
			mActiveLayers.emplace(mActiveLayers.begin() + mActiveLayerInsertOffset++, *it);
			mDormantLayers.erase(it);
			mDormantLayerInsertOffset--;
		}
	}

	void LayerStack::DisableLayer(const WeakPtr<Layer>& layer)
	{
		auto it = std::find(mActiveLayers.begin(), mActiveLayers.begin() + mActiveLayerInsertOffset, layer.lock());
		if (it != mActiveLayers.begin() + mActiveLayerInsertOffset)
		{
			(*it)->OnDetached();
			mDormantLayers.emplace(mDormantLayers.begin() + mDormantLayerInsertOffset++, *it);
			mActiveLayers.erase(it);
			mActiveLayerInsertOffset--;
		}
	}

	void LayerStack::EnableOverlay(const WeakPtr<Layer>& overlay)
	{
		auto it = std::find(mDormantLayers.begin() + mDormantLayerInsertOffset, mDormantLayers.end(), overlay.lock());
		if (it != mDormantLayers.end())
		{
			(*it)->OnAttached();
			mActiveLayers.emplace_back(*it);
			mDormantLayers.erase(it);
		}
	}

	void LayerStack::DisableOverlay(const WeakPtr<Layer>& overlay)
	{
		auto it = std::find(mActiveLayers.begin() + mActiveLayerInsertOffset, mActiveLayers.end(), overlay.lock());
		if (it != mActiveLayers.end())
		{
			(*it)->OnDetached();
			mDormantLayers.emplace_back(*it);
			mActiveLayers.erase(it);
		}
	}

	void LayerStack::OnUpdate(float deltaTime)
	{
		for (auto it = mActiveLayers.begin(); it != mActiveLayers.end(); ++it)
		{
			(*it)->OnUpdate(deltaTime);
		}
	}

	void LayerStack::RenderUI()
	{
		for (auto it = mActiveLayers.begin(); it != mActiveLayers.end(); ++it)
		{
			(*it)->OnUIRender();
		}
	}

	void LayerStack::OnEvent(Events::Event& e)
	{
		for (auto it = mActiveLayers.rbegin(); it != mActiveLayers.rend(); ++it)
		{
			if (e.IsHandled())
			{
				break;
			}

			(*it)->OnEvent(e);
		}
	}
}


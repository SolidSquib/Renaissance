#pragma once

#include "cereal/cereal.hpp"
#include "cereal/types/vector.hpp"

namespace Renaissance::Config
{
	struct SavedWindowData
	{
		uint32_t Width;
		uint32_t Height;
		int X;
		int Y;
		bool Maximized;

		template <class Archive>
		void serialize(Archive& ar)
		{
			ar(
				CEREAL_NVP(Width),
				CEREAL_NVP(Height),
				CEREAL_NVP(X),
				CEREAL_NVP(Y),
				CEREAL_NVP(Maximized)
			);
		}
	};
	
	struct EngineConfig
	{
		std::vector<SavedWindowData> Windows;

		template <class Archive>
		void serialize(Archive& ar)
		{
			ar(CEREAL_NVP(Windows));
		}
	};
}
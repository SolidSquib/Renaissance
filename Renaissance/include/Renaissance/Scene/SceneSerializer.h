#pragma once

#include "Renaissance.h"

#include <yaml-cpp/yaml.h>

namespace Renaissance
{
	class SceneSerializer
	{
	public:
		SceneSerializer(const SharedPtr<Scene>& scene);
		virtual ~SceneSerializer() = default;

		void SerializeText(const String& filepath);
		void SerializeBinary(const String& filepath);
		bool DeserializeText(const String& filepath);
		bool DeserializeBinary(const String& filepath);

	private:
		SharedPtr<Scene> mScene;
	};
}
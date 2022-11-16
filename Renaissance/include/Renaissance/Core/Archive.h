#pragma once

#include <tuple>
#include <queue>
#include <string>
#include <entt.hpp>

namespace Renaissance
{
	class Archive
	{
		using ArchiveData = std::tuple<
			std::queue<entt::entity>,
			std::queue<uint64_t>,
			std::queue<uint32_t>,
			std::queue<int>,
			std::queue<float>,
			std::queue<bool>,
			std::queue<std::string>>;

	public:
		Archive() = default;

		Archive(Archive&& other) noexcept 
			: mData()
		{
			mData = other.mData;
		}

		Archive(const Archive& ar)
			: mData(ar.mData)
		{

		}

		virtual ~Archive() = default;

		template<typename T>
		Archive& operator<<(T value)
		{
			std::get<std::queue<T>>(mData).push(value);
			return *this;
		}

		template<typename T>
		Archive& operator>>(T& value) 
		{
			auto& queue = std::get<std::queue<std::decay_t<T>>>(mData);
			value = queue.front();
			queue.pop();
			return *this;
		}

		Archive& operator=(Archive&& other) noexcept 
		{
			if (this != &other)
			{
				mData = other.mData;
			}
			return *this;
		}

	private:
		ArchiveData mData;
	};
}
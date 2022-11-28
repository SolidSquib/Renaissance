#pragma once

#include <xhash>

namespace cereal
{
	class access;
}

namespace Renaissance
{

	class GUID
	{
	public:
		GUID();
		GUID(uint64_t id);
		GUID(const GUID&) = default;

		explicit operator uint64_t() const { return mGuid; }

	private:
		friend class cereal::access;

		template <class Archive>
		uint64_t save_minimal(Archive const&) const
		{
			return mGuid;
		}

		template <class Archive>
		void load_minimal(Archive const&, uint64_t const& value)
		{
			mGuid = value;
		}

	private:
		uint64_t mGuid;
	};
}

namespace std
{
	template<>
	struct hash<Renaissance::GUID>
	{
		std::size_t operator()(const Renaissance::GUID& guid) const
		{
			return hash<uint64_t>()((uint64_t)guid);
		}
	};
}
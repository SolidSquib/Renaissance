#pragma once

#include <xhash>

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
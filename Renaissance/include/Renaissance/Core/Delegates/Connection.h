#pragma once

#include "Renaissance/Core/Delegates/Callable.h"

namespace Renaissance
{
	template<typename Signature>
	class MulticastDelegate;

	// Connection object for multicast delegates
	template<typename Signature>
	class Connection;

	template<typename Return, typename... Args>
	class Connection<Return(Args...)>
	{
		friend class MulticastDelegate<void(Args...)>;

	public:
		Connection() = default;
		Connection(const SharedPtr<Callable<Return(Args...)>>& callable) : mCallable(callable) { }

		bool IsValid() const { return !mCallable.expired(); }

	private:
		WeakPtr<Callable<Return(Args...)>> mCallable;
	};
}
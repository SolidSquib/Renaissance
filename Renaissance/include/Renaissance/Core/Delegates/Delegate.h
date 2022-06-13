#pragma once

#include "Renaissance/Core/Core.h"
#include "Renaissance/Core/Delegates/Callable.h"
#include "Renaissance/Core/Delegates/Connection.h"

namespace Renaissance
{	
	// Delegate class to be instantiated by subjects
	template<typename Signature>
	class Delegate;

	template<typename Signature>
	class MulticastDelegate;

	template<typename Return, typename... Args>
	class Delegate<Return(Args...)>
	{
		friend class MulticastDelegate<void(Args...)>;
	public:
		Delegate() : mCallable(nullptr) {}
		Delegate(const Delegate& other) = delete;
		Delegate(Delegate&& other) : mCallable(other.mCallable) { other.mCallable = nullptr; }
		~Delegate() = default;

		Delegate& operator=(Delegate const& other) = delete;
		Delegate& operator=(Delegate&& other)
		{
			Delegate temp(std::move(other));
			Swap(temp);
			return *this;
		}

		template<typename T>
		void Bind(T& instance, Return(T::*function)(Args...))
		{
			mCallable.reset(new CallableMember<T, Return(Args...)> (instance, function));
		}

		template<typename T>
		void Bind(T& instance, Return(T::*function)(Args...) const)
		{
			mCallable.reset(new CallableMemberConst<T, Return(Args...)>(instance, function));
		}

		template<typename T>
		void Bind(T&& functionObject)
		{
			mCallable.reset(new CallableLambda<std::remove_reference_t<T>, Return(Args...)>(std::forward<T>(functionObject)));
		}

		void Swap(Delegate& other)
		{
			SharedPtr<Callable<Return(Args...)>> temp = mCallable;
			mCallable = other.mCallable;
			other.mCallable = temp;
		}

		void Clear()
		{
			mCallable.reset();
		}

		bool IsBound() const { return mCallable != nullptr; }

		Return Invoke(Args... args)
		{
			return mCallable->Invoke(std::forward<Args>(args)...);
		}

		Return operator()(Args... args)
		{
			return mCallable->Invoke(std::forward<Args>(args)...);
		}

	private:
		SharedPtr<Callable<Return(Args...)>> mCallable;
	};

	template<typename... Args>
	class MulticastDelegate<void(Args...)>
	{		
	public:
		template<typename T> 
		Connection<void(Args...)> Bind(T& instance, void(T::* function)(Args...))
		{
			Delegate<void(Args...)> delegate;
			mDelegates.push_back(std::move(delegate));
			mDelegates.back().Bind(instance, function);
			return Connection<void(Args...)>(mDelegates.back().mCallable);
		}

		template<typename T>
		Connection<void(Args...)> Bind(T& instance, void(T::* function)(Args...) const)
		{
			Delegate<void(Args...)> delegate;
			mDelegates.push_back(std::move(delegate));
			mDelegates.back().Bind(instance, function);
			return Connection<void(Args...)>(mDelegates.back().mCallable);
		}

		template<typename T>
		Connection<void(Args...)> Bind(T&& functionObject)
		{
			Delegate<void(Args...)> delegate;
			mDelegates.push_back(std::move(delegate));
			mDelegates.back().Bind(functionObject);
			return Connection<void(Args...)>(mDelegates.back().mCallable);
		}

		void Unbind(const Connection<void(Args...)>& connection)
		{
			for (auto iter = mDelegates.begin(); iter != mDelegates.end(); ++iter)
			{
				if (iter->mCallable == connection.mCallable.lock())
				{
					mDelegates.erase(iter);
					return;
				}
			}
		}

		void Clear()
		{
			mDelegates.clear()
		}

		bool IsBound() const { return !mDelegates.empty(); }

		void Invoke(Args... args)
		{
			for (auto& delegate : mDelegates)
				delegate.Invoke(std::forward<Args>(args)...);
		}

		void operator()(Args... args)
		{
			for (auto& delegate : mDelegates)
				delegate.Invoke(std::forward<Args>(args)...);
		}

	private:
		std::vector<Delegate<void(Args...)>> mDelegates;
	};
}
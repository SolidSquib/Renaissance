#pragma once

namespace Renaissance
{
	template<typename Signature>
	class Callable;

	template<typename Return, typename... Args>
	class Callable<Return(Args...)>
	{
	public:
		virtual ~Callable() = default;

		virtual Return Invoke(Args... args) = 0;

	protected:
		Callable() = default;
	};

	// Wrapper for a non-const member function
	template<typename T, typename Signature>
	class CallableMember;

	template<typename T, typename Return, typename... Args>
	class CallableMember<T, Return(Args...)> : public Callable<Return(Args...)>
	{
		typedef Return(T::* MemberFunctionPtr)(Args...);

	public:
		CallableMember(T& instance, MemberFunctionPtr function) : mInstance(instance), mFunction(function) {}

		virtual Return Invoke(Args... args) final { return (mInstance.*mFunction)(std::forward<Args>(args)...); }

	private:
		T& mInstance;
		MemberFunctionPtr mFunction;
	};

	// Wrapper for a const member function
	template<typename T, typename Signature>
	class CallableMemberConst;

	template<typename T, typename Return, typename... Args>
	class CallableMemberConst<T, Return(Args...)> : public Callable<Return(Args...)>
	{
		typedef Return(T::* MemberFunctionPtr)(Args...) const;

	public:
		CallableMemberConst(T& instance, MemberFunctionPtr function) : mInstance(instance), mFunction(function) {}

		virtual Return Invoke(Args... args) final { return (mInstance.*mFunction)(std::forward<Args>(args)...); }

	private:
		T& mInstance;
		MemberFunctionPtr mFunction;
	};

	/* Wrapper for a function object or lambda
	 * NOTE: This can and will allocate memory on the heap if required, e.g. if the function captures any variables. */
	template<typename T, typename Signature>
	class CallableLambda;

	template<typename T, typename Return, typename... Args>
	class CallableLambda<T, Return(Args...)> : public Callable<Return(Args...)>
	{
	public:
		CallableLambda(T& function) : mFunction(function), mAllocated(false) {}						// lvalue can be copied with no issues
		CallableLambda(T&& function) : mFunction(new T(std::move(function))), mAllocated(true) {}	// rvalue must be copied and have memory allocated on the heap
		~CallableLambda() { Destroy(); }

		virtual Return Invoke(Args... args) final { return (*mFunction)(std::forward<Args>(args)...); }

	private:
		template<typename U = T, typename = std::enable_if_t<std::is_function<U>::value>>
		void Destroy() {}
		template<typename = T, typename U = T, typename = std::enable_if_t<!std::is_function<U>::value>>
		void Destroy() { if (mAllocated) delete mFunction; }

	private:
		T* mFunction;
		bool mAllocated;
	};

}
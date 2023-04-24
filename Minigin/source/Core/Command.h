#pragma once
#include <functional>

namespace dae
{
	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute() = 0;
	};

	class ActionCommand final : public Command
	{
	public:
		virtual ~ActionCommand() override = default;

		virtual void Execute() override;

		template<typename T, typename Func> 
		inline static ActionCommand Create(T* t, Func&& fn) 
		{
			return ActionCommand{ std::bind(fn, t) };
		}

	private:
		std::function<void()> m_FnAction;

		explicit ActionCommand(const std::function<void()>& fnAction);
	};

	template <typename T>
	class ValueCommand final : public Command
	{
	public:
		virtual ~ValueCommand() override = default;

		inline virtual void Execute() override
		{
			if (m_FnAction)
				m_FnAction(m_Value);
		}

		template<typename Obj, typename Func>
		inline static ValueCommand Create(Func&& fn, Obj* pObj)
		{
			return ValueCommand{ std::bind(fn, pObj, std::placeholders::_1)};
		}

		inline void SetValue(const T& value) { m_Value = value; }

	private:
		std::function<void(T)> m_FnAction;
		T m_Value{};

		explicit ValueCommand(const std::function<void(T)>& fnAction)
			: m_FnAction{ fnAction }
		{
		}
	};
}
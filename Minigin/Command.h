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
}
#pragma once
#include <functional>

namespace dae
{
	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute() = 0;
		virtual void Undo() = 0;
	};

	class ActionCommand final : public Command
	{
	public:
		explicit ActionCommand(const std::function<void()>& fnAction);
		virtual ~ActionCommand() override = default;

		virtual void Execute() override;
		virtual void Undo() override {} //nothing to undo

	private:
		std::function<void()> m_FnAction;
	};
}
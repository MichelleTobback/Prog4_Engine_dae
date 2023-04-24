#include "Command.h"

//======================================================
//					Action Command					  //
//======================================================

dae::ActionCommand::ActionCommand(const std::function<void()>& fnAction)
	: m_FnAction{fnAction}
{
}

void dae::ActionCommand::Execute()
{
	if (m_FnAction)
		m_FnAction();
}
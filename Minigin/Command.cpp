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
	m_FnAction();
}

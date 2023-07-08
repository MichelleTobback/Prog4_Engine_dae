#pragma once
#include "State/State.h"

namespace dae
{
	class GameMode : public State
	{
	public:
		virtual ~GameMode() = default;

		virtual void OnEnter() override {}
		virtual StatePtr OnUpdate() override { return nullptr; }
		virtual void OnExit() override {}
	};
}

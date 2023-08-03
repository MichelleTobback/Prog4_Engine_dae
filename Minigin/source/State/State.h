#pragma once
#include <memory>

namespace dae
{
	class State
	{
	public:
		using StatePtr = State*;
		virtual ~State() = default;

		virtual void OnEnter() = 0;
		virtual StatePtr OnUpdate() = 0;
		virtual void OnExit() = 0;
	};
}
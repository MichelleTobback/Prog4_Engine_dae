#pragma once
#include <memory>
#include "Core/Memory/Allocator.h"

namespace dae
{
	using StateAllocator = LinkedListAllocator;
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
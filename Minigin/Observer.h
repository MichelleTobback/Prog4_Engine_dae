#pragma once
#include <string>

namespace dae
{
	struct Event
	{
		uint32_t Id;
		char* data;
		size_t dataSize;
	};

	class Observer
	{
	public:
		Observer() = default;
		virtual ~Observer() = default;

		virtual void Invoke(const Event& event) = 0;
	};
}
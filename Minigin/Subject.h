#pragma once
#include "Observer.h"
#include <vector>

namespace dae
{
	class Subject final
	{
	public:
		Subject() = default;
		~Subject();

		void AddObserver(Observer* pObserver);
		void RemoveObserver(Observer* pObserver);
		void Invoke(const Event& event);

	private:
		std::vector<Observer*> m_pObservers;
	};
}
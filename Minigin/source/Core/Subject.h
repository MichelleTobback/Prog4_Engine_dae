#pragma once
#include "Observer.h"
#include <vector>
#include <queue>

namespace dae
{
	class Subject final
	{
	public:
		Subject() = default;
		~Subject();

		void AddObserver(Observer* pObserver);
		//removes the observer immediately
		void RemoveObserver(Observer* pObserver);
		//removes the observer when all observers are invoked
		void MarkForDestroy(Observer* pObserver);
		void Invoke(const Event& event);
		void Clear();

	private:
		std::vector<Observer*> m_pObservers;
		std::queue<Observer*> m_pObserversToRemove{};
	};
}
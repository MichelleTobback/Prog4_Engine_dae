#include "Subject.h"

dae::Subject::~Subject()
{
	 
}

void dae::Subject::AddObserver(Observer* pObserver)
{
	m_pObservers.push_back(pObserver);
}

void dae::Subject::RemoveObserver(Observer* pObserver)
{
	m_pObservers.erase(std::remove(m_pObservers.begin(), m_pObservers.end(), pObserver), m_pObservers.end());
}

void dae::Subject::MarkForDestroy(Observer* pObserver)
{
	auto it{ std::find(m_pObservers.begin(), m_pObservers.end(), pObserver) };
	if (it != m_pObservers.end())
	{
		m_ObserversPendingDestroy = true;
		(*it) = nullptr;
	}
}

void dae::Subject::Invoke(const Event& event)
{
	for (size_t i{}; i < m_pObservers.size(); ++i)
	{
		auto pObserver{ m_pObservers[i] };
		if (pObserver)
			pObserver->Invoke(event, this);
	}
	if (m_ObserversPendingDestroy)
		RemoveInvalidObservers();
}

void dae::Subject::Clear()
{
	m_pObservers.clear();
}

void dae::Subject::RemoveInvalidObservers()
{
	m_pObservers.erase(std::remove_if(m_pObservers.begin(), m_pObservers.end(), [](const auto& observer) 
		{
			return !observer;
		}), m_pObservers.end());

	m_ObserversPendingDestroy = false;
}

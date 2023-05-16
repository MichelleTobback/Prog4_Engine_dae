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
	m_pObserversToRemove.push(pObserver);
}

void dae::Subject::Invoke(const Event& event)
{
	for (auto pObserver : m_pObservers)
	{
		pObserver->Invoke(event, this);
	}
	while (!m_pObserversToRemove.empty())
	{
		RemoveObserver(m_pObserversToRemove.front());
		m_pObserversToRemove.pop();
	}
}

void dae::Subject::Clear()
{
	m_pObservers.clear();
	while (!m_pObserversToRemove.empty())
		m_pObserversToRemove.pop();
}

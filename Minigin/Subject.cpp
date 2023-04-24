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

void dae::Subject::Invoke(const Event& event)
{
	for (auto pObserver : m_pObservers)
	{
		pObserver->Invoke(event);
	}
}

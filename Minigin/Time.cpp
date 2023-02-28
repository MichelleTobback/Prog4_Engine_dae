#include "Time.h"

#include <chrono>

void dae::Time::Start()
{
	m_StartTime = std::chrono::high_resolution_clock::now();
	m_LastTime = m_StartTime;
}

void dae::Time::Update()
{
	m_CurrentTime = std::chrono::high_resolution_clock::now();

	m_DeltaTime = std::chrono::duration<float, std::milli>(m_CurrentTime - m_LastTime).count();

	m_LastTime = m_CurrentTime;
	m_Lag += m_DeltaTime;
}

void dae::Time::HandleFixedUpdate(const std::function<void()>& fnFixedUpdate)
{
	while (m_Lag >= m_FixedTimeStep)
	{
		fnFixedUpdate();
		m_Lag -= m_FixedTimeStep;
	}
}

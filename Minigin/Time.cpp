#include "Time.h"

#include <chrono>
#include <thread>

void dae::Time::Start()
{
	m_StartTime = std::chrono::high_resolution_clock::now();
	m_LastTime = m_StartTime;
	return;
}

void dae::Time::Update()
{
	m_CurrentTime = Now();

	m_DeltaTime = std::chrono::duration<float, std::milli>(m_CurrentTime - m_LastTime).count();

	m_LastTime = m_CurrentTime;
}

void dae::Time::Wait()
{
	const int targetFrameTime = 1000 / m_MaxFPS;

	const auto frameEnd{ m_LastTime + std::chrono::milliseconds(targetFrameTime) };
	const auto sleepTime{ frameEnd - Now() };

	std::this_thread::sleep_for(sleepTime);
}

dae::TimePoint dae::Time::Now()
{
	return std::chrono::high_resolution_clock::now();
}

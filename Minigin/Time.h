#pragma once
#include "Singleton.h"

#include <chrono>
#include <functional>

namespace dae
{
	typedef std::chrono::steady_clock::time_point TimePoint;

	class Time final : public Singleton<Time>
	{
	public:
		void Start();
		void Update();
		void HandleFixedUpdate(const std::function<void()>& fnFixedUpdate);

		inline float GetDeltaTime() const { return m_DeltaTime; }
		inline float GetDuration() const { return std::chrono::duration<float>(m_CurrentTime - m_StartTime).count(); }
		inline const TimePoint& GetTime() const { return m_CurrentTime; }
		inline float GetLag() const { return m_Lag; }
		inline float GetFixedTimeStep() const { return m_FixedTimeStep; }
		inline void SetFixedTimeStep(float value) { m_FixedTimeStep = value; }

	private:
		float m_DeltaTime{};
		float m_FixedTimeStep{0.02f};
		float m_Lag{};
		TimePoint m_LastTime{};
		TimePoint m_CurrentTime{};
		TimePoint m_StartTime{};
	};
}
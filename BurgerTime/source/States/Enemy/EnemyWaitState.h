#pragma once
#include "States/Enemy/EnemyState.h"
#include "Audio/AudioClip.h"
namespace dae
{
	enum class CollisionLayer;
	class EnemyWaitState final : public EnemyState
	{
	public:
		EnemyWaitState(EnemyComponent* pEnemy, float time = 3.f);
		virtual ~EnemyWaitState() = default;

		virtual void OnEnter() override;
		virtual StatePtr OnUpdate() override;
		virtual void OnExit() override;

	private:
		const float m_Time;
		float m_Current{};
	};
}
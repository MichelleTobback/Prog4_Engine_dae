#pragma once
#include "State/State.h"
#include "Component/Physics/RigidBody2DComponent.h"
#include "Components/CharacterInfo.h"

namespace dae
{
	class EnemyComponent;
	class EnemyState : public State
	{
	public:
		EnemyState(EnemyComponent* pEnemy);
		virtual ~EnemyState() = default;

		virtual void OnEnter() = 0;
		virtual StatePtr OnUpdate() = 0;
		virtual void OnExit() = 0;

		EnemyComponent* GetEnemy();
		CharacterInfo& GetCharacter();

		virtual void OnBeginOverlap(const CollisionHit&){}
		virtual void OnEndOverlap(const CollisionHit&) {}

	private:
		EnemyComponent* m_pEnemy;
	};
}
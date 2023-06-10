#pragma once
#include "State.h"
#include "Component/Component.h"

namespace dae
{
	class StateMachine final : public Component
	{
	public:
		StateMachine(GameObject* pOwner, State::StatePtr&& pEntry);
		~StateMachine() = default;

		virtual void Awake() override;
		virtual void OnDestroy() override;
		virtual void Update() override;

	private:
		State::StatePtr m_pCurrent;
	};
}
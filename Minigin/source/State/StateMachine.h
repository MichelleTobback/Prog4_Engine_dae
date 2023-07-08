#pragma once
#include "State.h"
#include "Component/Component.h"

namespace dae
{
	class StateMachine final : public Component
	{
	public:
		StateMachine(GameObject* pOwner, State::StatePtr pEntry);
		~StateMachine() = default;

		virtual void Awake() override;
		virtual void Sleep() override;
		virtual void Update() override;

		State& GetCurrent() const;

	private:
		State::StatePtr m_pCurrent;
	};
}
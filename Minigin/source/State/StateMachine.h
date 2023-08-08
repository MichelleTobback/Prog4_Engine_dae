#pragma once
#include "State.h"
#include "Component/Component.h"
#include <memory>

namespace dae
{
	class StateMachine final : public Component
	{
	public:
		StateMachine(GameObject* pOwner, State::StatePtr pState);
		~StateMachine() = default;

		virtual void Awake() override;
		virtual void Sleep() override;
		virtual void Update() override;

		State& GetCurrent() const;
		void SetState(State::StatePtr pState);

	private:
		State::StatePtr m_pCurrent;
	};
}
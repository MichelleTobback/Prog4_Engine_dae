#pragma once
#include "State/State.h"

namespace dae
{
	struct CharacterInfo;
	class CharacterInfoComponent;
	class CharacterState : public State
	{
	public:
		CharacterState(CharacterInfoComponent* pCharacterInfo)
			: m_pCharacterInfo{pCharacterInfo}{}
		virtual ~CharacterState() = default;

		virtual void OnEnter() = 0;
		virtual StatePtr OnUpdate() = 0;
		virtual void OnExit() = 0;

		CharacterInfo* GetCharacter();

	private:
		CharacterInfoComponent* m_pCharacterInfo;
	};
}
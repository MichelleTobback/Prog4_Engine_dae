#include "CharacterState.h"
#include "Components/CharacterInfo.h"

dae::CharacterInfo* dae::CharacterState::GetCharacter()
{
    return &m_pCharacterInfo->Get();
}

#pragma once
#include "State/State.h"
#include "Component/SpriteAnimatorComponent.h"
#include "Component/SpriteAtlasComponent.h"

namespace dae
{
	class SpriteAnimState : public State
	{
	public:
		SpriteAnimState(size_t clipId, SpriteAnimatorComponent* pAnimator);
		virtual ~SpriteAnimState() override = default;

		virtual void OnEnter() = 0;
		virtual StatePtr OnUpdate() = 0;
		virtual void OnExit() = 0;

		SpriteAnimClip& GetClip();

	protected:
		SpriteAnimClip& SetClip(size_t clip);
		size_t GetID() const;
		SpriteAnimatorComponent* GetAnimator();

	private:
		SpriteAnimatorComponent* m_pAnimator;
		size_t m_AnimClip;
	};
}
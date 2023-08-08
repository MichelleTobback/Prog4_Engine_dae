#pragma once
#include "SpriteAnimState.h"

namespace dae
{
	class CharacterAnimationController;
	class WalkDownAnimState final : public SpriteAnimState
	{
	public:
		WalkDownAnimState(size_t clipId, CharacterAnimationController* pAnimatorController);
		virtual ~WalkDownAnimState() override = default;

		virtual void OnEnter() override;
		virtual StatePtr OnUpdate() override;
		virtual void OnExit() override;

	private:
		CharacterAnimationController* m_pAnimController;
	};

	class WalkUpAnimState final : public SpriteAnimState
	{
	public:
		WalkUpAnimState(size_t clipId, CharacterAnimationController* pAnimatorController);
		virtual ~WalkUpAnimState() override = default;

		virtual void OnEnter() override;
		virtual StatePtr OnUpdate() override;
		virtual void OnExit() override;

	private:
		CharacterAnimationController* m_pAnimController;
	};

	class WalkRightAnimState final : public SpriteAnimState
	{
	public:
		WalkRightAnimState(size_t clipId, CharacterAnimationController* pAnimatorController);
		virtual ~WalkRightAnimState() override = default;

		virtual void OnEnter() override;
		virtual StatePtr OnUpdate() override;
		virtual void OnExit() override;

	private:
		CharacterAnimationController* m_pAnimController;
	};
	class WalkLeftAnimState final : public SpriteAnimState
	{
	public:
		WalkLeftAnimState(size_t clipId, CharacterAnimationController* pAnimatorController);
		virtual ~WalkLeftAnimState() override = default;

		virtual void OnEnter() override;
		virtual StatePtr OnUpdate() override;
		virtual void OnExit() override;

	private:
		CharacterAnimationController* m_pAnimController;
	};
	class DieAnimState final : public SpriteAnimState
	{
	public:
		DieAnimState(size_t clipId, CharacterAnimationController* pAnimatorController);
		virtual ~DieAnimState() override = default;

		virtual void OnEnter() override;
		virtual StatePtr OnUpdate() override;
		virtual void OnExit() override;

	private:
		void OnAnimEnd();

		CharacterAnimationController* m_pAnimController;
		bool m_AnimEnd{ false };
	};

	class StunnedAnimState final : public SpriteAnimState
	{
	public:
		StunnedAnimState(size_t clipId, CharacterAnimationController* pAnimatorController);
		virtual ~StunnedAnimState() override = default;

		virtual void OnEnter() override;
		virtual StatePtr OnUpdate() override;
		virtual void OnExit() override;

	private:
		CharacterAnimationController* m_pAnimController;
	};
}
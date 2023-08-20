#pragma once
#include "States/GameStates/GameState.h"

namespace dae
{
	class AudioClip;
	class HiScoreGameState final : public GameState
	{
	public:
		HiScoreGameState();
		virtual ~HiScoreGameState() = default;

		HiScoreGameState(const HiScoreGameState& other) = delete;
		HiScoreGameState(HiScoreGameState&& other) = delete;
		HiScoreGameState& operator=(const HiScoreGameState& other) = delete;
		HiScoreGameState& operator=(HiScoreGameState&& other) = delete;

		virtual void OnEnter(Scene& scene) override;
		virtual void OnExit(Scene& scene) override;

	private:
		void Next();
		std::unique_ptr<AudioClip> m_pMusic;
	};
}
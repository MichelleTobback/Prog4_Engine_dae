#pragma once
#include "States/GameStates/GameState.h"

namespace dae
{
	class BTEditorGameMode final : public GameState
	{
	public:
		BTEditorGameMode(size_t sceneIndex);
		virtual ~BTEditorGameMode() = default;

		BTEditorGameMode(const BTEditorGameMode& other) = delete;
		BTEditorGameMode(BTEditorGameMode&& other) = delete;
		BTEditorGameMode& operator=(const BTEditorGameMode& other) = delete;
		BTEditorGameMode& operator=(BTEditorGameMode&& other) = delete;

		virtual void OnEnter(Scene& scene) override;
		virtual void OnExit(Scene& scene) override;

	private:
		size_t m_SceneIndex{};
	};
}
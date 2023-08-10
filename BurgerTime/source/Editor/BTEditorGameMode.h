#pragma once
#include "State/GameState/GameState.h"

namespace dae
{
	class BTEditorGameMode final : public GameMode
	{
	public:
		BTEditorGameMode();
		virtual ~BTEditorGameMode() = default;

		BTEditorGameMode(const BTEditorGameMode& other) = delete;
		BTEditorGameMode(BTEditorGameMode&& other) = delete;
		BTEditorGameMode& operator=(const BTEditorGameMode& other) = delete;
		BTEditorGameMode& operator=(BTEditorGameMode&& other) = delete;

		virtual void OnEnter() override;
		virtual StatePtr OnUpdate() override;
		virtual void OnExit() override;

	private:
		size_t m_SceneIndex{};
	};
}
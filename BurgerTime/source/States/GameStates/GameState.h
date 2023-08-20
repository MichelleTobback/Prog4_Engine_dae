#pragma once
#include "Scene/Scene.h"

namespace dae
{
	class GameState
	{
	public:
		GameState(size_t sceneIndex)
			: m_SceneIndex{sceneIndex}{}
		virtual ~GameState() = default;

		virtual void OnEnter(Scene& scene) = 0;
		virtual void OnExit(Scene& scene) = 0;

		size_t GetSceneIndex() const { return m_SceneIndex; }

	private:
		size_t m_SceneIndex;
	};
}
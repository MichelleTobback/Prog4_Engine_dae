#pragma once
#include "Platform/ImGui/ImGuiComponent.h"

namespace dae
{
	class RigidBody2DComponent;
	class SceneGraphPanel;
	class BurgerTimeLevelPanel final : public ImGuiComponent
	{
	public:
		BurgerTimeLevelPanel(GameObject* pOwner, SceneGraphPanel* pSceneGraphPanel);
		virtual ~BurgerTimeLevelPanel() = default;

		BurgerTimeLevelPanel(const BurgerTimeLevelPanel& other) = delete;
		BurgerTimeLevelPanel(BurgerTimeLevelPanel&& other) = delete;
		BurgerTimeLevelPanel& operator=(const BurgerTimeLevelPanel& other) = delete;
		BurgerTimeLevelPanel& operator=(BurgerTimeLevelPanel&& other) = delete;

		virtual void Awake() override;

	protected:
		virtual void OnImGuiRender();

	private:
		SceneGraphPanel* m_pSceneGraphPanel;
		GameObject* m_pLevelRoot{ nullptr };
		RigidBody2DComponent* m_pLevelRigid{ nullptr };
	};
}
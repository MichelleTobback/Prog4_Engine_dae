#include "BurgerTimeLevelPanel.h"
#include "Scene/Scene.h"
#include "SceneGraphPanel.h"
#include "Component/Physics/ColliderComponent.h"

#include "Prefabs.h" //LADDER_COLLISION_LAYER

dae::BurgerTimeLevelPanel::BurgerTimeLevelPanel(GameObject* pOwner, SceneGraphPanel* pSceneGraphPanel)
	: ImGuiComponent(pOwner, "BurgerTimeLevelPanel"), m_pSceneGraphPanel{ pSceneGraphPanel }
{
}

void dae::BurgerTimeLevelPanel::Awake()
{
	if (!m_pLevelRoot)
	{
		m_pLevelRoot = GetScene()->Instantiate();
		m_pLevelRigid = m_pLevelRoot->AddComponent<RigidBody2DComponent>(CollisionMode::Static);
	}
}

void dae::BurgerTimeLevelPanel::OnImGuiRender()
{
	GameObject* pNewGameObject{ nullptr };

	if (ImGui::Button("Add Floor"))
	{
		pNewGameObject = GetScene()->Instantiate(m_pLevelRoot);
		auto pShape{ pNewGameObject->AddComponent<QuadComponent>(glm::vec2{28.f, 4.f})};
		auto pCollider{ pNewGameObject->AddComponent<BoxCollider2DComponent>() };
		pCollider->SetShape(pShape);
		pCollider->SetTrigger(true);
		m_pLevelRigid->AddCollider(pCollider);
	}
	if (ImGui::Button("Add Ladder"))
	{
		pNewGameObject = GetScene()->Instantiate(m_pLevelRoot);
		auto pShape{ pNewGameObject->AddComponent<QuadComponent>(glm::vec2{4.f, 32.f}) };
		auto pCollider{ pNewGameObject->AddComponent<BoxCollider2DComponent>() };
		pCollider->SetShape(pShape);
		pCollider->SetTrigger(true);
		pCollider->SetCollisionLayer(LADDER_COLLISION_LAYER);
		m_pLevelRigid->AddCollider(pCollider);
		pNewGameObject->AddTag("Ladder", true);
		pNewGameObject->AddComponent<TagComponent>("Ladder");
	}

	if (pNewGameObject)
	{
		m_pSceneGraphPanel->SelectGameObject(pNewGameObject);
	}
}

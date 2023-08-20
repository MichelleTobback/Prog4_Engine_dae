#include "BurgerTimeLevelPanel.h"
#include "Scene/Scene.h"
#include "SceneGraphPanel.h"
#include "Component/Physics/ColliderComponent.h"
#include "Managers/SceneManager.h"

#include "Component/UI/ButtonComponent.h"
#include "Components/BurgerPlate.h"
#include "Components/NodeComponent.h"
#include "Components/ObjectSpawner.h"

#include "GameManager.h"
#include "States/GameStates/BTGameMode.h"

#include "Prefabs.h" //LADDER_COLLISION_LAYER

dae::BurgerTimeLevelPanel::BurgerTimeLevelPanel(GameObject* pOwner, SceneGraphPanel* pSceneGraphPanel)
	: ImGuiComponent(pOwner, "BurgerTimeLevelPanel"), m_pSceneGraphPanel{ pSceneGraphPanel }
{
}

void dae::BurgerTimeLevelPanel::Awake()
{
	if (!m_pLevelRoot)
	{
		auto lvlObjts{ GetScene()->GetGameObjectWithTag("Lvl") };
		if (lvlObjts.empty())
		{
			m_pLevelRoot = GetScene()->Instantiate();
			m_pLevelRoot->AddTag("Lvl");

			const int levelIdx{ std::stoi(std::string(&GetScene()->GetName().back())) - 1};
			const float width{ 208.f }, height{ 200.f }, spacing{ 8.f };
			GameObject* pObject{ GetScene()->Instantiate(m_pLevelRoot, glm::vec3{spacing, 0.f, 0.f}) };
			TextureComponent* pTexture{ pObject->AddComponent<TextureComponent>() };
			pTexture->SetTexture("Textures/BurgerTimeStages.png");
			SpriteComponent* pSprite{ pObject->AddComponent<SpriteComponent>(pTexture, (width + spacing) * (levelIdx % 3), 2.f * (height + spacing) * (levelIdx / 3), width, height)};
			pObject->AddComponent<SpriteRenderComponent>(pSprite);
		}
		else
			m_pLevelRoot = lvlObjts[0];
		m_pLevelRigid = m_pLevelRoot->HasComponent<RigidBody2DComponent>() 
			? m_pLevelRoot->GetComponent<RigidBody2DComponent>()
			: m_pLevelRoot->AddComponent<RigidBody2DComponent>(CollisionMode::Static);
		CreateGrid();
	}
}

void dae::BurgerTimeLevelPanel::OnImGuiRender()
{
	GameObject* pNewGameObject{ nullptr };

	if (ImGui::BeginCombo("Mode", GetEditorModeName(m_Mode).c_str()))
	{
		for (int mode{ 0 }; mode <= static_cast<int>(EditMode::Last); ++mode)
		{
			EditMode currentMode{ static_cast<EditMode>(mode) };
			bool selected{ m_Mode == currentMode };
			if (ImGui::Selectable(GetEditorModeName(currentMode).c_str(), &selected))
			{
				ClearSelection();
				m_Mode = currentMode;
			}
		}
		ImGui::EndCombo();
	}

	if (m_Mode == EditMode::Select && ImGui::Button("Add Plate"))
	{
		if (!m_pPlatesRigid)
			m_pPlatesRigid = GetScene()->Instantiate()->AddComponent<RigidBody2DComponent>(CollisionMode::Static);
		pNewGameObject = Prefabs::CreateBurgerPlate(GetScene(), m_pPlatesRigid);
		pNewGameObject->GetTransform().Translate(m_SelectedTiles.begin()->second->pos);
	}
	if (m_Mode == EditMode::Select && ImGui::Button("remove Plate"))
	{
		for (auto& tile : m_SelectedTiles)
		{
			if (tile.second->pPlate)
			{
				tile.second->pPlate->GetOwner()->RemoveComponent<BurgerPlate>();
				tile.second->pPlate = nullptr;
			}
		}
	}
	//if (m_Mode == EditMode::Select && ImGui::Button("Add Mr Hotdog"))
	//{
	//	pNewGameObject = Prefabs::CreateEnemy(Prefabs::CreateMrHotDog(GetScene()));
	//	pNewGameObject->GetTransform().Translate(m_SelectedTiles.begin()->second->pos);
	//}

	ImGui::Separator();
	ImGui::Text("character Spawners");

	if (m_Mode == EditMode::Select && ImGui::Button("Add Player Spawner"))
	{
		pNewGameObject = AddSpawner(dae::BurgerTime::SpawnID::Player);
	}
	if (m_Mode == EditMode::Select && ImGui::Button("Add Mr Hotdog Spawner"))
	{
		pNewGameObject = AddSpawner(dae::BurgerTime::SpawnID::MrHotDog);
	}
	if (m_Mode == EditMode::Select && ImGui::Button("Add Mr Egg Spawner"))
	{
		pNewGameObject = AddSpawner(dae::BurgerTime::SpawnID::MrEgg);
	}
	if (m_Mode == EditMode::Select && ImGui::Button("Add Mr Pickle Spawner"))
	{
		pNewGameObject = AddSpawner(dae::BurgerTime::SpawnID::MrPickle);
	}
	ImGui::Separator();
	ImGui::Text("ingredient Spawners");

	if (m_Mode == EditMode::Select && ImGui::Button("Add TopBun Spawner"))
	{
		pNewGameObject = AddSpawner(dae::BurgerTime::SpawnID::TopBun);
	}
	if (m_Mode == EditMode::Select && ImGui::Button("Add BottomBun Spawner"))
	{
		pNewGameObject = AddSpawner(dae::BurgerTime::SpawnID::BottomBun);
	}
	if (m_Mode == EditMode::Select && ImGui::Button("Add Lettuce Spawner"))
	{
		pNewGameObject = AddSpawner(dae::BurgerTime::SpawnID::Lettuce);
	}
	if (m_Mode == EditMode::Select && ImGui::Button("Add Patty Spawner"))
	{
		pNewGameObject = AddSpawner(dae::BurgerTime::SpawnID::Patty);
	}
	if (m_Mode == EditMode::Select && ImGui::Button("Add Cheese Spawner"))
	{
		pNewGameObject = AddSpawner(dae::BurgerTime::SpawnID::Cheese);
	}
	if (m_Mode == EditMode::Select && ImGui::Button("Add Tomato Spawner"))
	{
		pNewGameObject = AddSpawner(dae::BurgerTime::SpawnID::Tomato);
	}

	if (m_Mode == EditMode::Select && ImGui::Button("Spawn All"))
	{
		SpawnAll();
	}

	ImGui::Separator();
	ImGui::Text("edit collider");

	if (m_Mode == EditMode::Select && ImGui::Button("ignore ingredient"))
	{
		for (auto& tile : m_SelectedTiles)
		{
			if (tile.second->pCollider)
				tile.second->pCollider->SetCollisionIgnoreLayer(BurgerTime::INGREDIENT_COLLISION_LAYER, true);
		}
	}

	if (pNewGameObject)
	{
		m_pSceneGraphPanel->SelectGameObject(pNewGameObject);
	}
}

void dae::BurgerTimeLevelPanel::ClearSelection()
{
	auto tiles{ m_SelectedTiles };
	for (auto& tile : tiles)
		tile.second->pButton->Deselect();
	m_SelectedTiles.clear();
}

std::string dae::BurgerTimeLevelPanel::GetEditorModeName(EditMode mode)
{
	std::string name{};
	switch (mode)
	{
	case EditMode::Select:
		name = "Select";
		break;

	case EditMode::AddCollider:
		name = "AddCollider";
	break;

	case EditMode::AddPath:
		name = "AddPath";
	break;
	}
	return name;
}

void dae::BurgerTimeLevelPanel::CreateGrid()
{
	const glm::vec2 size{ 208.f, 200.f };
	const float tileSize{ 8.f };
	glm::ivec2 gridSize{ static_cast<glm::ivec2>(size / tileSize) };
	gridSize.x += 2;
	++gridSize.y;
	m_pGrid.resize(static_cast<int>((gridSize.x) * gridSize.y));

	std::vector<GameObject*> pLevelChildren{m_pLevelRoot->GetChildren()};

	for (int y{}; y < gridSize.y; ++y)
	{
		for (int x{}; x < gridSize.x; ++x)
		{
			int index{ y * gridSize.x + x };
			glm::vec3 tilePos{ x * tileSize, y * tileSize, 0.f };
			//editor
			GameObject* pButtonObject{ GetScene()->Instantiate(GetOwner(), tilePos)};
			ButtonComponent* pButton{ pButtonObject->AddComponent<ButtonComponent>(glm::vec2{tileSize, tileSize}) };
			m_pGrid[index].pButton = pButton;
			m_pGrid[index].pos = tilePos;

			//map objects + components if serialized
			const float epsilonSqrt{ 1.f };
			for (auto pObject : pLevelChildren)
			{
				const glm::vec3 currentPos{ pObject->GetTransform().GetWorldPosition() };
				const float distanceSqrt{ glm::distance2(tilePos, currentPos) };

				if (distanceSqrt < epsilonSqrt)
				{
					m_pGrid[index].pObject = pObject;
					if (pObject->HasComponent<BoxCollider2DComponent>())
						m_pGrid[index].pCollider = pObject->GetComponent<BoxCollider2DComponent>();
					if (pObject->HasComponent<BurgerPlate>())
						m_pGrid[index].pPlate = pObject->GetComponent<BurgerPlate>();
					if (pObject->HasComponent<NodeComponent>())
						m_pGrid[index].pNode = pObject->GetComponent<NodeComponent>();
					if (pObject->HasComponent<ObjectSpawner>())
					{
						m_pGrid[index].pSpawner = pObject->GetComponent<ObjectSpawner>();
						m_Spawners.push_back(static_cast<size_t>(index));
					}

					pLevelChildren.erase(std::remove(pLevelChildren.begin(), pLevelChildren.end(), pObject), pLevelChildren.end());
				}
			}

			pButton->GetQuadRenderer()->SetLayer(4);
			pButton->EnableToggle(true);
			UIButtonLayout layout{};
			layout.color = glm::vec4{ 0.f, 1.f, 0.f, 1.f };
			pButton->SetLayout(layout, UIButtonFlag::Selected);
			layout.color = glm::vec4{ 0.f, 0.5f, 0.7f, 1.f };
			pButton->SetLayout(layout, UIButtonFlag::Hovered);
			pButton->GetOnPressedDelegate() += [this, x, y, gridSize, tileSize]()->void
			{
				int index{ y * gridSize.x + x };
				
				switch (m_Mode)
				{
				case EditMode::Select:
				{
					ClearSelection();

					m_SelectedTiles[index] = &m_pGrid[index];
				}
					break;
				
				case EditMode::AddCollider:
				{
					if (m_SelectedTiles.size() > 1)
					{
						ClearSelection();
					}
					if (m_SelectedTiles.size() == 0)
						m_SelectedTiles[index] = &m_pGrid[index];
					else if (m_SelectedTiles.size() == 1)
					{
						int firstIndex{ m_SelectedTiles.begin()->first };
						if (!m_pGrid[firstIndex])
						{
							m_pGrid[firstIndex].pObject = GetScene()->Instantiate(m_pLevelRoot, m_pGrid[firstIndex].pos);
						}
						auto& firstTile{ m_SelectedTiles.begin()->second };

						glm::vec3 endPos{ m_pGrid[index].pos};
						endPos.x += tileSize;
						endPos.y += tileSize;
						glm::vec3 startPos{ firstTile->pos};
						glm::vec2 colliderSize{ endPos.x - startPos.x, endPos.y - startPos.y };

						auto pShape{ firstTile->pObject->AddComponent<QuadComponent>(colliderSize)};
						firstTile->pCollider = firstTile->pObject->AddComponent<BoxCollider2DComponent>();
						firstTile->pCollider->SetShape(pShape);
						firstTile->pCollider->SetTrigger(false);
						m_pLevelRigid->AddCollider(firstTile->pCollider);

						ClearSelection();
					}
				}
				break;
				
				case EditMode::AddPath:
				{
					if (!m_pGrid[index])
					{
						m_pGrid[index].pObject = GetScene()->Instantiate(m_pLevelRoot, m_pGrid[index].pos);
					}
					if (!m_pGrid[index].pNode)
					{
						m_pGrid[index].pNode = m_pGrid[index].pObject->AddComponent<NodeComponent>();
					}
					if (m_pGrid[index - 1].pNode) //left
					{
						m_pGrid[index].pNode->AddEdge(m_pGrid[index - 1].pNode, false);
					}
					if (m_pGrid[index + 1].pNode) //right
					{
						m_pGrid[index].pNode->AddEdge(m_pGrid[index + 1].pNode, false);
					}
					if (m_pGrid[index - gridSize.x].pNode) //top
					{
						m_pGrid[index].pNode->AddEdge(m_pGrid[index - gridSize.x].pNode, false);
					}
					if (m_pGrid[index + gridSize.x].pNode) //bottom
					{
						m_pGrid[index].pNode->AddEdge(m_pGrid[index + gridSize.x].pNode, false);
					}
				}
				break;
				}
			};

			pButton->GetOnReleasedDelegate() += [this, index]()->void
			{
				m_SelectedTiles.erase(index);
			};
		}
	}
}

dae::GameObject* dae::BurgerTimeLevelPanel::AddSpawner(BurgerTime::SpawnID id)
{
	if (!m_SelectedTiles.begin()->second->pObject)
	{
		m_SelectedTiles.begin()->second->pObject = GetScene()->Instantiate(m_pLevelRoot, m_SelectedTiles.begin()->second->pos);
	}

	GameObject* pNewGameObject{ m_SelectedTiles.begin()->second->pObject };
	m_SelectedTiles.begin()->second->pSpawner = pNewGameObject->AddComponent<ObjectSpawner>(static_cast<uint32_t>(id));
	m_Spawners.push_back(static_cast<size_t>(m_SelectedTiles.begin()->first));

	return pNewGameObject;
}

void dae::BurgerTimeLevelPanel::SpawnAll()
{
	BTGameMode* pGameMode{ dynamic_cast<BTGameMode*>(&GameManager::GetInstance().GetState()) };
	if (pGameMode)
	{
		pGameMode->OnPlayerLostLife();
	}
}

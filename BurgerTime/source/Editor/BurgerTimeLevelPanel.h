#pragma once
#include "Platform/ImGui/ImGuiComponent.h"

#include "BurgerTime.h"

#include <map>
#include <glm/glm.hpp>

namespace dae
{
	class ObjectSpawner;
	class NodeComponent;
	class BurgerPlate;
	class BoxCollider2DComponent;
	class GameObject;
	class ButtonComponent;
	struct BurgerTimeTile
	{
		GameObject* pObject{nullptr};
		ButtonComponent* pButton{ nullptr };
		BoxCollider2DComponent* pCollider{ nullptr };
		BurgerPlate* pPlate{ nullptr };
		NodeComponent* pNode{ nullptr };
		ObjectSpawner* pSpawner{ nullptr };
		glm::vec3 pos{};

		operator bool() const
		{
			return pObject;
		}
	};

	class RigidBody2DComponent;
	class SceneGraphPanel;
	class BurgerTimeLevelPanel final : public ImGuiComponent
	{
	public:
		enum class EditMode
		{
			Select, AddCollider, AddPath, 
			Last = AddPath
		};

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
		void ClearSelection();
		std::string GetEditorModeName(EditMode mode);
		void CreateGrid();
		GameObject* AddSpawner(BurgerTime::SpawnID id);
		void SpawnAll();

		std::vector<BurgerTimeTile> m_pGrid{};
		SceneGraphPanel* m_pSceneGraphPanel;
		GameObject* m_pLevelRoot{ nullptr };
		RigidBody2DComponent* m_pLevelRigid{ nullptr };
		RigidBody2DComponent* m_pPlatesRigid{ nullptr };

		EditMode m_Mode{ EditMode::Select };

		std::map<int, BurgerTimeTile*> m_SelectedTiles;
		std::vector<size_t> m_Spawners;
	};
}
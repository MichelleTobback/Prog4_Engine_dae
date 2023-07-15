#pragma once
#include "Platform/ImGui/ImGuiComponent.h"
#include "Component/Physics/RigidBody2DComponent.h"
#include "Core/UUID.h"

namespace dae
{
	class SpriteComponent;
	class Scene;
	class SceneGraphPanel final : public ImGuiComponent
	{
	public:
		SceneGraphPanel(GameObject* pOwner);
		virtual ~SceneGraphPanel() = default;

		SceneGraphPanel(const SceneGraphPanel& other) = delete;
		SceneGraphPanel(SceneGraphPanel&& other) = delete;
		SceneGraphPanel& operator=(const SceneGraphPanel& other) = delete;
		SceneGraphPanel& operator=(SceneGraphPanel&& other) = delete;

		void SelectGameObject(GameObject* pGameObject);

	protected:
		virtual void OnImGuiRender();

	private:
		void RenderGameObjectNode(GameObject* pGameObject);
		void AddGameObjectPopUp(Scene* pScene, GameObject* pParent);
		void DrawComponents(GameObject* pGameObject);
		void DrawSpriteComponent(GameObject* pSprite, int index = 0);
		void DrawQuadComponent(GameObject* pGameObject);
		void DrawRigidBodyComponent(GameObject* pGameObject);
		void DrawBoxCollider2DComponent(GameObject* pGameObject);
		std::string GetCollisionModeName(CollisionMode mode);
		void AddComponentsPopUp(GameObject* pGameObject);
		void DrawTextureComponent(GameObject* pTexture);

		GameObject* m_SelectedGameObject{ 0 };
	};
}

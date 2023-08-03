#pragma once
#include <string>

#include "Core/Delegate.h"
#include "Scene/Scene.h"
#include "Scene/GameObject.h"
#include "Components/BurgerIngredient.h"
#include "Component/Physics/ColliderComponent.h"

#include "Components/CharacterInfo.h"

namespace dae
{
	class SpriteAtlasComponent;
	class Prefabs final
	{
	public:
		//UI
		static GameObject* CreateScoreHUD(GameObject* pHudObject, ObservableType<uint32_t>& value, const std::string& text, const glm::vec4& color);
		static GameObject* CreatePlayerLifesHUD(SpriteAtlasComponent* pSpriteAtlas, HealthComponent* pPlayerHealth);
		static GameObject* CreateSinglePlayerHUD(GameObject* pLevelObject, GameObject* pPlayer);

		//Scene
		static GameObject* CreateEnemy(GameObject* pObject);
		static GameObject* CreateMrHotDog(Scene* pScene);
		static GameObject* CreateBurgerPlate(Scene* pScene, RigidBody2DComponent* pRigidBody);
		static GameObject* CreateBurgerIngredient(Scene* pScene, BurgerIngredient::IngredientType type);
		static GameObject* CreatePeterPepper(Scene* pScene);
		static GameObject* CreateTestLevel(Scene* pScene);

	private:
		Prefabs() = default;
	};
}
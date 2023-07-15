#pragma once
#include <string>

#include "Scene/Scene.h"
#include "Scene/GameObject.h"
#include "Component/Physics/ColliderComponent.h"

namespace dae
{
	constexpr CollisionLayer LADDER_COLLISION_LAYER = CollisionLayer::Three;

	class Prefabs final
	{
	public:
		static GameObject* CreatePeterPepper(Scene* pScene);

		static GameObject* CreateTestLevel(Scene* pScene);

	private:
		Prefabs() = default;
	};
}
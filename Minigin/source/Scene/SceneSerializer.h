#pragma once
#include <filesystem>

#include "Core/BinaryReader.h"
#include "Core/UUID.h"
#include "Core/Subject.h"
#include "Scene/Scene.h"
#include "Core/Delegate.h"
#include "Serializable.h"

#include <functional>

namespace dae
{
	class GameObject;

	class SceneSerializer final
	{
	public:
		SceneSerializer(const SceneSerializer& other) = delete;
		SceneSerializer(SceneSerializer&& other) = delete;
		SceneSerializer& operator=(const SceneSerializer& other) = delete;
		SceneSerializer& operator=(SceneSerializer&& other) = delete;

		void Serialize(Scene* pScene, const std::filesystem::path& path);
		void Deserialize(Scene* pScene, const std::filesystem::path& path);

	private:
		friend class SceneManager;
		SceneSerializer() = default;

		void SerializeGameObject(GameObject* pGameObject, BinaryWriter& out, ComponentFactory& factory);
		//instantiates the gameobject if not already deserialized
		GameObject* GetDeserializedGameObject(UUID uuid, UUID parent = 0u);
		template<typename Fn>
		void DeserializeComponentOnRefInstantiated(UUID gameObject, const std::vector<std::string>& components, Fn fn);

		Scene* m_pScene{};
		int m_SerializableObjects{};

		Subject m_GameObjectDeserialized{};
		std::list<std::unique_ptr<OnGameObjectDeserialized>> m_OnDeserialized{};
		static void RegisterEngineComponents(ComponentFactory& factory);
	};
}

template<typename Fn>
void dae::SceneSerializer::DeserializeComponentOnRefInstantiated(dae::UUID gameObject, const std::vector<std::string>& components, Fn fn)
{
	auto pOnDeserialized{ std::make_unique<OnGameObjectDeserialized>(gameObject, m_pScene, components, fn) };
	m_OnDeserialized.push_back(std::move(pOnDeserialized));
	m_GameObjectDeserialized.AddObserver(m_OnDeserialized.back().get());
}
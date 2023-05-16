#pragma once
#include <filesystem>

#include "Core/BinaryReader.h"
#include "Core/UUID.h"
#include "Core/Subject.h"
#include "Scene/Scene.h"

#include <functional>

namespace dae
{
	class GameObject;
	using OnGameObjectDeserializedFn = std::function<bool(GameObject*, Scene*)>;
	class OnGameObjectDeserialized final : public Observer
	{
	public:
		OnGameObjectDeserialized(UUID uuid, Scene* pScene, OnGameObjectDeserializedFn fn);
		virtual ~OnGameObjectDeserialized() = default;

		virtual void Invoke(const Event& event, Subject* pSubject) override;

	private:
		UUID m_GameObject{};
		Scene* m_pScene;
		OnGameObjectDeserializedFn m_Func;
	};

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

		void SerializeGameObject(GameObject* pGameObject, BinaryWriter& out);
		//instantiates the gameobject if not already deserialized
		GameObject* GetDeserializedGameObject(UUID uuid, UUID parent = 0u);
		template<typename Fn>
		void DeserializeComponentOnRefInstantiated(UUID gameObject, Fn fn);

		Scene* m_pScene;

		Subject m_GameObjectDeserialized{};
		std::list<OnGameObjectDeserialized> m_OnDeserialized{};
	};
}

template<typename Fn>
void dae::SceneSerializer::DeserializeComponentOnRefInstantiated(dae::UUID gameObject, Fn fn)
{
	OnGameObjectDeserialized observer{ gameObject, m_pScene, fn };
	if (m_pScene->GetGameObject(gameObject) != nullptr)
	{
		Event event{};
		uint64_t obj{ static_cast<uint64_t>(gameObject) };
		event.SetData(obj);
		observer.Invoke(event, &m_GameObjectDeserialized);
	}
	else
	{
		m_OnDeserialized.push_back(observer);
		m_GameObjectDeserialized.AddObserver(&m_OnDeserialized.back());
	}
}
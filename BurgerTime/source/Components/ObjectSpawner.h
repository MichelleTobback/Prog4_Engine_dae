#pragma once
#include "Component/Component.h"
#include "Scene/Serializable.h"

#include <memory>
#include <unordered_map>
#include <functional>
#include "Core/Delegate.h"

namespace dae
{
	using SpawnFunc = std::function<GameObject* (Scene* pScene)>;
	class ObjectSpawner final : public Component
	{
	public:
		ObjectSpawner(GameObject* pOwner, uint32_t spawnId);
		virtual ~ObjectSpawner() = default;

		ObjectSpawner(const ObjectSpawner& other) = delete;
		ObjectSpawner(ObjectSpawner&& other) = delete;
		ObjectSpawner& operator=(const ObjectSpawner& other) = delete;
		ObjectSpawner& operator=(ObjectSpawner&& other) = delete;

		virtual void Awake() override;
		virtual void Sleep() override;
		virtual void Update() override;

		GameObject* Spawn();
		void SpawnDelayed(float time);
		bool IsRegistered() const;
		uint32_t GetSpawnID() const;
		GameObject* GetInstance();
		Delegate<void(GameObject*)>& GetOnObjectSpawnedDelegate() { return *m_pOnObjectSpawned; }

		static void Register(uint32_t id, const SpawnFunc& fn);

	private:
		GameObject* m_pObjectInstance{ nullptr };
		uint32_t m_Id;
		float m_WaitTime{};

		static std::unordered_map<uint32_t, SpawnFunc> s_SpawnFuncMap;
		std::unique_ptr<Delegate<void(GameObject*)>> m_pOnObjectSpawned;
	};

	//===============================//
	//		Serializable			 //
	//===============================//

	class ObjectSpawnerComponentSerializer final : public Serializable
	{
	public:
		ObjectSpawnerComponentSerializer();
		virtual ~ObjectSpawnerComponentSerializer() override = default;

		virtual void Serialize(BinaryWriter& out, Component* pComponent) const override;
		virtual Component* Deserialize(DeserializeParams& params) override;
	};
}
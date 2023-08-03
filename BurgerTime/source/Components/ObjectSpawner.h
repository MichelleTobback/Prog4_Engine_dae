#pragma once
#include "Component/Component.h"
#include "Scene/Serializable.h"

#include <unordered_map>
#include <functional>

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

		GameObject* Spawn();
		bool IsRegistered() const;
		uint32_t GetSpawnID() const;

		GameObject* GetInstance();

		static void Register(uint32_t id, const SpawnFunc& fn);

	private:
		GameObject* m_pObjectInstance{ nullptr };
		uint32_t m_Id;

		static std::unordered_map<uint32_t, SpawnFunc> s_SpawnFuncMap;
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
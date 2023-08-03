#include "ObjectSpawner.h"
#include "Scene/GameObject.h"

std::unordered_map<uint32_t, dae::SpawnFunc> dae::ObjectSpawner::s_SpawnFuncMap;

dae::ObjectSpawner::ObjectSpawner(GameObject* pOwner, uint32_t spawnId)
	: Component(pOwner), m_Id{spawnId}
{
	m_Id = m_Id;
}

dae::GameObject* dae::ObjectSpawner::Spawn()
{
	if (!m_pObjectInstance && IsRegistered())
	{
		m_pObjectInstance = s_SpawnFuncMap[m_Id](GetScene());
		m_pObjectInstance->GetOnDestroyed() += [this](GameObject*)
		{
			m_pObjectInstance = nullptr;
		};
	}
	if (m_pObjectInstance)
	{
		m_pObjectInstance->GetTransform().SetLocalPosition(GetTransform().GetWorldPosition());

		if (!m_pObjectInstance->IsActive())
			m_pObjectInstance->SetActive(true);
	}

	return m_pObjectInstance;
}

bool dae::ObjectSpawner::IsRegistered() const
{
	return s_SpawnFuncMap.find(m_Id) != s_SpawnFuncMap.end();
}

uint32_t dae::ObjectSpawner::GetSpawnID() const
{
	return m_Id;
}

dae::GameObject* dae::ObjectSpawner::GetInstance()
{
	return m_pObjectInstance;
}

void dae::ObjectSpawner::Register(uint32_t id, const SpawnFunc& fn)
{
	s_SpawnFuncMap[id] = fn;
}

//===============================//
//		Serializable			 //
//===============================//

dae::ObjectSpawnerComponentSerializer::ObjectSpawnerComponentSerializer()
	: Serializable(Serializable::Create<ObjectSpawner>())
{
}

void dae::ObjectSpawnerComponentSerializer::Serialize(BinaryWriter& out, Component* pComponent) const
{
	ObjectSpawner* pNode{ pComponent->As<ObjectSpawner>() };
	out.Write(pNode->GetSpawnID());
}

dae::Component* dae::ObjectSpawnerComponentSerializer::Deserialize(DeserializeParams& params)
{
	if (params.pGameObject)
	{
		uint32_t spawnId;
		params.in.Read(spawnId);

		ObjectSpawner* pInstance{ params.pGameObject->AddComponent<ObjectSpawner>(spawnId) };
		return pInstance;
	}

	return nullptr;
}
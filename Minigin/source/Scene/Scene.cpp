#include "Scene.h"
#include "GameObject.h"

#include "Component/TransformComponent.h"

using namespace dae;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name) : m_Name(name), m_pPhysics{std::make_unique<PhysicsScene>()} {}

GameObject* dae::Scene::GetGameObject(UUID uuid)
{
	if (uuid.IsValid() && m_Objects.find(uuid) != m_Objects.end())
	{
		return m_Objects.at(uuid).get();
	}
	return nullptr;
}

Scene::~Scene() = default;

GameObject* Scene::Add(std::shared_ptr<GameObject> object)
{
	//m_Objects.emplace_back(std::move(object));
	//return m_Objects.back().get();

	UUID uuid{ object->GetUUID() };
	m_Objects.emplace(uuid, std::move(object));
	return m_Objects[uuid].get();
}

GameObject* dae::Scene::Instantiate(GameObject* pParent, const glm::vec3& pos)
{
	auto pGameObject{ Add(std::make_shared<GameObject>(this)) };
	if (pParent)
		pGameObject->AttachToGameObject(pParent);
	pGameObject->GetTransform().SetLocalPosition(pos);
	return pGameObject;
}

GameObject* dae::Scene::Instantiate(UUID uuid, GameObject* pParent, const glm::vec3& pos)
{
	auto pGameObject{ Add(std::make_shared<GameObject>(this, uuid)) };
	if (pParent)
		pGameObject->AttachToGameObject(pParent);
	pGameObject->GetTransform().SetLocalPosition(pos);
	return pGameObject;
}


void Scene::Remove(std::shared_ptr<GameObject> object)
{
	//m_Objects.erase(std::remove(m_Objects.begin(), m_Objects.end(), object), m_Objects.end());
	m_Objects.erase(object->GetUUID());
}

void Scene::RemoveAll()
{
	m_Objects.clear();
}

void Scene::Update()
{
	for(auto& [uuid, pGameObject] : m_Objects)
	{
		pGameObject->Update();
	}
}

void dae::Scene::FixedUpdate()
{
	for (auto& [uuid, pGameObject] : m_Objects)
	{
		pGameObject->FixedUpdate();
	}
}

void dae::Scene::LateUpdate()
{
	for (auto& [uuid, pGameObject] : m_Objects)
	{
		pGameObject->LateUpdate();

		if (!pGameObject->IsValid())
			m_ObjectsPendingDestroy.push(uuid);
	}
}

void dae::Scene::HandleObjectLifeTime()
{
	while (!m_ObjectsPendingDestroy.empty())
	{
		Remove(m_Objects[m_ObjectsPendingDestroy.top()]);
		m_ObjectsPendingDestroy.pop();
	}
}


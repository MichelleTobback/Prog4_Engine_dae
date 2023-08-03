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

std::vector<GameObject*> dae::Scene::GetGameObjectWithTag(const std::string& tag)
{
	std::vector<GameObject*> tags {};
	for (auto& [uuid, pGameObject] : m_Objects)
	{
		if (pGameObject &&pGameObject->HasTag(tag))
			tags.push_back(pGameObject.get());
	}
	return tags;
}

std::vector<GameObject*> dae::Scene::GetGameObjectsAtPos(const glm::vec3& pos, float epsilon, bool onlyRoot)
{
	std::vector<GameObject*> pObjects;
	const float epsilonSqrt{ epsilon * epsilon };
	for (auto& pObject : m_Objects)
	{
		if (onlyRoot && !pObject.second->IsRoot())
			continue;
		const glm::vec3 currentPos{pObject.second->GetTransform().GetWorldPosition()};
		const float distanceSqrt{ glm::distance2(pos, currentPos) };

		if (distanceSqrt < epsilonSqrt)
			pObjects.push_back(pObject.second.get());
	}
	return pObjects;
}

void dae::Scene::DestroyGameObject(GameObject* pObject)
{
	pObject->SetActive(false);
	m_ObjectsPendingDestroy.push(pObject->GetUUID());
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
	pGameObject->SetActive(true);
	return pGameObject;
}

GameObject* dae::Scene::Instantiate(UUID uuid, GameObject* pParent, const glm::vec3& pos)
{
	auto pGameObject{ Add(std::make_shared<GameObject>(this, uuid)) };
	if (pParent)
		pGameObject->AttachToGameObject(pParent);
	pGameObject->GetTransform().SetLocalPosition(pos);
	pGameObject->SetActive(true);
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

void dae::Scene::Awake()
{
	for (auto& [uuid, pGameObject] : m_Objects)
	{
		if (pGameObject->IsRoot())
			pGameObject->SetActive(true);
	}
}

void dae::Scene::Sleep()
{
	for (auto& [uuid, pGameObject] : m_Objects)
	{
		if (pGameObject->IsRoot())
			pGameObject->SetActive(false);
	}
}

void Scene::Update()
{
	for(auto& [uuid, pGameObject] : m_Objects)
	{
		if (pGameObject->IsActive())
			pGameObject->Update();
	}
}

void dae::Scene::FixedUpdate()
{
	for (auto& [uuid, pGameObject] : m_Objects)
	{
		if (pGameObject->IsActive())
			pGameObject->FixedUpdate();
	}
}

void dae::Scene::LateUpdate()
{
	for (auto& [uuid, pGameObject] : m_Objects)
	{
		if (pGameObject->IsActive())
			pGameObject->LateUpdate();
	}
}

void dae::Scene::HandleObjectLifeTime()
{
	while (!m_ObjectsPendingDestroy.empty())
	{
		auto pObject{ m_Objects[m_ObjectsPendingDestroy.top()] };
		if (pObject)
		{
			pObject->OnDestroy();
		}
		m_Objects.erase(m_ObjectsPendingDestroy.top());
		m_ObjectsPendingDestroy.pop();
	}
}


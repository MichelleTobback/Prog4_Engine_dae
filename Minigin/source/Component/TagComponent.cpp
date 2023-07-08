#include "TagComponent.h"
#include "Scene/GameObject.h"

dae::TagComponent::TagComponent(GameObject* pOwner, const std::string& tag)
	: TagComponent(pOwner, std::vector<std::string>{tag})
{
}

dae::TagComponent::TagComponent(GameObject* pOwner, const std::vector<std::string>& tags)
	: Component(pOwner), m_Tags{tags}
{
}

void dae::TagComponent::AddTag(const std::string& tag, bool includeChildren)
{
	if (!HasTag(tag))
		m_Tags.push_back(tag);

	if (includeChildren)
	{
		auto pChildren{ GetOwner()->GetChildren() };
		for (auto& pChild : pChildren)
		{
			pChild->AddTag(tag, true);
		}
	}
}

void dae::TagComponent::RemoveTag(const std::string& tag)
{
	m_Tags.erase(std::remove(m_Tags.begin(), m_Tags.end(), tag), m_Tags.end());
}

bool dae::TagComponent::HasTag(const std::string& tag) const
{
	return m_Tags.size() > 0 && std::find(m_Tags.begin(), m_Tags.end(), tag) != m_Tags.end();
}

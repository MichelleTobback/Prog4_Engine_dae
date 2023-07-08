#pragma once
#include "Component/Component.h"

#include <vector>

namespace dae
{
	class TagComponent : public Component
	{
	public:
		TagComponent(GameObject* pOwner, const std::string& tag);
		TagComponent(GameObject* pOwner, const std::vector<std::string>& tags = {});
		virtual ~TagComponent() = default;

		TagComponent(const TagComponent& other) = delete;
		TagComponent(TagComponent&& other) = delete;
		TagComponent& operator=(const TagComponent& other) = delete;
		TagComponent& operator=(TagComponent&& other) = delete;

		void AddTag(const std::string& tag, bool includeChildren = true);
		void RemoveTag(const std::string& tag);
		bool HasTag(const std::string& tag) const;

		const std::vector<std::string>& GetAll() const { return m_Tags; }

	private:
		std::vector<std::string> m_Tags{};
	};
}
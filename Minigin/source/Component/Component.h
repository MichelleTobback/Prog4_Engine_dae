#pragma once
#include "Core/UUID.h"

#include <memory>
#include <string>

namespace dae
{
	class Scene;
	class GameObject;
	class Component;
	class TransformComponent;

	struct ComponentMessage
	{
		Component* pMessengerComponent{ nullptr };
		std::string Message;
	};

	class Component
	{
	public:
		Component(GameObject* pGameObject);
		virtual ~Component(){}

		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

		virtual void Awake(){}
		virtual void OnDestroy() {}
		virtual void ReceiveMessage(const ComponentMessage&){}
		virtual void Update(){}
		virtual void FixedUpdate(){}
		virtual void LateUpdate() {}

		TransformComponent& GetTransform();

		UUID GetUUID() const;

		inline GameObject* GetOwner() const { return m_pOwner; }
		Scene* GetScene() const;

		template <typename T>
		T* As()
		{
			//static_assert(std::is_base_of<Component, T>::value, "T must derive from Component!");
			T* ptr{ dynamic_cast<T*>(this) };
			return ptr;
		}

		template <typename T>
		bool Is() const;
		template <typename T>
		static std::string GetName();

		std::string GetName() const;

	private:
		GameObject* m_pOwner;
	};

	template<typename T>
	inline bool Component::Is() const
	{
		return std::strcmp(Component::GetName<T>().c_str(), typeid(*this).name()) == 0;
	}
	template<typename T>
	inline std::string Component::GetName()
	{
		return typeid(T).name();
	}
}
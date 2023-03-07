#pragma once
#include <memory>
#include <string>

namespace dae
{
	class GameObject;
	class Component;

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

		virtual void ReceiveMessage(const ComponentMessage&){}
		virtual void Update(){}
		virtual void FixedUpdate(){}
		virtual void LateUpdate() {}

		inline GameObject* GetOwner() const { return m_pOwner; }

		template <typename T>
		T* As()
		{
			//static_assert(std::is_base_of<Component, T>::value, "T must derived from Component!");
			T* ptr{ dynamic_cast<T*>(this) };
			return ptr;
		}

	private:
		GameObject* m_pOwner;
	};
}
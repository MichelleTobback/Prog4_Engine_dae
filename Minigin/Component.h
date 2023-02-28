#pragma once

namespace dae
{
	class GameObject;

	typedef int CompMessageInfo;
	constexpr CompMessageInfo COMPMSG_DEFAULT = 0;

	class Component;

	struct ComponentMessage
	{
		CompMessageInfo MessageCode{ COMPMSG_DEFAULT };
		Component* pMessengerComponent{ nullptr };
	};

	class Component
	{
	public:
		Component() = default;
		virtual ~Component(){}

		virtual void ReceiveMessage(const ComponentMessage& message);
		virtual void Update(GameObject* pGameObject);
		virtual void FixedUpdate(GameObject* pGameObject);
		virtual void LateUpdate(GameObject* pGameObject);

		template <typename T>
		T& As()
		{
			//static_assert(std::is_base_of<Component, T>::value, "T must derived from Component!");
			T* ptr{ dynamic_cast<T*>(this) };
			return *ptr;
		}

	private:

	};
}
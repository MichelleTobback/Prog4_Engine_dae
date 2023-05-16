#pragma once
#include "Component/Component.h"

namespace dae
{
	class GameObject;
	class TankCanon : public Component
	{
	public:
		TankCanon(GameObject* pOwner);
		~TankCanon() = default;

		void Shoot();

	private:
		GameObject* m_pSocket{};
	};
}
#pragma once
#include "Component/Component.h"
#include "Audio/AudioClip.h"

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
		std::unique_ptr<AudioClip> m_pShootSound;
	};
}
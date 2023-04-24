#pragma once
#include "Singleton.h"

#include <steam_api.h>


namespace dae
{
	class SteamIntegration final : public Singleton<SteamIntegration>
	{
	public:
		SteamIntegration() = default;
		virtual ~SteamIntegration() = default;

		void Init();
		void Shutdown();
		void Update();

		inline bool IsActive() const { return m_Initialized; }

	private:
		bool m_Initialized{ false };
	};
}
#include "SteamIntegration.h"
#include <iostream>

void dae::SteamIntegration::Init()
{
	if (!m_Initialized)
	{
		if (!SteamAPI_Init())
		{
			std::cout << "Steam must be running to play this game (SteamAPI_Init() failed!)\n";
			return;
		}
		m_Initialized = true;
		std::cout << "Successfully inialized steam.\n";
	}
}

void dae::SteamIntegration::Shutdown()
{
	if (m_Initialized)
	{
		SteamAPI_Shutdown();
	}
}

void dae::SteamIntegration::Update()
{
	SteamAPI_RunCallbacks();
}

#include "Keyboard.h"
#include <SDL.h>
#include <backends/imgui_impl_sdl2.h>

dae::Keyboard::Keyboard()
{

}

dae::Keyboard::~Keyboard()
{
}

void dae::Keyboard::Update()
{
	for (auto& key : m_KeysToRelease)
	{
		m_KeysPressed.erase(key);
	}
	m_KeysToRelease.clear();
}

bool dae::Keyboard::ProcessInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {

		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {

		}
		// etc...
		// 
		//ImGui Input
		ImGui_ImplSDL2_ProcessEvent(&e);
	}
	return true;
}

bool dae::Keyboard::IsKeyPressed(KeyCode key)
{
	bool isDown{ IsKeyDown(key) };
	if (!WasDown(key))
	{
		if (isDown)
		{
			SetKeyDown(key);
			return true;
		}
	}
	else if (!isDown)
	{
		SetKeyReleased(key);
	}

	return false;
}

bool dae::Keyboard::IsKeyUp(KeyCode key)
{
	bool isDown{ IsKeyDown(key) };
	if (WasDown(key))
	{
		if (!isDown)
		{
			SetKeyReleased(key);
			return true;
		}
	}
	else if (isDown)
		SetKeyDown(key);


	return false;
}

bool dae::Keyboard::IsKeyDown(KeyCode key)
{
	auto keys{ SDL_GetKeyboardState(NULL) };

	int buttonCode{ static_cast<int>(key) };
	return keys[buttonCode];
}

bool dae::Keyboard::WasDown(KeyCode key)
{
	return m_KeysPressed.find(key) != m_KeysPressed.end();
}

void dae::Keyboard::SetKeyDown(KeyCode key)
{
	m_KeysPressed[key] = key;
}

void dae::Keyboard::SetKeyReleased(KeyCode key)
{
	m_KeysToRelease.insert(key);
}

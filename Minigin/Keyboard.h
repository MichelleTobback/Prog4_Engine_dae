#pragma once
#include <memory>

#include <SDL.h>

namespace dae
{
	class Keyboard final
	{
	public:
		enum class KeyButtonState
		{
			Down, Released, Pressed
		};

		explicit Keyboard();
		~Keyboard();

		void Update();

	private:
	};
}
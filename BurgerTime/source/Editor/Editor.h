#pragma once

namespace dae
{
	class Editor final
	{
	public:
		Editor();
		~Editor() = default;

		Editor(const Editor& other) = delete;
		Editor(Editor&& other) = delete;
		Editor& operator=(const Editor& other) = delete;
		Editor& operator=(Editor&& other) = delete;

		void Init();
	private:

	};
}
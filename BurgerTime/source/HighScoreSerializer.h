#pragma once
#include <tuple>
#include <string>
#include <array>
#include <unordered_map>

namespace dae
{
	namespace SaveGame
	{
		constexpr uint32_t NUM_HISCORES{ 10 };

		struct HighScore
		{
			uint32_t hiScore{};
			unsigned char nameIndex{};
		};
	};

	struct GameSaveData
	{
		std::array<SaveGame::HighScore, SaveGame::NUM_HISCORES> hiScores{};
		std::vector<std::string> names;
	};

	using HighScore = std::pair<std::string, size_t>;
	class GameDataSerializer final
	{
	public:
		GameDataSerializer() = default;

		void Serialize(const GameSaveData& data);
		void Deserialize(GameSaveData& data);
	};
}
#include "HighScoreSerializer.h"

#include "Core/BinaryReader.h"

void dae::GameDataSerializer::Serialize(const GameSaveData& data)
{
	if (BinaryWriter writer{ "Content/SaveGame.bin" }; writer)
	{
		writer.Write(static_cast<unsigned char>(data.names.size()));
		for (const auto& name : data.names)
		{
			writer.WriteString(name);
		}
		writer.WriteArray(&data.hiScores[0], data.hiScores.size());
	}
}

void dae::GameDataSerializer::Deserialize(GameSaveData& data)
{
	if (BinaryReader reader{ "Content/SaveGame.bin" }; reader)
	{
		unsigned char numNames{};
		reader.Read(numNames);
		size_t namesSize{ static_cast<size_t>(numNames) };
		data.names.resize(namesSize);
		for (size_t i{}; i < namesSize; ++i)
		{
			reader.ReadString(data.names[i]);
		}
		int size{};
		reader.Read(size);
		reader.ReadArray(&data.hiScores[0], static_cast<size_t>(size));
	}
}

#include "BinaryReader.h"

//=======================================
//BinaryReader
//=======================================

dae::BinaryReader::BinaryReader(const std::filesystem::path& path)
	: m_In{ path, std::ios::out | std::ios::binary }
{
}

dae::BinaryReader::~BinaryReader()
{
	if (m_In)
		m_In.close();
}

std::string& dae::BinaryReader::ReadString(std::string& outStr)
{
	if (m_In)
	{
		int32_t size{};
		m_In.read(reinterpret_cast<char*>(&size), sizeof(int32_t));

		if (size > 0)
		{
			std::vector<char> buffer(size);
			m_In.read(&buffer[0], size);

			outStr.append(&buffer[0], size);
		}
	}
	return outStr;
}

//=======================================
//BinaryWriter
//=======================================

dae::BinaryWriter::BinaryWriter(const std::filesystem::path& path)
	: m_Out{path, std::ios::out | std::ios::binary }
{
}

dae::BinaryWriter::~BinaryWriter()
{
	if (m_Out)
		m_Out.close();
}

bool dae::BinaryWriter::WriteString(const std::string& str)
{
	if (m_Out)
	{
		int32_t length{ static_cast<int32_t>(str.length()) };
		Write(length);
		
		if (length > 0)
		{
			m_Out.write(str.c_str(), length);
			return true;
		}
	}
	return false;
}

void dae::BinaryWriter::SetPos(uint32_t pos)
{
	m_Out.seekp(pos);
}

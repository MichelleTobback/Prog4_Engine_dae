#pragma once

#include <filesystem>
#include <fstream>

namespace dae
{
	class BinaryReader final
	{
	public:
		BinaryReader(const std::filesystem::path& path);
		~BinaryReader();

		template <typename T>
		T& Read(T& outValue);
		template <typename T>
		void ReadArray(std::vector<T>& pOutData);
		std::string& ReadString(std::string& outStr);

		operator bool() const { return (m_In) ? true : false; }

	private:
		std::ifstream m_In;
	};

	class BinaryWriter final
	{
	public:
		BinaryWriter(const std::filesystem::path& path);
		~BinaryWriter();

		template <typename T>
		bool Write(const T& value);
		template <typename T>
		bool WriteArray(T* pData, size_t size);
		bool WriteString(const std::string& str);

		void SetPos(uint32_t pos);


		operator bool() const { return (m_Out) ? true : false; }

	private:
		std::ofstream m_Out;
	};
}

//=======================================
//BinaryReader
//=======================================

template <typename T>
T& dae::BinaryReader::Read(T& outValue)
{
	if (m_In)
	{
		m_In.read(reinterpret_cast<char*>(&outValue), sizeof(T));
	}
	return outValue;
}

template <typename T>
void dae::BinaryReader::ReadArray(std::vector<T>& pOutData)
{
	if (m_In)
	{
		int size{};
		m_In.read(reinterpret_cast<char*>(&size), sizeof(int));
		if (size > 0)
		{
			pOutData.resize(size);
			m_In.read(reinterpret_cast<char*>(&pOutData[0]), sizeof(T) * size);
		}
	}
}

//=======================================
//BinaryWriter
//=======================================

template <typename T>
bool dae::BinaryWriter::Write(const T& value)
{
	if (m_Out)
	{
		m_Out.write(reinterpret_cast<const char*>(&value), sizeof(T));
		return true;
	}
	return false;
}

template <typename T>
bool dae::BinaryWriter::WriteArray(T* pData, size_t size)
{
	if (m_Out)
	{
		Write(static_cast<int>(size));
		if (size > 0)
		{
			m_Out.write(reinterpret_cast<const char*>(pData), sizeof(T) * size);
			return true;
		}
	}
	return false;
}
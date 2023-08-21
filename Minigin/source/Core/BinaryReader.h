#pragma once
#include <cstdint>
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
		template <typename T>
		void ReadArray(T* pOutData, size_t size);
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
		uint32_t size{};
		m_In.read(reinterpret_cast<char*>(&size), sizeof(uint32_t));
		if (size > 0)
		{
			pOutData.resize(size);
			m_In.read(reinterpret_cast<char*>(&pOutData[0]), sizeof(T) * size);
		}
	}
}

template <typename T>
void dae::BinaryReader::ReadArray(T* pOutData, size_t size)
{
	if (m_In)
	{
		if (size > 0)
		{
			m_In.read(reinterpret_cast<char*>(pOutData), sizeof(T) * size);
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
		Write(static_cast<uint32_t>(size));
		if (size > 0)
		{
			m_Out.write(reinterpret_cast<const char*>(pData), sizeof(T) * size);
			return true;
		}
	}
	return false;
}
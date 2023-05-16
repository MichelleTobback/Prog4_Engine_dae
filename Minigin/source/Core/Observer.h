#pragma once
#include <string>

namespace dae
{
	struct Event
	{
		uint32_t Id;
		char* data;
		size_t dataSize;

		template <typename T>
		void GetData(T& outData) const
		{
			std::memcpy(&outData, data, dataSize);
		}
		template <typename T>
		void SetData(T& inData)
		{
			data = reinterpret_cast<char*>(&inData);
			dataSize = sizeof(T);
		}
	};

	class Subject;
	class Observer
	{
	public:
		Observer() = default;
		virtual ~Observer() = default;

		virtual void Invoke(const Event& event, Subject* pSubject) = 0;
	};
}
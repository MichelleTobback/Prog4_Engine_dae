#pragma once

#include <xhash>

//https://www.youtube.com/watch?v=O_0nUE4S8T8

namespace dae
{
	class UUID final
	{
	public:
		UUID();
		UUID(uint64_t uuid);

		bool IsValid() const;

		operator uint64_t() const { return m_Uuid; }
		//operator bool() const { return IsValid(); }

	private:
		uint64_t m_Uuid{};
	};
}

namespace std
{
	template<>
	struct hash<dae::UUID>
	{
		std::size_t operator()(const dae::UUID& uuid) const
		{
			return hash<uint64_t>()(static_cast<uint64_t>(uuid));
		}
	};
}
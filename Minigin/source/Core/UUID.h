#pragma once

#include <xhash>

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
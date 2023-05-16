#include "UUID.h"

#include "Random.h"

#include <unordered_map>
#include <string>

dae::UUID::UUID()
	: m_Uuid{}
{
	m_Uuid = Random::GetRandomUint64();
}

dae::UUID::UUID(uint64_t uuid)
	: m_Uuid{uuid}
{
}

bool dae::UUID::IsValid() const
{
	return m_Uuid != 0u;
}

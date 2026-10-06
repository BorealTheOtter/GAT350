#include "CharacterBase.h"

void CharacterBase::Read(const sr::json::value_t& value)
{
	Actor::Read(value);

	JSON_READ_NAME(value, "health", m_health);
}

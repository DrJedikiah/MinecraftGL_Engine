#include "Block.h"


const float Block::size = 1.f;


bool Block::IsEnabled() const
{
	return m_enabled;
}
bool Block::IsSolid() const
{
	return m_solid;
}
void Block::SetEnabled(bool state)
{
	m_enabled = state;
}

Block::Type Block::GetType() const
{
	return m_type;
}

void Block::SetType(Block::Type type)
{
	m_type = type;

	if (m_type == Type::air || m_type == Type::water)
		m_solid = false;
	else
		m_solid = true;

	SetEnabled(m_type != Type::air);
}
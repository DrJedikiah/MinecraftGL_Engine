#include "Cube.h"

const float Cube::size = 1.f;

Cube::Cube(CubeType  type) : m_type(type)
{

}

CubeType Cube::Type() const
{
	return m_type;
}

void Cube::SetType(CubeType type)
{
	m_type = type;

	if (m_type == CubeType::air || m_type == CubeType::water)
		m_solid = false;
	else
		m_solid = true;

	SetEnabled(m_type != CubeType::air);
}

bool Cube::IsSolid() const { return m_solid; }
bool Cube::IsEnabled() const { return m_enabled;  }
void Cube::SetEnabled(bool state) { m_enabled = state; }
#include <engine/map/CircularArray.h>



CircularArray::CircularArray(int size, int originX, int originZ) :
	m_size(size),
	m_xOrigin(originX),
	m_zOrigin(originZ),
	m_xOffset(0),
	m_zOffset(0)
{
	m_array.resize(size);
	for (int i = 0; i < size; ++i)
		m_array[i].resize(size, nullptr);
}

void CircularArray::MoveRight()
{
	m_xOffset = (m_xOffset + 1) % m_size;
	++m_xOrigin;
}

void CircularArray::MoveLeft()
{
	m_xOffset = (m_xOffset + m_size - 1) % m_size;
	--m_xOrigin;
}

void CircularArray::MoveBack()
{
	m_zOffset = (m_zOffset + m_size - 1) % m_size;
	--m_zOrigin;
}

void CircularArray::MoveFront()
{
	m_zOffset = (m_zOffset + 1) % m_size;
	++m_zOrigin;
}



void CircularArray::Set(int x, int z, Chunck* chunck)
{
	m_array[(x - m_xOrigin + m_xOffset) % m_size][(z - m_zOrigin + m_zOffset) % m_size] = chunck;
}

Chunck * CircularArray::Get(int x, int z)
{
	if (x < m_xOrigin || x >= m_xOrigin + m_size || z < m_zOrigin || z >= m_zOrigin + m_size)
		return nullptr;
	else
		return m_array[(x - m_xOrigin + m_xOffset) % m_size][(z - m_zOrigin + m_zOffset) % m_size];
}

int CircularArray::Size() const { return m_size; }
int CircularArray::OriginX() const { return m_xOrigin; }
int CircularArray::OriginZ() const { return m_zOrigin; }
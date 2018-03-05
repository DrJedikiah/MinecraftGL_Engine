#include "graphics/Tiles.h"

glm::ivec2 Tiles::m_nbTiles = { 1,1 };
glm::vec2 Tiles::m_tileSize = { 1,1 };

std::map<Tiles::ID, fRect> Tiles::m_blockRectangles;

void Tiles::Initialize(int width, int height)
{
	m_nbTiles = glm::ivec2(width, height);
	m_tileSize = glm::vec2(1.f / m_nbTiles.x, 1.f / m_nbTiles.y);

	Tiles::SetBlockTile(Tiles::invalid, 0, 0);
	Tiles::SetBlockTile(Tiles::dirt, 1, 1);
	Tiles::SetBlockTile(Tiles::grassTop, 1, 0);
	Tiles::SetBlockTile(Tiles::grassSide, 0, 1);
	Tiles::SetBlockTile(Tiles::bedrock, 2, 1);
	Tiles::SetBlockTile(Tiles::stone, 2, 0);
	Tiles::SetBlockTile(Tiles::wood, 3, 0);
	Tiles::SetBlockTile(Tiles::leaf, 3, 1);
}

void Tiles::SetBlockTile(Tiles::ID block, int x, int y)
{
	m_blockRectangles[block] = {
		m_tileSize.x*x,//X
		m_tileSize.y*y,//Y
		m_tileSize.x,//Width
		m_tileSize.y//Height
	};
}

fRect Tiles::GetRectangle(Tiles::ID block)
{
	return m_blockRectangles[block];
}

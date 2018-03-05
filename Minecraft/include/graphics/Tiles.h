#pragma once

#include <glm/glm.hpp>
#include <map>

#include "util/Util.h"

class Tiles
{
public:
	enum ID
	{
		invalid,
		dirt,
		grassTop,
		grassSide,
		bedrock,
		stone,
		wood,
		leaf,
	};
	
	static void Initialize(int width, int height);
	static fRect GetRectangle(ID block);
private:
	static void SetBlockTile(ID block, int x, int y);

	static glm::ivec2 m_nbTiles;
	static glm::vec2 m_tileSize;
	static std::map<ID, fRect> m_blockRectangles;
};
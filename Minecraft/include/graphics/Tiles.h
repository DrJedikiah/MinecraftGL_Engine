#pragma once

#include <glm/glm.hpp>
#include <map>

struct fRect
{
	float x1 = 0;
	float y1 = 0;
	float x2 = 0;
	float y2 = 0;
};

class Tiles
{
public:
	enum ID { dirt, grassTop, grassSide, player };

	static void Initialize(int width, int height);
	static fRect GetRectangle(ID block);


private:
	static void SetBlockTile(ID block, int x, int y);

	static glm::ivec2 m_nbTiles;
	static glm::vec2 m_tileSize;
	static std::map<ID, fRect> m_blockRectangles;
};
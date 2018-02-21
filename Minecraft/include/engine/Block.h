#pragma once

#include <glm/glm.hpp>

class Block
{
public:
	enum Type { dirt=0, grass, air, water};

	const static float size;
	Type type = dirt;
	bool enabled = true;
	bool solid = true;
};

/*
void ToDirt( Block& block)
{
	
}*/
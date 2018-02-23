#pragma once

#include <glm/glm.hpp>

class Block
{
public:
	enum Type { dirt=0, grass, air, water, player};

	const static float size;
	Type type = dirt;
	bool enabled = true;
	bool solid = true;

	void ToDirt()  { type = dirt;  enabled = true; solid = true;  }
	void ToGrass() { type = grass; enabled = true; solid = true;  }
	void ToAir()   { type = air;   enabled = false; solid = false; }
	void ToWater() { type = water; enabled = true; solid = false; }
};
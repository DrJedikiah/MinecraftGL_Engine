#pragma once

#include <glm/glm.hpp>

class Block
{
public:
	enum Type
	{ 
		invalid=0,
		air, 
		dirt, 
		grass, 
		water,
		stone, 
		bedrock, 
		wood, 
		leaf, 


	};

	const static float size;
	Type type = dirt;
	bool enabled = true;
	bool solid = true;
	bool destructible = true;

	void ToAir()		{ type = air;		enabled = false; solid = false; destructible = false;	}
	void ToDirt()		{ type = dirt;		enabled = true;  solid = true;  destructible = true;	}
	void ToGrass()		{ type = grass;		enabled = true;  solid = true;  destructible = true;	}
	void ToStone()		{ type = stone;		enabled = true;  solid = true;  destructible = true;	}
	void ToBedrock()	{ type = bedrock;	enabled = true;  solid = true;  destructible = false;	}
	void ToWood()		{ type = wood;		enabled = true;  solid = true;  destructible = true;	}
	void ToLeaf()		{ type = leaf;		enabled = true;  solid = false; destructible = true;	}
	void ToWater()		{ type = water;		enabled = true;  solid = false; destructible = false;	}
};
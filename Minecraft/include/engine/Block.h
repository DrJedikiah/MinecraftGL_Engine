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
	bool transparent = false;

	void SetType(Type newType)
	{
		switch (newType)
		{
		case Block::invalid: 
			type = air;		enabled = false; solid = false; destructible = false; transparent = false;
			break;
		case Block::air:
			type = air;		enabled = false; solid = false; destructible = false; transparent = false;
			break;
		case Block::dirt:
			type = dirt;	enabled = true;  solid = true;  destructible = true; transparent = false;
			break;
		case Block::grass:
			type = grass;	enabled = true;  solid = true;  destructible = true; transparent = false;
			break;
		case Block::water:
			type = water;	enabled = true;  solid = false; destructible = false; transparent = true;
			break;
		case Block::stone:
			type = stone;	enabled = true;  solid = true;  destructible = true; transparent = false;
			break;
		case Block::bedrock:
			type = bedrock;	enabled = true;  solid = true;  destructible = false; transparent = false;
			break;
		case Block::wood:
			type = wood;	enabled = true;  solid = true;  destructible = true; transparent = false;
			break;
		case Block::leaf:
			type = leaf;	enabled = true;  solid = true; destructible = true; transparent = true;
			break;
		default:
			break;
		}
	}
};
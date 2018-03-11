#pragma once

#include <glm/glm.hpp>
#include <iostream>

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
		glassRed
	};

	const static float size;
	Type type = dirt;
	bool solid = true;
	bool destructible = true;
	bool seeThrough = false;
	bool transparent = false;

	void SetType(Type newType)
	{
		switch (newType)
		{
		case Block::invalid: 
			type = air;			solid = false; destructible = false; seeThrough = false, transparent = false;
			break;
		case Block::air:
			type = air;			solid = false; destructible = false; seeThrough = false, transparent = false;
			break;
		case Block::dirt:
			type = dirt;		solid = true;  destructible = true; seeThrough = false, transparent = false;
			break;
		case Block::grass:
			type = grass;		solid = true;  destructible = true; seeThrough = false, transparent = false;
			break;
		case Block::water:
			type = water;		solid = false; destructible = false; seeThrough = true, transparent = false;
			break;
		case Block::stone:
			type = stone;		solid = true;  destructible = true; seeThrough = false, transparent = false;
			break;
		case Block::bedrock:
			type = bedrock;		solid = true;  destructible = false; seeThrough = false, transparent = false;
			break;
		case Block::wood:
			type = wood;		solid = true;  destructible = true; seeThrough = false, transparent = false;
			break;
		case Block::leaf:
			type = leaf;		solid = true; destructible = true; seeThrough = true, transparent = false;
			break;
		case Block::glassRed:
			type = glassRed;	solid = true; destructible = true; seeThrough = true, transparent = true;
			break;
		default:
			std::cerr << "Block::SetType : block not found " << newType << std::endl;
			break;
		}
	}
};
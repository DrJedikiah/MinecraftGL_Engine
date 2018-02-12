#pragma once

#include <glm/glm.hpp>

class Cube
{
public:
	enum Type{ dirt, grass, water, air };
	static const float size;

	Cube(Type Type = air);
	Type type;

};
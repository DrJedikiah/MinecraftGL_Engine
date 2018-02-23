#pragma once

#include <vector>

#include "graphics/Tiles.h"
#include "graphics/Mesh.h"

enum Tag { default, chunck, entity };

namespace glm
{
	inline glm::vec3 toVec3(btVector3 vector) { return glm::vec3(vector.getX(), vector.getY(), vector.getZ());  };
}

namespace bt
{
	inline btVector3 toVec3(glm::vec3 vector) { return btVector3(vector.x, vector.y, vector.z); };
}

namespace std
{
	inline float clamp(float value, float min, float max)
	{
		if ( value < min ) return min;
		if (value > max) return max;
		return value;
	}
}

namespace Util
{

	inline std::vector<Vertex> cubeTopFace(float size, float x, float y, float z, fRect rect)
	{
		const float s = size / 2, X = x * size, Y = y * size, Z = z * size;
		return {{ { X + s,  Y + s, Z - s },{ 0.f,1.f,0.f },{ rect.x2, rect.y2 } },
				{ { X - s,  Y + s, Z - s },{ 0.f,1.f,0.f },{ rect.x1, rect.y2 } },
				{ { X + s,  Y + s, Z + s },{ 0.f,1.f,0.f },{ rect.x2, rect.y1 } },
				{ { X - s,  Y + s, Z + s },{ 0.f,1.f,0.f },{ rect.x1, rect.y1 } },
				{ { X + s,  Y + s, Z + s },{ 0.f,1.f,0.f },{ rect.x2, rect.y1 } },
				{ { X - s,  Y + s, Z - s },{ 0.f,1.f,0.f },{ rect.x1, rect.y2 } }};
	}

	inline std::vector<Vertex> cubeBotFace(float size, float x, float y, float z, fRect rect)
	{
		const float s = size / 2, X = x * size, Y = y * size, Z = z * size;
		return {{ { X - s, Y - s, Z - s },{ 0.f,-1.f,0.f },{ rect.x1, rect.y2 } },
				{ { X + s, Y - s, Z - s },{ 0.f,-1.f,0.f },{ rect.x2, rect.y2 } },
				{ { X + s, Y - s, Z + s },{ 0.f,-1.f,0.f },{ rect.x2, rect.y1 } },
				{ { X + s, Y - s, Z + s },{ 0.f,-1.f,0.f },{ rect.x2, rect.y1 } },
				{ { X - s, Y - s, Z + s },{ 0.f,-1.f,0.f },{ rect.x1, rect.y1 } },
				{ { X - s, Y - s, Z - s },{ 0.f,-1.f,0.f },{ rect.x1, rect.y2 } }};
	}

	inline std::vector<Vertex> cubeLeftFace(float size, float x, float y, float z, fRect rect)
	{
		const float s = size / 2, X = x * size, Y = y * size, Z = z * size;
		return{ { { X - s, Y + s, Z + s },{ -1.f,0.f,0.f },{ rect.x1, rect.y2 } },
				{ { X - s, Y + s, Z - s },{ -1.f,0.f,0.f },{ rect.x2, rect.y2 } },
				{ { X - s, Y - s, Z - s },{ -1.f,0.f,0.f },{ rect.x2, rect.y1 } },
				{ { X - s, Y - s, Z - s },{ -1.f,0.f,0.f },{ rect.x2, rect.y1 } },
				{ { X - s, Y - s, Z + s },{ -1.f,0.f,0.f },{ rect.x1, rect.y1 } },
				{ { X - s, Y + s, Z + s },{ -1.f,0.f,0.f },{ rect.x1, rect.y2 } }, };
	}

	inline std::vector<Vertex> cubeRightFace(float size, float x, float y, float z, fRect rect)
	{
		const float s = size / 2, X = x * size, Y = y * size, Z = z * size;
		return{ { { X + s, Y + s, Z + s },{ 1.f,0.f,0.f },{ rect.x1, rect.y2 } },//Top
				{ { X + s, Y - s, Z - s },{ 1.f,0.f,0.f },{ rect.x2, rect.y1 } },
				{ { X + s, Y + s, Z - s },{ 1.f,0.f,0.f },{ rect.x2, rect.y2 } },
				{ { X + s, Y - s, Z - s },{ 1.f,0.f,0.f },{ rect.x2, rect.y1 } },//Bot
				{ { X + s, Y + s, Z + s },{ 1.f,0.f,0.f },{ rect.x1, rect.y2 } },
				{ { X + s, Y - s, Z + s },{ 1.f,0.f,0.f },{ rect.x1, rect.y1 } }, };
	}

	inline std::vector<Vertex> cubeFrontFace(float size, float x, float y, float z, fRect rect)
	{
		const float s = size / 2, X = x * size, Y = y * size, Z = z * size;
		return{ { { X - s, Y - s, Z + s },{ 0.f,0.f,1.f },{ rect.x1, rect.y1 } },//bot
				{ { X + s,Y - s, Z + s },{ 0.f,0.f,1.f },{ rect.x2, rect.y1 } },
				{ { X + s, Y + s,Z + s },{ 0.f,0.f,1.f },{ rect.x2, rect.y2 } },
				{ { X + s, Y + s, Z + s },{ 0.f,0.f,1.f },{ rect.x2, rect.y2 } },//Top
				{ { X - s, Y + s, Z + s },{ 0.f,0.f,1.f },{ rect.x1, rect.y2 } },
				{ { X - s, Y - s,Z + s },{ 0.f,0.f,1.f },{ rect.x1, rect.y1 } }, };
	}

	inline std::vector<Vertex> cubeBackFace(float size, float x, float y, float z, fRect rect)
	{
		const float s = size / 2, X = x * size, Y = y * size, Z = z * size;
		return{ { { X - s, Y - s, Z - s },{ 0.f,0.f,-1.f },{ rect.x1, rect.y1 } },//bot
				{ { X + s, Y + s, Z - s },{ 0.f,0.f,-1.f },{ rect.x2, rect.y2 } },
				{ { X + s, Y - s, Z - s },{ 0.f,0.f,-1.f },{ rect.x2, rect.y1 } },
				{ { X + s, Y + s, Z - s },{ 0.f,0.f,-1.f },{ rect.x2, rect.y2 } },//Top
				{ { X - s, Y - s, Z - s },{ 0.f,0.f,-1.f },{ rect.x1, rect.y1 } },
				{ { X - s, Y + s, Z - s },{ 0.f,0.f,-1.f },{ rect.x1, rect.y2 } }, };
	}
}
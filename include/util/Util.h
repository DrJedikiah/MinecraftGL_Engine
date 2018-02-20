#pragma once

#include <vector>
#include "graphics/Mesh.h"

namespace Util
{
	static std::vector<Vertex> cubeTopFace(float size, float x, float y, float z, fRect rect)
	{
		const float s = size / 2, X = x * size, Y = y * size, Z = z * size;
		return {{ { X + s,  Y + s, Z - s },{ 0.f,1.f,0.f },{ rect.x2, rect.y2 } },
				{ { X - s,  Y + s, Z - s },{ 0.f,1.f,0.f },{ rect.x1, rect.y2 } },
				{ { X + s,  Y + s, Z + s },{ 0.f,1.f,0.f },{ rect.x2, rect.y1 } },
				{ { X - s,  Y + s, Z + s },{ 0.f,1.f,0.f },{ rect.x1, rect.y1 } },
				{ { X + s,  Y + s, Z + s },{ 0.f,1.f,0.f },{ rect.x2, rect.y1 } },
				{ { X - s,  Y + s, Z - s },{ 0.f,1.f,0.f },{ rect.x1, rect.y2 } }};
	}

	static std::vector<Vertex> cubeBotFace(float size, float x, float y, float z, fRect rect)
	{
		const float s = size / 2, X = x * size, Y = y * size, Z = z * size;
		return {{ { X - s, Y - s, Z - s },{ 0.f,-1.f,0.f },{ rect.x1, rect.y2 } },
				{ { X + s, Y - s, Z - s },{ 0.f,-1.f,0.f },{ rect.x2, rect.y2 } },
				{ { X + s, Y - s, Z + s },{ 0.f,-1.f,0.f },{ rect.x2, rect.y1 } },
				{ { X + s, Y - s, Z + s },{ 0.f,-1.f,0.f },{ rect.x2, rect.y1 } },
				{ { X - s, Y - s, Z + s },{ 0.f,-1.f,0.f },{ rect.x1, rect.y1 } },
				{ { X - s, Y - s, Z - s },{ 0.f,-1.f,0.f },{ rect.x1, rect.y2 } }};
	}

	static std::vector<Vertex> cubeLeftFace(float size, float x, float y, float z, fRect rect)
	{
		const float s = size / 2, X = x * size, Y = y * size, Z = z * size;
		return{ { { X - s, Y + s, Z + s },{ -1.f,0.f,0.f },{ rect.x1, rect.y2 } },
				{ { X - s, Y + s, Z - s },{ -1.f,0.f,0.f },{ rect.x2, rect.y2 } },
				{ { X - s, Y - s, Z - s },{ -1.f,0.f,0.f },{ rect.x2, rect.y1 } },
				{ { X - s, Y - s, Z - s },{ -1.f,0.f,0.f },{ rect.x2, rect.y1 } },
				{ { X - s, Y - s, Z + s },{ -1.f,0.f,0.f },{ rect.x1, rect.y1 } },
				{ { X - s, Y + s, Z + s },{ -1.f,0.f,0.f },{ rect.x1, rect.y2 } }, };
	}

	static std::vector<Vertex> cubeRightFace(float size, float x, float y, float z, fRect rect)
	{
		const float s = size / 2, X = x * size, Y = y * size, Z = z * size;
		return{ { { X + s, Y + s, Z + s },{ 1.f,0.f,0.f },{ rect.x1, rect.y2 } },//Top
				{ { X + s, Y - s, Z - s },{ 1.f,0.f,0.f },{ rect.x2, rect.y1 } },
				{ { X + s, Y + s, Z - s },{ 1.f,0.f,0.f },{ rect.x2, rect.y2 } },
				{ { X + s, Y - s, Z - s },{ 1.f,0.f,0.f },{ rect.x2, rect.y1 } },//Bot
				{ { X + s, Y + s, Z + s },{ 1.f,0.f,0.f },{ rect.x1, rect.y2 } },
				{ { X + s, Y - s, Z + s },{ 1.f,0.f,0.f },{ rect.x1, rect.y1 } }, };
	}

	static std::vector<Vertex> cubeFrontFace(float size, float x, float y, float z, fRect rect)
	{
		const float s = size / 2, X = x * size, Y = y * size, Z = z * size;
		return{ { { X - s, Y - s, Z + s },{ 0.f,0.f,1.f },{ rect.x1, rect.y1 } },//bot
				{ { X + s,Y - s, Z + s },{ 0.f,0.f,1.f },{ rect.x2, rect.y1 } },
				{ { X + s, Y + s,Z + s },{ 0.f,0.f,1.f },{ rect.x2, rect.y2 } },
				{ { X + s, Y + s, Z + s },{ 0.f,0.f,1.f },{ rect.x2, rect.y2 } },//Top
				{ { X - s, Y + s, Z + s },{ 0.f,0.f,1.f },{ rect.x1, rect.y2 } },
				{ { X - s, Y - s,Z + s },{ 0.f,0.f,1.f },{ rect.x1, rect.y1 } }, };
	}

	static std::vector<Vertex> cubeBackFace(float size, float x, float y, float z, fRect rect)
	{
		const float s = size / 2, X = x * size, Y = y * size, Z = z * size;
		return{ { { X - s, Y - s, Z - s },{ 0.f,0.f,-1.f },{ rect.x1, rect.y1 } },//bot
				{ { X + s, Y + s, Z - s },{ 0.f,0.f,-1.f },{ rect.x2, rect.y2 } },
				{ { X + s, Y - s, Z - s },{ 0.f,0.f,-1.f },{ rect.x2, rect.y1 } },
				{ { X + s, Y + s, Z - s },{ 0.f,0.f,-1.f },{ rect.x2, rect.y2 } },//Top
				{ { X - s, Y - s, Z - s },{ 0.f,0.f,-1.f },{ rect.x1, rect.y1 } },
				{ { X - s, Y + s, Z - s },{ 0.f,0.f,-1.f },{ rect.x1, rect.y2 } }, };
	}

	static std::vector<Vertex> GrassCubeMesh(float size, float x, float y, float z)
	{
		float s = size / 2;
		float X = x * s * 2;
		float Y = y * s * 2;
		float Z = z * s * 2;
		  
		return{
			//Back
		{ { X - s, Y - s, Z - s },{ 0.f,0.f,-1.f },{ 0.f, 0.5f } },//bot
		{ { X + s, Y + s, Z - s },{ 0.f,0.f,-1.f },{ 0.5f, 1.0f } },
		{ { X + s, Y - s, Z - s },{ 0.f,0.f,-1.f },{ 0.5f, 0.5f } },
		{ { X + s, Y + s, Z - s },{ 0.f,0.f,-1.f },{ 0.5f, 1.0f } },//Top
		{ { X - s, Y - s, Z - s },{ 0.f,0.f,-1.f },{ 0.f, 0.5f } },
		{ { X - s, Y + s, Z - s },{ 0.f,0.f,-1.f },{ 0.f, 1.0f } },

		//Front
		{ { X - s, Y - s, Z + s },{ 0.f,0.f,1.f },{ 0.f, 0.5f } },//bot
		{ { X + s,Y - s, Z + s },{ 0.f,0.f,1.f },{ 0.5f, 0.5f } },
		{ { X + s, Y + s,Z + s },{ 0.f,0.f,1.f },{ s, 1.0f } },
		{ { X + s, Y + s, Z + s },{ 0.f,0.f,1.f },{ 0.5f, 1.0f } },//Top
		{ { X - s, Y + s, Z + s },{ 0.f,0.f,1.f },{ 0.f, 1.0f } },
		{ { X - s, Y - s,Z + s },{ 0.f,0.f,1.f },{ 0.f, 0.5f } },

		//Left
		{ { X - s, Y + s, Z + s },{ -1.f,0.f,0.f },{ 0.f, 1.f } },//Top
		{ { X - s, Y + s, Z - s },{ -1.f,0.f,0.f },{ 0.5f, 1.f } },
		{ { X - s, Y - s, Z - s },{ -1.f,0.f,0.f },{ 0.5f, 0.5f } },
		{ { X - s, Y - s, Z - s },{ -1.f,0.f,0.f },{ 0.5f, 0.5f } },//Bot
		{ { X - s, Y - s, Z + s },{ -1.f,0.f,0.f },{ 0.f, 0.5f } },
		{ { X - s, Y + s, Z + s },{ -1.f,0.f,0.f },{ 0.f, 1.f } },

		//Right
		{ { X + s, Y + s, Z + s },{ 1.f,0.f,0.f },{ 0.f, 1.f } },//Top
		{ { X + s, Y - s, Z - s },{ 1.f,0.f,0.f },{ 0.5f, 0.5f } },
		{ { X + s, Y + s, Z - s },{ 1.f,0.f,0.f },{ 0.5f, 1.f } },
		{ { X + s, Y - s, Z - s },{ 1.f,0.f,0.f },{ 0.5f, 0.5f } },//Bot
		{ { X + s, Y + s, Z + s },{ 1.f,0.f,0.f },{ 0.f, 1.f } },
		{ { X + s, Y - s, Z + s },{ 1.f,0.f,0.f },{ 0.f, 0.5f } },

		//Bot
		{ { X - s, Y - s, Z - s },{ 0.f,-1.f,0.f },{ 0.5f, 1.0f } },
		{ { X + s, Y - s, Z - s },{ 0.f,-1.f,0.f },{ 1.0f, 1.0f } },
		{ { X + s, Y - s, Z + s },{ 0.f,-1.f,0.f },{ 1.0f, 0.5f } },
		{ { X + s, Y - s, Z + s },{ 0.f,-1.f,0.f },{ 1.0f, 0.5f } },
		{ { X - s, Y - s, Z + s },{ 0.f,-1.f,0.f },{ 0.5f, 0.5f } },
		{ { X - s, Y - s, Z - s },{ 0.f,-1.f,0.f },{ 0.5f, 1.0f } },

		//Top
		{ { X + s,  Y + s, Z - s },{ 0.f,1.f,0.f },{ 0.5f, 0.5f } },
		{ { X - s,  Y + s, Z - s },{ 0.f,1.f,0.f },{ 0.0f, 0.5f } },
		{ { X + s,  Y + s, Z + s },{ 0.f,1.f,0.f },{ 0.5f, 0.0f } },
		{ { X - s,  Y + s, Z + s },{ 0.f,1.f,0.f },{ 0.0f, 0.0f } },
		{ { X + s,  Y + s, Z + s },{ 0.f,1.f,0.f },{ 0.5f, 0.0f } },
		{ { X - s,  Y + s, Z - s },{ 0.f,1.f,0.f },{ 0.0f, 0.5f } }
		};
	}
	static std::vector<Vertex> DirtCubeMesh(float size, float x, float y, float z)
	{
		float s = size / 2;
		float X = x * s * 2;
		float Y = y * s * 2;
		float Z = z * s * 2;

		return{
			//Back
			{ { X - s, Y - s, Z - s },{ 0.f,0.f,-1.f },{ 0.5f, 0.5f } },//bot
			{ { X + s, Y + s, Z - s },{ 0.f,0.f,-1.f },{ 1.f, 1.0f } },
			{ { X + s, Y - s, Z - s },{ 0.f,0.f,-1.f },{ 1.f, 0.5f } },
			{ { X + s, Y + s, Z - s },{ 0.f,0.f,-1.f },{ 1.f, 1.0f } },//Top
			{ { X - s, Y - s, Z - s },{ 0.f,0.f,-1.f },{ 0.5f, 0.5f } },
			{ { X - s, Y + s, Z - s },{ 0.f,0.f,-1.f },{ 0.5f, 1.0f } },

			//Front
			{ { X - s, Y - s, Z + s },{ 0.f,0.f,1.f },{ 0.5f, 0.5f } },//bot
			{ { X + s,Y - s, Z + s },{ 0.f,0.f,1.f },{ 1.f, 0.5f } },
			{ { X + s, Y + s,Z + s },{ 0.f,0.f,1.f },{ 1.f, 1.0f } },
			{ { X + s, Y + s, Z + s },{ 0.f,0.f,1.f },{ 1.f, 1.0f } },//Top
			{ { X - s, Y + s, Z + s },{ 0.f,0.f,1.f },{ 0.5f, 1.0f } },
			{ { X - s, Y - s,Z + s },{ 0.f,0.f,1.f },{ 0.5f, 0.5f } },

			//Left
			{ { X - s, Y + s, Z + s },{ -1.f,0.f,0.f },{ 0.5f, 1.f } },//Top
			{ { X - s, Y + s, Z - s },{ -1.f,0.f,0.f },{ 1.f, 1.f } },
			{ { X - s, Y - s, Z - s },{ -1.f,0.f,0.f },{ 1.f, 0.5f } },
			{ { X - s, Y - s, Z - s },{ -1.f,0.f,0.f },{ 1.f, 0.5f } },//Bot
			{ { X - s, Y - s, Z + s },{ -1.f,0.f,0.f },{ 0.5f, 0.5f } },
			{ { X - s, Y + s, Z + s },{ -1.f,0.f,0.f },{ 0.5f, 1.f } },

			//Right
			{ { X + s, Y + s, Z + s },{ 1.f,0.f,0.f },{ 0.5f, 1.f } },//Top
			{ { X + s, Y - s, Z - s },{ 1.f,0.f,0.f },{ 1.f, 0.5f } },
			{ { X + s, Y + s, Z - s },{ 1.f,0.f,0.f },{ 1.f, 1.f } },
			{ { X + s, Y - s, Z - s },{ 1.f,0.f,0.f },{ 1.f, 0.5f } },//Bot
			{ { X + s, Y + s, Z + s },{ 1.f,0.f,0.f },{ 0.5f, 1.f } },
			{ { X + s, Y - s, Z + s },{ 1.f,0.f,0.f },{ 0.5f, 0.5f } },

			//Bot
			{ { X - s, Y - s, Z - s },{ 0.f,-1.f,0.f },{ 0.5f, 1.0f } },
			{ { X + s, Y - s, Z - s },{ 0.f,-1.f,0.f },{ 1.0f, 1.0f } },
			{ { X + s, Y - s, Z + s },{ 0.f,-1.f,0.f },{ 1.0f, 0.5f } },
			{ { X + s, Y - s, Z + s },{ 0.f,-1.f,0.f },{ 1.0f, 0.5f } },
			{ { X - s, Y - s, Z + s },{ 0.f,-1.f,0.f },{ 0.5f, 0.5f } },
			{ { X - s, Y - s, Z - s },{ 0.f,-1.f,0.f },{ 0.5f, 1.0f } },

			//Top
			{ { X + s,  Y + s, Z - s },{ 0.f,1.f,0.f },{ 1.f, 1.f }   },
			{ { X - s,  Y + s, Z - s },{ 0.f,1.f,0.f },{ 0.5f, 1.f }  },
			{ { X + s,  Y + s, Z + s },{ 0.f,1.f,0.f },{ 1.f, 0.5f }  },
			{ { X - s,  Y + s, Z + s },{ 0.f,1.f,0.f },{ 0.5f, 0.5f } },
			{ { X + s,  Y + s, Z + s },{ 0.f,1.f,0.f },{ 1.f, 0.5f }  },
			{ { X - s,  Y + s, Z - s },{ 0.f,1.f,0.f },{ 0.5f, 1.f }  }
		};
	}
}
#pragma once

#include "engine/Entity.h"
#include "engine/Block.h"
#include "graphics/Model.h"
#include "graphics/TexturesBlocks.h"
#include "graphics/Drawable.h"
#include "util/Util.h"

class Cube : public Entity
{
public:
	Cube(float size = 1.f);

	void Draw(const Shader & shader) const override;
	void Update(float delta) override;
	void UpdateModels() override ;

	static std::vector<Mesh::Vertex> CreateCubeMesh(float size, Block::Type type = Block::Type::dirt, float x = 0, float y = 0, float z = 0);
private:
	Model m_model;
	float m_size;

public:
	inline static std::vector<Mesh::Vertex> cubeTopFace(float size, float x, float y, float z, fRect rect)
	{
		const float s = size / 2, X = x * size, Y = y * size, Z = z * size;
		return { { { X + s,  Y + s, Z - s },{ 0.f,1.f,0.f },{ rect.x + rect.width, rect.y + rect.height } },
		{ { X - s,  Y + s, Z - s },{ 0.f,1.f,0.f },{ rect.x, rect.y + rect.height } },
		{ { X + s,  Y + s, Z + s },{ 0.f,1.f,0.f },{ rect.x + rect.width, rect.y } },
		{ { X - s,  Y + s, Z + s },{ 0.f,1.f,0.f },{ rect.x, rect.y } },
		{ { X + s,  Y + s, Z + s },{ 0.f,1.f,0.f },{ rect.x + rect.width, rect.y } },
		{ { X - s,  Y + s, Z - s },{ 0.f,1.f,0.f },{ rect.x, rect.y + rect.height } } };
	}

	inline static std::vector<Mesh::Vertex> cubeBotFace(float size, float x, float y, float z, fRect rect)
	{
		const float s = size / 2, X = x * size, Y = y * size, Z = z * size;
		return { { { X - s, Y - s, Z - s },{ 0.f,-1.f,0.f },{ rect.x, rect.y + rect.height } },
		{ { X + s, Y - s, Z - s },{ 0.f,-1.f,0.f },{ rect.x + rect.width, rect.y + rect.height } },
		{ { X + s, Y - s, Z + s },{ 0.f,-1.f,0.f },{ rect.x + rect.width, rect.y } },
		{ { X + s, Y - s, Z + s },{ 0.f,-1.f,0.f },{ rect.x + rect.width, rect.y } },
		{ { X - s, Y - s, Z + s },{ 0.f,-1.f,0.f },{ rect.x, rect.y } },
		{ { X - s, Y - s, Z - s },{ 0.f,-1.f,0.f },{ rect.x, rect.y + rect.height } } };
	}

	inline static std::vector<Mesh::Vertex> cubeLeftFace(float size, float x, float y, float z, fRect rect)
	{
		const float s = size / 2, X = x * size, Y = y * size, Z = z * size;
		return{ { { X - s, Y + s, Z + s },{ -1.f,0.f,0.f },{ rect.x, rect.y + rect.height } },
		{ { X - s, Y + s, Z - s },{ -1.f,0.f,0.f },{ rect.x + rect.width, rect.y + rect.height } },
		{ { X - s, Y - s, Z - s },{ -1.f,0.f,0.f },{ rect.x + rect.width, rect.y } },
		{ { X - s, Y - s, Z - s },{ -1.f,0.f,0.f },{ rect.x + rect.width, rect.y } },
		{ { X - s, Y - s, Z + s },{ -1.f,0.f,0.f },{ rect.x, rect.y } },
		{ { X - s, Y + s, Z + s },{ -1.f,0.f,0.f },{ rect.x, rect.y + rect.height } }, };
	}

	inline static std::vector<Mesh::Vertex> cubeRightFace(float size, float x, float y, float z, fRect rect)
	{
		const float s = size / 2, X = x * size, Y = y * size, Z = z * size;
		return{ { { X + s, Y + s, Z + s },{ 1.f,0.f,0.f },{ rect.x, rect.y + rect.height } },//Top
		{ { X + s, Y - s, Z - s },{ 1.f,0.f,0.f },{ rect.x + rect.width, rect.y } },
		{ { X + s, Y + s, Z - s },{ 1.f,0.f,0.f },{ rect.x + rect.width, rect.y + rect.height } },
		{ { X + s, Y - s, Z - s },{ 1.f,0.f,0.f },{ rect.x + rect.width, rect.y } },//Bot
		{ { X + s, Y + s, Z + s },{ 1.f,0.f,0.f },{ rect.x, rect.y + rect.height } },
		{ { X + s, Y - s, Z + s },{ 1.f,0.f,0.f },{ rect.x, rect.y } }, };
	}

	inline static std::vector<Mesh::Vertex> cubeFrontFace(float size, float x, float y, float z, fRect rect)
	{
		const float s = size / 2, X = x * size, Y = y * size, Z = z * size;
		return{ { { X - s, Y - s, Z + s },{ 0.f,0.f,1.f },{ rect.x, rect.y } },//bot
		{ { X + s,Y - s, Z + s },{ 0.f,0.f,1.f },{ rect.x + rect.width, rect.y } },
		{ { X + s, Y + s,Z + s },{ 0.f,0.f,1.f },{ rect.x + rect.width, rect.y + rect.height } },
		{ { X + s, Y + s, Z + s },{ 0.f,0.f,1.f },{ rect.x + rect.width, rect.y + rect.height } },//Top
		{ { X - s, Y + s, Z + s },{ 0.f,0.f,1.f },{ rect.x, rect.y + rect.height } },
		{ { X - s, Y - s,Z + s },{ 0.f,0.f,1.f },{ rect.x, rect.y } }, };
	}

	inline static std::vector<Mesh::Vertex> cubeBackFace(float size, float x, float y, float z, fRect rect)
	{
		const float s = size / 2, X = x * size, Y = y * size, Z = z * size;
		return{ { { X - s, Y - s, Z - s },{ 0.f,0.f,-1.f },{ rect.x, rect.y } },//bot
		{ { X + s, Y + s, Z - s },{ 0.f,0.f,-1.f },{ rect.x + rect.width, rect.y + rect.height } },
		{ { X + s, Y - s, Z - s },{ 0.f,0.f,-1.f },{ rect.x + rect.width, rect.y } },
		{ { X + s, Y + s, Z - s },{ 0.f,0.f,-1.f },{ rect.x + rect.width, rect.y + rect.height } },//Top
		{ { X - s, Y - s, Z - s },{ 0.f,0.f,-1.f },{ rect.x, rect.y } },
		{ { X - s, Y + s, Z - s },{ 0.f,0.f,-1.f },{ rect.x, rect.y + rect.height } }, };
	}
};
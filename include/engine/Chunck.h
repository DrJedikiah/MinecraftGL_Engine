#pragma once

#include <vector>

#include "Block.h"
#include "graphics/Shader.h"
#include "graphics/Model.h"
#include "util/Util.h"
#include "engine/Physics.h"



class Chunck
{
public:
	Chunck();
	void Setup(btVector3 position);

	static const int size = 16;

	void GenerateMesh();
	void GenerateCollider(  PhysicsEngine & physicsEngine);
	void Draw(const Shader& shader) const;

	Block& GetBlock( int x, int y, int z);

	friend class World;

private:
	btVector3 m_position;

	Model m_model;
	Block m_blocks[size][size][size];

	std::vector<Vertex> vertices;

};


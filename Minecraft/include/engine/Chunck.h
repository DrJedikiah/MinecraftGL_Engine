#pragma once

#include <vector>

#include "graphics/Shader.h"
#include "graphics/Model.h"
#include "graphics/TexturesBlocks.h"
#include "engine/Block.h"
#include "engine/Physics.h"
#include "engine/World.h"
#include "util/Util.h"

class World;

class Chunck : public Drawable
{
public:
	Chunck();
	void Setup( glm::ivec3 position);

	static const int size = 16;

	void GenerateMesh(World& world);
	void GenerateCollider();
	void Draw(const Shader & shader) const override;

	Block& GetBlock( int x, int y, int z);

	friend class World;

private:
	glm::ivec3 m_position;

	Model m_model;
	Block m_blocks[size][size][size];

	std::vector<Vertex> vertices;

	//Collider
	btTriangleMesh m_btMesh;
	btBvhTriangleMeshShape * m_shape = nullptr;
		
};


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
	friend class World;

	Chunck();
	void Setup( World* world, glm::ivec3 position);

	static const int size = 16;

	void GenerateMesh();
	void GenerateCollider();
	void Draw(const Shader & shader) const override;

	Block& GetBlock(glm::ivec3 position);
	void RemoveBlock(glm::ivec3 position);

private:
	World* m_world;
	glm::ivec3 m_position;

	Model m_model;
	Block m_blocks[size][size][size];

	std::vector<Vertex> vertices;

	//Collider
	btTriangleMesh m_btMesh;
	btBvhTriangleMeshShape * m_shape = nullptr;
};


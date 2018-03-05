#pragma once

#include <vector>

#include "graphics/Shader.h"
#include "graphics/Model.h"
#include "graphics/TexturesBlocks.h"
#include "engine/Block.h"
#include "engine/Physics.h"
#include "engine/World.h"
#include "engine/Cube.h"
#include "util/Util.h"
#include "util/Time.h"
#include "util/Statistics.h"


class World;

class Chunck : public Drawable, public Statistics
{
public:
	friend class World;

	Chunck();
	~Chunck();
	void Setup( World* world, glm::ivec3 position);

	static const int size = 16;

	void Update(float delta);
	void Draw(const Shader & shader) const override;

	Block& GetBlock(glm::ivec3 position);

	void GenerateLater();
	void GenerateMesh();
	void GenerateCollider();

private:
	bool m_generateLater = false;

	World* m_world;
	glm::ivec3 m_position;

	Model * m_model;
	Block m_blocks[size][size][size];

	std::vector<Mesh::Vertex> vertices;

	//Collider
	RigidBody * m_rb;
	btTriangleMesh m_btMesh;
	btBvhTriangleMeshShape * m_shape = nullptr;
};


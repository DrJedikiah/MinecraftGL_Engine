#pragma once

#include <glm/glm.hpp>


#include "graphics/Shader.h"
#include "graphics/Model.h"
#include "graphics/TexturesBlocks.h"

#include "engine/Physics.h"
#include "engine/map/Block.h"
#include "engine/Cube.h"
#include "engine/map/World.h"

#include "util/Statistics.h"

class World;

class SubChunck : public Statistics
{
public:
	friend class Chunck;
	static const int size = 16;

	SubChunck(glm::ivec3 position);
	~SubChunck();

	void Update(float delta);
	void SetEnabled(bool state);

	void GenerateCollider();
	void GenerateMesh();

	void DrawTransparent(const Shader & shader) const;
	void DrawOpaque(const Shader & shader) const;

	

	Block* GetBlock(glm::ivec3 position);
private:
	void CheckEmpty();
	bool m_isEmpty = true;
	bool m_colliderGenerated = false;
	bool m_regenerateLater = false;
	bool m_enabled = true;

	glm::ivec3 m_position;

	Block m_blocks[SubChunck::size][SubChunck::size][SubChunck::size];

	//Collider
	RigidBody * m_rb;
	btTriangleMesh m_btMesh;
	btBvhTriangleMeshShape * m_shape = nullptr;

	Model * m_modelOpaque;
	Model * m_modelTransparent;
};
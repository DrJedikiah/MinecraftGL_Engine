#include "engine/map/SubChunck.h"

SubChunck::SubChunck(glm::ivec3 position, Chunck * parent) :
	m_position(position),
	m_parent(parent),
	m_modelOpaque(nullptr),
	m_modelTransparent(nullptr),
	m_shape(nullptr),
	m_rb(nullptr),
	m_isEmpty(true),
	m_colliderGenerated(false)
{
	m_btMesh.preallocateVertices(SubChunck::size*SubChunck::size*SubChunck::size);
	m_btMesh.preallocateIndices(SubChunck::size*SubChunck::size*SubChunck::size);
}

void SubChunck::Update(float delta)
{
	if (m_regenerateLater)
	{
		/*GenerateMesh();
		GenerateModels();*/
		//GenerateCollider();
		m_regenerateLater = false;
	}
}

Block* SubChunck::GetBlock(glm::ivec3 position)
{
	return &m_blocks[position.x][position.y][position.z];
}

void SubChunck::CheckEmpty()
{
	for (int x = 0; x < SubChunck::size; ++x)
		for (int y = 0; y < SubChunck::size; ++y)
			for (int z = 0; z < SubChunck::size; ++z)
			{
				Block* block = GetBlock({ x, y, z });
				if (block->type != Block::Type::air)
				{
					m_isEmpty = false;
					return;
				}
			}
	m_isEmpty = true;
}

void SubChunck::SetEnabled(bool state)
{
	m_enabled = state;
	STATS_enabled = m_enabled;
}

void SubChunck::GenerateCollider()
{
	m_colliderGenerated = true;
	if (!m_isEmpty)
	{

		std::vector<btVector3> btVertices;
		btVertices.reserve(SubChunck::size*SubChunck::size*SubChunck::size);

		std::vector<Mesh::Vertex> vertices;
		for (int x = 0; x < SubChunck::size; ++x)
			for (int y = 0; y < SubChunck::size; ++y)
				for (int z = 0; z < SubChunck::size; ++z)
				{
					Block* block = GetBlock({ x, y, z });
					if (block->solid)
					{
						glm::ivec3 pos = m_position * SubChunck::size + glm::ivec3(x, y + 1, z);
						Block* otherBlock = World::GetBlock(pos);
						if (!otherBlock || !otherBlock->solid)
						{
							std::vector<Mesh::Vertex> topFace = Cube::cubeTopFace(Block::size, (float)x, (float)y, (float)z, TexturesBlocks::Top(block->type));
							vertices.insert(vertices.end(), topFace.begin(), topFace.end());
						}

						pos = m_position * SubChunck::size + glm::ivec3(x, y - 1, z);
						otherBlock = World::GetBlock(pos);
						if (!otherBlock || !otherBlock->solid)
						{
							std::vector<Mesh::Vertex> botFace = Cube::cubeBotFace(Block::size, (float)x, (float)y, (float)z, TexturesBlocks::Bot(block->type));
							vertices.insert(vertices.end(), botFace.begin(), botFace.end());
						}

						pos = m_position * SubChunck::size + glm::ivec3(x - 1, y, z);
						otherBlock = World::GetBlock(pos);
						if (!otherBlock || !otherBlock->solid)
						{
							std::vector<Mesh::Vertex> leftFace = Cube::cubeLeftFace(Block::size, (float)x, (float)y, (float)z, TexturesBlocks::Left(block->type));
							vertices.insert(vertices.end(), leftFace.begin(), leftFace.end());
						}

						pos = m_position * SubChunck::size + glm::ivec3(x + 1, y, z);
						otherBlock = World::GetBlock(pos);
						if (!otherBlock || !otherBlock->solid)
						{
							std::vector<Mesh::Vertex> rightFace = Cube::cubeRightFace(Block::size, (float)x, (float)y, (float)z, TexturesBlocks::Right(block->type));
							vertices.insert(vertices.end(), rightFace.begin(), rightFace.end());
						}

						pos = m_position * SubChunck::size + glm::ivec3(x, y, z - 1);
						otherBlock = World::GetBlock(pos);
						if (!otherBlock || !otherBlock->solid)
						{
							std::vector<Mesh::Vertex> backFace = Cube::cubeBackFace(Block::size, (float)x, (float)y, (float)z, TexturesBlocks::Back(block->type));
							vertices.insert(vertices.end(), backFace.begin(), backFace.end());
						}

						pos = m_position * SubChunck::size + glm::ivec3(x, y, z + 1);
						otherBlock = World::GetBlock(pos);
						if (!otherBlock || !otherBlock->solid)
						{
							std::vector<Mesh::Vertex> frontFace = Cube::cubeFrontFace(Block::size, (float)x, (float)y, (float)z, TexturesBlocks::Front(block->type));
							vertices.insert(vertices.end(), frontFace.begin(), frontFace.end());
						}
					}
				}

		if (m_rb) Physics::DeleteRigidBody(m_rb);

		//Get vertices
		for (Mesh::Vertex vertex : vertices)
			btVertices.push_back(btVector3((float)m_position.x, (float)m_position.y, (float)m_position.z)*(float)SubChunck::size*Block::size + btVector3(vertex.vertex.x, vertex.vertex.y, vertex.vertex.z));

		if (btVertices.size() >= 3)
		{
			m_btMesh = btTriangleMesh();
			for (int i = 0; i < (int)btVertices.size() / 3; ++i)
				m_btMesh.addTriangle((btVertices)[3 * i + 0], (btVertices)[3 * i + 1], (btVertices)[3 * i + 2]);

			if (m_shape) delete(m_shape);
			m_shape = new btBvhTriangleMeshShape(&m_btMesh, false);
			btTransform transform = btTransform::getIdentity();
			m_rb = Physics::CreateRigidBody(0, transform, m_shape);
			m_rb->SetTag(Tag::chunck);
		}
	}

	
}

void SubChunck::GenerateMesh()
{
	if (!m_isEmpty)
	{
		std::vector<Mesh::Vertex>* targetVertices = nullptr;

		for (int x = 0; x < SubChunck::size; ++x)
			for (int y = 0; y < SubChunck::size; ++y)
				for (int z = 0; z < SubChunck::size; ++z)
				{
					Block* block = GetBlock({ x, y, z });

					//Set target
					if (block->transparent)
						targetVertices = &m_verticesTransparent;
					else
						targetVertices = &m_verticesOpaque;

					//Create vertices
					if (block->solid)
					{
						if (block->type == Block::Type::leaf)
						{
							std::vector<Mesh::Vertex> cube = Cube::CreateCubeMesh(14.f * Block::size / 16.f, Block::Type::leaf, (float)x, (float)y, (float)z);
							targetVertices->insert(targetVertices->end(), cube.begin(), cube.end());
						}
						else//Regular block
						{
							glm::ivec3 pos = m_position * SubChunck::size + glm::ivec3(x, y + 1, z);
							Block * otherBlock = World::GetBlock(pos);
							if (!otherBlock || (!otherBlock->solid || (otherBlock->seeThrough && (otherBlock->type != block->type || (!otherBlock->transparent || !block->transparent)))))
							{
								std::vector<Mesh::Vertex> topFace = Cube::cubeTopFace(Block::size, (float)x, (float)y, (float)z, TexturesBlocks::Top(block->type));
								targetVertices->insert(targetVertices->end(), topFace.begin(), topFace.end());
							}

							pos = m_position * SubChunck::size + glm::ivec3(x, y - 1, z);
							otherBlock = World::GetBlock(pos);
							if (otherBlock && (!otherBlock->solid || (otherBlock->seeThrough && (otherBlock->type != block->type || (!otherBlock->transparent || !block->transparent)))))
							{
								std::vector<Mesh::Vertex> botFace = Cube::cubeBotFace(Block::size, (float)x, (float)y, (float)z, TexturesBlocks::Bot(block->type));
								targetVertices->insert(targetVertices->end(), botFace.begin(), botFace.end());
							}

							pos = m_position * SubChunck::size + glm::ivec3(x - 1, y, z);
							otherBlock = World::GetBlock(pos);
							if (otherBlock && (!otherBlock->solid || (otherBlock->seeThrough && (otherBlock->type != block->type || (!otherBlock->transparent || !block->transparent)))))
							{
								std::vector<Mesh::Vertex> leftFace = Cube::cubeLeftFace(Block::size, (float)x, (float)y, (float)z, TexturesBlocks::Left(block->type));
								targetVertices->insert(targetVertices->end(), leftFace.begin(), leftFace.end());
							}

							pos = m_position * SubChunck::size + glm::ivec3(x + 1, y, z);
							otherBlock = World::GetBlock(pos);
							if (otherBlock && (!otherBlock->solid || (otherBlock->seeThrough && (otherBlock->type != block->type || (!otherBlock->transparent || !block->transparent)))))
							{
								std::vector<Mesh::Vertex> rightFace = Cube::cubeRightFace(Block::size, (float)x, (float)y, (float)z, TexturesBlocks::Right(block->type));
								targetVertices->insert(targetVertices->end(), rightFace.begin(), rightFace.end());
							}

							pos = m_position * SubChunck::size + glm::ivec3(x, y, z - 1);
							otherBlock = World::GetBlock(pos);
							if (otherBlock && (!otherBlock->solid || (otherBlock->seeThrough && (otherBlock->type != block->type || (!otherBlock->transparent || !block->transparent)))))
							{
								std::vector<Mesh::Vertex> backFace = Cube::cubeBackFace(Block::size, (float)x, (float)y, (float)z, TexturesBlocks::Back(block->type));
								targetVertices->insert(targetVertices->end(), backFace.begin(), backFace.end());
							}

							pos = m_position * SubChunck::size + glm::ivec3(x, y, z + 1);
							otherBlock = World::GetBlock(pos);
							if (otherBlock && (!otherBlock->solid || (otherBlock->seeThrough && (otherBlock->type != block->type || (!otherBlock->transparent || !block->transparent)))))
							{
								std::vector<Mesh::Vertex> frontFace = Cube::cubeFrontFace(Block::size, (float)x, (float)y, (float)z, TexturesBlocks::Front(block->type));
								targetVertices->insert(targetVertices->end(), frontFace.begin(), frontFace.end());
							}
						}

					}
				}
	}
}

void SubChunck::GenerateModels()
{
	STATS_triangles = 0;

	//Generates opaque
	if (m_modelOpaque) delete(m_modelOpaque);
	m_modelOpaque = new Model(m_verticesOpaque);
	m_modelOpaque->Translate(SubChunck::size * Block::size * glm::vec3(m_position.x, m_position.y, m_position.z));
	STATS_triangles += m_verticesOpaque.size() / 3;
	m_verticesOpaque.clear();
	m_verticesOpaque.shrink_to_fit();

	//Generates transparent
	if (m_modelTransparent) delete(m_modelTransparent);
	m_modelTransparent = new Model(m_verticesTransparent);
	m_modelTransparent->Translate(SubChunck::size * Block::size * glm::vec3(m_position.x, m_position.y, m_position.z));
	STATS_triangles += m_verticesTransparent.size() / 3;
	m_verticesTransparent.clear();
	m_verticesTransparent.shrink_to_fit();

}


void SubChunck::DrawTransparent(const Shader & shader) const
{
	if(m_modelTransparent && m_enabled)
		m_modelTransparent->Draw(shader);
}

void SubChunck::DrawOpaque(const Shader & shader) const
{
	if(m_modelOpaque && m_enabled)
		m_modelOpaque->Draw(shader);
}

SubChunck::~SubChunck()
{
	if (m_modelOpaque) delete(m_modelOpaque);
	if (m_modelTransparent) delete(m_modelTransparent);
	if (m_rb) Physics::DeleteRigidBody(m_rb);
	if (m_shape) delete(m_shape);
}
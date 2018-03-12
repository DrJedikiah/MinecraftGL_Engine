#include "engine/Chunck.h"


Chunck::Chunck() : 
	m_modelOpaque(nullptr),
	m_modelTransparent(nullptr),
	m_shape(nullptr),
	m_rb(nullptr)
{
	m_btMesh.preallocateVertices(size*size*size);
	m_btMesh.preallocateIndices(size*size*size);
}

void Chunck::Update(float delta)
{
	if (m_generateLater)
	{
		GenerateMesh();
		GenerateCollider();
		m_generateLater = false;
	}
}

void Chunck::Setup(World* world, glm::ivec3 position)
{
	m_world = world;
	m_position = position;
}

void Chunck::DrawTransparent(const Shader & shader) const
{
	m_modelTransparent->Draw(shader);
}
void Chunck::DrawOpaque(const Shader & shader) const
{
	m_modelOpaque->Draw(shader);
}


void Chunck::GenerateLater()
{
	m_generateLater = true;
}

Block& Chunck::GetBlock(glm::ivec3 position)
{
	return m_blocks[position.x][position.y][position.z];
}


void Chunck::GenerateCollider()
{
	std::vector<btVector3> btVertices;
	btVertices.reserve(size*size*size);

	std::vector<Mesh::Vertex> vertices;
	for (int y = 0; y < size; ++y)
		for (int z = 0; z < size; ++z)
			for (int x = 0; x < size; ++x)
			{
				Block& block = GetBlock({ x, y, z });
				if (block.solid)
				{
					glm::ivec3 pos = m_position * size + glm::ivec3(x, y + 1, z);
					if (!World::BlockGenerated(pos) || !World::GetBlock(pos).solid)
					{
						std::vector<Mesh::Vertex> topFace = Cube::cubeTopFace(Block::size, (float)x, (float)y, (float)z, TexturesBlocks::Top(block.type));
						vertices.insert(vertices.end(), topFace.begin(), topFace.end());
					}

					pos = m_position * size + glm::ivec3(x, y - 1, z);
					if (!World::BlockGenerated(pos) || !World::GetBlock(pos).solid)
					{
						std::vector<Mesh::Vertex> botFace = Cube::cubeBotFace(Block::size, (float)x, (float)y, (float)z, TexturesBlocks::Bot(block.type));
						vertices.insert(vertices.end(), botFace.begin(), botFace.end());
					}

					pos = m_position * size + glm::ivec3(x - 1, y, z);
					if (!World::BlockGenerated(pos) || !World::GetBlock(pos).solid)
					{
						std::vector<Mesh::Vertex> leftFace = Cube::cubeLeftFace(Block::size, (float)x, (float)y, (float)z, TexturesBlocks::Left(block.type));
						vertices.insert(vertices.end(), leftFace.begin(), leftFace.end());
					}

					pos = m_position * size + glm::ivec3(x + 1, y, z);
					if (!World::BlockGenerated(pos) || !World::GetBlock(pos).solid)
					{
						std::vector<Mesh::Vertex> rightFace = Cube::cubeRightFace(Block::size, (float)x, (float)y, (float)z, TexturesBlocks::Right(block.type));
						vertices.insert(vertices.end(), rightFace.begin(), rightFace.end());
					}

					pos = m_position * size + glm::ivec3(x, y, z - 1);
					if (!World::BlockGenerated(pos) || !World::GetBlock(pos).solid)
					{
						std::vector<Mesh::Vertex> backFace = Cube::cubeBackFace(Block::size, (float)x, (float)y, (float)z, TexturesBlocks::Back(block.type));
						vertices.insert(vertices.end(), backFace.begin(), backFace.end());
					}

					pos = m_position * size + glm::ivec3(x, y, z + 1);
					if (!World::BlockGenerated(pos) || !World::GetBlock(pos).solid)
					{
						std::vector<Mesh::Vertex> frontFace = Cube::cubeFrontFace(Block::size, (float)x, (float)y, (float)z, TexturesBlocks::Front(block.type));
						vertices.insert(vertices.end(), frontFace.begin(), frontFace.end());
					}
				}
			}

	if( m_rb) PhysicsEngine::DeleteRigidBody(m_rb);

	//Get vertices
	for (Mesh::Vertex vertex : vertices)
		btVertices.push_back(btVector3((float)m_position .x, (float)m_position .y, (float)m_position .z)*(float)size*Block::size + btVector3(vertex.vertex.x, vertex.vertex.y, vertex.vertex.z));
	
	if (btVertices.size() >= 3)
	{
		m_btMesh = btTriangleMesh();
		for (int i = 0; i < (int)btVertices.size() / 3; ++i)
			m_btMesh.addTriangle((btVertices)[3 * i + 0], (btVertices)[3 * i + 1], (btVertices)[3 * i + 2]);

		if (m_shape) delete(m_shape);
		m_shape = new btBvhTriangleMeshShape(&m_btMesh, false);
		btTransform transform = btTransform::getIdentity();
		m_rb = PhysicsEngine::CreateRigidBody(0, transform, m_shape);
		m_rb->SetTag(Tag::chunck);
	}
	
}

void Chunck::GenerateMesh()
{
	std::vector<Mesh::Vertex> verticesOpaque;
	std::vector<Mesh::Vertex> verticesTransparent;
	std::vector<Mesh::Vertex>* targetVertices = nullptr;


	for (int y = 0; y < size; ++y)
		for (int z = 0; z < size; ++z)
			for (int x = 0; x < size; ++x)
			{
				Block& block = GetBlock({ x, y, z });

				//Set target
				if (block.transparent)
					targetVertices = &verticesTransparent;
				else
					targetVertices = &verticesOpaque;

				//Create vertices
				if(block.solid)
				{
					if (block.type == Block::Type::leaf)
					{
						std::vector<Mesh::Vertex> cube = Cube::CreateCubeMesh( 14.f * Block::size / 16.f , Block::Type::leaf, (float)x, (float)y, (float)z);
						targetVertices->insert(targetVertices->end(), cube.begin(), cube.end());
					}
					else//Regular block
					{
						glm::ivec3 pos = m_position * size + glm::ivec3(x, y + 1, z);
						Block& otherBlock = World::GetBlock(pos);
						if (!World::BlockGenerated(pos) || !otherBlock.solid || (otherBlock.seeThrough && (otherBlock.type != block.type || ( ! otherBlock.transparent || ! block.transparent))) )
						{
							std::vector<Mesh::Vertex> topFace = Cube::cubeTopFace(Block::size, (float)x, (float)y, (float)z, TexturesBlocks::Top(block.type));
							targetVertices->insert(targetVertices->end(), topFace.begin(), topFace.end());
						}

						pos = m_position * size + glm::ivec3(x, y - 1, z);
						Block& otherBlock2 = World::GetBlock(pos);
						if (!World::BlockGenerated(pos) || !otherBlock2.solid || (otherBlock2.seeThrough && (otherBlock2.type != block.type || (!otherBlock2.transparent || !block.transparent))))
						{
							std::vector<Mesh::Vertex> botFace = Cube::cubeBotFace(Block::size, (float)x, (float)y, (float)z, TexturesBlocks::Bot(block.type));
							targetVertices->insert(targetVertices->end(), botFace.begin(), botFace.end());
						}

						pos = m_position * size + glm::ivec3(x - 1, y, z);
						Block& otherBlock3 = World::GetBlock(pos);
						if (!World::BlockGenerated(pos) || !otherBlock3.solid || (otherBlock3.seeThrough && (otherBlock3.type != block.type || (!otherBlock3.transparent || !block.transparent))))
						{
							std::vector<Mesh::Vertex> leftFace = Cube::cubeLeftFace(Block::size, (float)x, (float)y, (float)z, TexturesBlocks::Left(block.type));
							targetVertices->insert(targetVertices->end(), leftFace.begin(), leftFace.end());
						}

						pos = m_position * size + glm::ivec3(x + 1, y, z);
						Block& otherBlock4 = World::GetBlock(pos);
						if (!World::BlockGenerated(pos) || !otherBlock4.solid || (otherBlock4.seeThrough && (otherBlock4.type != block.type || (!otherBlock4.transparent || !block.transparent))))
						{
							std::vector<Mesh::Vertex> rightFace = Cube::cubeRightFace(Block::size, (float)x, (float)y, (float)z, TexturesBlocks::Right(block.type));
							targetVertices->insert(targetVertices->end(), rightFace.begin(), rightFace.end());
						}

						pos = m_position * size + glm::ivec3(x, y, z - 1);
						Block& otherBlock5 = World::GetBlock(pos);
						if (!World::BlockGenerated(pos) || !otherBlock5.solid || (otherBlock5.seeThrough && (otherBlock5.type != block.type || (!otherBlock5.transparent || !block.transparent))))
						{
							std::vector<Mesh::Vertex> backFace = Cube::cubeBackFace(Block::size, (float)x, (float)y, (float)z, TexturesBlocks::Back(block.type));
							targetVertices->insert(targetVertices->end(), backFace.begin(), backFace.end());
						}

						pos = m_position * size + glm::ivec3(x, y, z + 1);
						Block& otherBlock6 = World::GetBlock(pos);
						if (!World::BlockGenerated(pos) || !otherBlock6.solid || (otherBlock6.seeThrough && (otherBlock6.type != block.type || (!otherBlock6.transparent || !block.transparent))))
						{
							std::vector<Mesh::Vertex> frontFace = Cube::cubeFrontFace(Block::size, (float)x, (float)y, (float)z, TexturesBlocks::Front(block.type));
							targetVertices->insert(targetVertices->end(), frontFace.begin(), frontFace.end());
						}
					}

				}
	}

	//Delete previous model
	if (m_modelOpaque) delete(m_modelOpaque);
	if (m_modelTransparent) delete(m_modelTransparent);


	//Set new models
	m_modelOpaque = new Model(verticesOpaque);
	m_modelOpaque->Translate(Chunck::size * Block::size * glm::vec3(m_position.x, m_position.y, m_position.z));
	m_modelTransparent = new Model(verticesTransparent);
	m_modelTransparent->Translate(Chunck::size * Block::size * glm::vec3(m_position.x, m_position.y, m_position.z));

	//stats
	STATS_triangles = verticesOpaque.size() / 3 + verticesTransparent.size() / 3;
}

Chunck::~Chunck()
{
	if (m_modelOpaque) delete(m_modelOpaque);
	if (m_modelTransparent) delete(m_modelTransparent);

	if (m_shape) delete(m_shape);
}



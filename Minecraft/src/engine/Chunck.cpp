#include "engine/Chunck.h"


Chunck::Chunck() : 
	m_model(nullptr),
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

void Chunck::Draw(const Shader & shader) const
{
	m_model->Draw(shader);
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
	if( m_rb) PhysicsEngine::DeleteRigidBody(m_rb);

	std::vector<btVector3> btVertices;
	btVertices.reserve(size*size*size);

	//Get vertices
	btVertices.clear();
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
	vertices.clear();
	for (int y = 0; y < size; ++y)
		for (int z = 0; z < size; ++z)
			for (int x = 0; x < size; ++x)
			{
				Block& block = GetBlock({ x, y, z });
				if(block.enabled && block.solid)
				{
					glm::ivec3 pos = m_position * size + glm::ivec3(x, y + 1, z);
					if ( ! World::BlockGenerated(pos) || ! World::GetBlock(pos).solid)
					{
						std::vector<Mesh::Vertex> topFace = Util::cubeTopFace(Block::size, (float)x, (float)y, (float)z, TexturesBlocks::Top(block.type));
						vertices.insert(vertices.end(), topFace.begin(), topFace.end());
					}
						
					pos = m_position * size + glm::ivec3(x, y - 1, z);
					if (!World::BlockGenerated(pos) || ! World::GetBlock(pos).solid)
					{
						std::vector<Mesh::Vertex> botFace = Util::cubeBotFace(Block::size, (float)x, (float)y, (float)z, TexturesBlocks::Bot(block.type));
						vertices.insert(vertices.end(), botFace.begin(), botFace.end());
					}
					
					pos = m_position * size + glm::ivec3(x-1, y, z);
					if (!World::BlockGenerated(pos) || !World::GetBlock(pos).solid)
					{
						std::vector<Mesh::Vertex> leftFace = Util::cubeLeftFace(Block::size, (float)x, (float)y, (float)z, TexturesBlocks::Left(block.type));
						vertices.insert(vertices.end(), leftFace.begin(), leftFace.end());
					}

					pos = m_position * size + glm::ivec3(x+1, y , z);
					if (!World::BlockGenerated(pos) || !World::GetBlock(pos).solid)
					{
						std::vector<Mesh::Vertex> rightFace = Util::cubeRightFace(Block::size, (float)x, (float)y, (float)z, TexturesBlocks::Right(block.type));
						vertices.insert(vertices.end(), rightFace.begin(), rightFace.end());
					}

					pos = m_position * size + glm::ivec3(x, y, z-1);
					if (!World::BlockGenerated(pos) || !World::GetBlock(pos).solid)
					{
						std::vector<Mesh::Vertex> backFace = Util::cubeBackFace(Block::size, (float)x, (float)y, (float)z, TexturesBlocks::Back(block.type));
						vertices.insert(vertices.end(), backFace.begin(), backFace.end());
					}

					pos = m_position * size + glm::ivec3(x, y, z+1);
					if (!World::BlockGenerated(pos) || !World::GetBlock(pos).solid)
					{
						std::vector<Mesh::Vertex> frontFace = Util::cubeFrontFace(Block::size, (float)x, (float)y, (float)z, TexturesBlocks::Front(block.type));
						vertices.insert(vertices.end(), frontFace.begin(), frontFace.end());
					}
				}
	}
	if (m_model) delete(m_model);

	m_model = new  Model(vertices);
	m_model->Translate(Chunck::size * Block::size * glm::vec3(m_position.x, m_position.y, m_position.z));

}

Chunck::~Chunck()
{
	if (m_model) delete(m_model);
	if (m_shape) delete(m_shape);
}



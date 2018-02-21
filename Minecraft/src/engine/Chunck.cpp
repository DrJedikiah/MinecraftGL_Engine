#include "engine/Chunck.h"


Chunck::Chunck() : m_model({})
{
	m_btMesh.preallocateVertices(size*size*size);
	m_btMesh.preallocateIndices(size*size*size);
}

void Chunck::Setup(glm::ivec3 position)
{
	m_position = position;
}

void Chunck::Draw(const Shader& shader) const
{
	m_model.Draw(shader);
}

Block& Chunck::GetBlock(int x, int y, int z)
{
	return m_blocks[x][y][z];
}

void Chunck::GenerateCollider(PhysicsEngine & physicsEngine)
{
	std::vector<btVector3> btVertices;
	btVertices.reserve(size*size*size);

	//Get vertices
	btVertices.clear();
	for (Vertex vertex : vertices)
		btVertices.push_back(btVector3((float)m_position .x, (float)m_position .y, (float)m_position .z)*(float)size*Block::size + btVector3(vertex.vertex.x, vertex.vertex.y, vertex.vertex.z));

	if (btVertices.size() >= 3)
	{
		m_btMesh = btTriangleMesh();
		for (int i = 0; i < (int)btVertices.size() / 3; ++i)
			m_btMesh.addTriangle((btVertices)[3 * i + 0], (btVertices)[3 * i + 1], (btVertices)[3 * i + 2]);

		m_shape = new btBvhTriangleMeshShape(&m_btMesh, false);
		btTransform transform = btTransform::getIdentity();
		physicsEngine.CreateRigidBody(0, transform, m_shape);

	}

}

void Chunck::GenerateMesh(World& world)
{
	vertices.clear();
	for (int y = 0; y < size; ++y)
		for (int z = 0; z < size; ++z)
			for (int x = 0; x < size; ++x)
			{
				Block& block = GetBlock(x, y, z);
				if(block.enabled)
				{

					if ((y + 1 < size && !GetBlock(x, y + 1, z).solid) || 
						(world.BlockExists( m_position*size + glm::ivec3( x, y + 1, z )) && !world.GetBlock(m_position*size + glm::ivec3(x, y + 1, z)).solid))
					{
						std::vector<Vertex> topFace = Util::cubeTopFace(Block::size, (float)x, (float)y, (float)z, TexturesBlocks::Top(block.type));
						vertices.insert(vertices.end(), topFace.begin(), topFace.end());
					}
						
					if ((y - 1 >= 0 && !GetBlock(x, y - 1, z).solid) ||
						(world.BlockExists(m_position*size + glm::ivec3(x, y - 1, z)) && ! world.GetBlock(m_position*size + glm::ivec3(x, y - 1, z)).solid))
					{
						std::vector<Vertex> botFace = Util::cubeBotFace(Block::size, (float)x, (float)y, (float)z, TexturesBlocks::Bot(block.type));
						vertices.insert(vertices.end(), botFace.begin(), botFace.end());
					}

					if ((x - 1 >= 0 && !GetBlock(x - 1, y, z).solid) ||
					   (world.BlockExists(m_position*size + glm::ivec3(x-1, y, z)) && !world.GetBlock(m_position*size + glm::ivec3(x-1, y, z)).solid))
					{
						std::vector<Vertex> leftFace = Util::cubeLeftFace(Block::size, (float)x, (float)y, (float)z, TexturesBlocks::Left(block.type));
						vertices.insert(vertices.end(), leftFace.begin(), leftFace.end());
					}

					if ((x + 1 < size && !GetBlock(x + 1, y, z).solid) ||
						(world.BlockExists(m_position*size + glm::ivec3(x + 1, y, z)) && !world.GetBlock(m_position*size + glm::ivec3(x + 1, y, z)).solid))
					{
						std::vector<Vertex> rightFace = Util::cubeRightFace(Block::size, (float)x, (float)y, (float)z, TexturesBlocks::Right(block.type));
						vertices.insert(vertices.end(), rightFace.begin(), rightFace.end());
					}

					if ((z - 1 >= 0 && !GetBlock(x, y, z - 1).solid) ||
						(world.BlockExists(m_position*size + glm::ivec3(x, y, z-1)) && !world.GetBlock(m_position*size + glm::ivec3(x, y, z-1)).solid))
					{
						std::vector<Vertex> backFace = Util::cubeBackFace(Block::size, (float)x, (float)y, (float)z, TexturesBlocks::Back(block.type));
						vertices.insert(vertices.end(), backFace.begin(), backFace.end());
					}

					if ((z + 1 < size && !GetBlock(x, y, z + 1).solid) ||
						(world.BlockExists(m_position*size + glm::ivec3(x, y, z + 1)) && !world.GetBlock(m_position*size + glm::ivec3(x, y, z + 1)).solid))
					{
						std::vector<Vertex> frontFace = Util::cubeFrontFace(Block::size, (float)x, (float)y, (float)z, TexturesBlocks::Front(block.type));
						vertices.insert(vertices.end(), frontFace.begin(), frontFace.end());
					}
				}
	}
	m_model = Model({ Mesh(vertices) });
}
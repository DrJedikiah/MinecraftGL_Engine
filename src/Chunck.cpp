#include "Chunck.h"


Chunck::Chunck() : m_model({})
{
}

void Chunck::Setup(btVector3 position)
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
	//Get vertices
	std::vector<btVector3> * btVertices = new std::vector<btVector3>();
	btVertices->reserve(size*size*size);
	for (Vertex vertex : vertices)
		btVertices->push_back(m_position*size*Block::size + btVector3(vertex.vertex.x, vertex.vertex.y, vertex.vertex.z));

	btTriangleMesh * mesh = new btTriangleMesh();
	mesh->preallocateVertices(size*size*size);
	mesh->preallocateIndices(size*size*size);

	for (int i = 0; i < btVertices->size() / 3; ++i)
		mesh->addTriangle((*btVertices)[3 * i + 0], (*btVertices)[3 * i + 1], (*btVertices)[3 * i + 2]);

	btBvhTriangleMeshShape * shape = new btBvhTriangleMeshShape(mesh, false);
	btTransform transform = btTransform::getIdentity();
	physicsEngine.CreateRigidBody(0, transform, shape); 
}

void Chunck::GenerateMesh()
{
	fRect dirt = BlockTiles::GetRectangle(BlockTiles::grassSide);
	vertices.clear();
	
	for (int y = 0; y < size; ++y)
		for (int z = 0; z < size; ++z)
			for (int x = 0; x < size; ++x)
			{
				Block& cube = GetBlock(x, y, z);
				if( cube.IsEnabled())
				{
					if (y + 1 == size || !GetBlock(x, y + 1, z).IsSolid())
					{
						std::vector<Vertex> topFace = Util::cubeTopFace(Block::size, (float)x, (float)y, (float)z, dirt);
						vertices.insert(vertices.end(), topFace.begin(), topFace.end());
					}
						
					if (y - 1 == -1 || !GetBlock(x, y + 1, z).IsSolid())
					{
						std::vector<Vertex> botFace = Util::cubeBotFace(Block::size, (float)x, (float)y, (float)z, dirt);
						vertices.insert(vertices.end(), botFace.begin(), botFace.end());
					}

					if (x - 1 == -1 || !GetBlock(x - 1, y, z).IsSolid())
					{
						std::vector<Vertex> leftFace = Util::cubeLeftFace(Block::size, (float)x, (float)y, (float)z, dirt);
						vertices.insert(vertices.end(), leftFace.begin(), leftFace.end());
					}

					if (x + 1 == size || !GetBlock(x + 1, y, z).IsSolid())
					{
						std::vector<Vertex> rightFace = Util::cubeRightFace(Block::size, (float)x, (float)y, (float)z, dirt);
						vertices.insert(vertices.end(), rightFace.begin(), rightFace.end());
					}

					if (z - 1 == -1 || !GetBlock(x, y, z - 1).IsSolid())
					{
						std::vector<Vertex> backFace = Util::cubeBackFace(Block::size, (float)x, (float)y, (float)z, dirt);
						vertices.insert(vertices.end(), backFace.begin(), backFace.end());

					}

					if (z + 1 == size || !GetBlock(x, y, z + 1).IsSolid())
					{
						std::vector<Vertex> frontFace = Util::cubeFrontFace(Block::size, (float)x, (float)y, (float)z, dirt);
						vertices.insert(vertices.end(), frontFace.begin(), frontFace.end());
					}
				}
	}
	m_model = Model({ Mesh(vertices) });
}
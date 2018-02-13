#include "Chunck.h"

const int Chunck::size = 16;

Chunck::Chunck() : m_model({})
{
	m_cubes.resize(size*size*size);
}

void Chunck::Draw(const Shader& shader) const
{
	m_model.Draw(shader);
}

Cube& Chunck::GetCube(int x, int y, int z)
{
	return m_cubes[x + z * size + y * size * size];
}

void Chunck::GenerateMesh()
{
	fRect dirt = BlockTiles::GetRectangle(BlockTiles::grassTop);

	std::vector<Vertex> vertices;
	for (int y = 0; y < size; ++y)
		for (int z = 0; z < size; ++z)
			for (int x = 0; x < size; ++x)
			{
				Cube& cube = GetCube(x, y, z);
				if( cube.IsEnabled())
				{
					if (y + 1 == size || !GetCube(x, y + 1, z).IsSolid())
					{
						std::vector<Vertex> topFace = Util::cubeTopFace(Cube::size, (float)x, (float)y, (float)z, dirt);
						vertices.insert(vertices.end(), topFace.begin(), topFace.end());
					}
						
					if (y - 1 == -1 || !GetCube(x, y + 1, z).IsSolid())
					{
						std::vector<Vertex> botFace = Util::cubeBotFace(Cube::size, (float)x, (float)y, (float)z, dirt);
						vertices.insert(vertices.end(), botFace.begin(), botFace.end());
					}

					if (x - 1 == -1 || !GetCube(x - 1, y, z).IsSolid())
					{
						std::vector<Vertex> leftFace = Util::cubeLeftFace(Cube::size, (float)x, (float)y, (float)z, dirt);
						vertices.insert(vertices.end(), leftFace.begin(), leftFace.end());
					}

					if (x + 1 == size || !GetCube(x + 1, y, z).IsSolid())
					{
						std::vector<Vertex> rightFace = Util::cubeRightFace(Cube::size, (float)x, (float)y, (float)z, dirt);
						vertices.insert(vertices.end(), rightFace.begin(), rightFace.end());
					}

					if (z - 1 == -1 || !GetCube(x, y, z - 1).IsSolid())
					{
						std::vector<Vertex> backFace = Util::cubeBackFace(Cube::size, (float)x, (float)y, (float)z, dirt);
						vertices.insert(vertices.end(), backFace.begin(), backFace.end());

					}

					if (z + 1 == size || !GetCube(x, y, z + 1).IsSolid())
					{
						std::vector<Vertex> frontFace = Util::cubeFrontFace(Cube::size, (float)x, (float)y, (float)z, dirt);
						vertices.insert(vertices.end(), frontFace.begin(), frontFace.end());
					}
				}
	}
	m_model = Model({ Mesh(vertices) });
}
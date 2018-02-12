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
	std::vector<Vertex> vertices;
	for (int y = 0; y < size; ++y)
	{
		for (int z = 0; z < size; ++z)
		{
			for (int x = 0; x < size; ++x)
			{
				if( GetCube( x, y, z).type != Cube::Type::air)
				{
					float s = Cube::size / 2;
					float X = x * s * 2;
					float Y = y * s * 2;
					float Z = z * s * 2;
					std::vector<Vertex> cube =
					{
						//Back
						{ { X - s, Y - s, Z - s },{ 0.f,0.f,-1.f },{ 0.f, 0.5f } },//bot
						{ { X + s, Y + s, Z - s },{ 0.f,0.f,-1.f },{ 0.5f, 1.0f } },
						{ { X + s, Y - s, Z - s },{ 0.f,0.f,-1.f },{ 0.5f, 0.5f } },
						{ { X + s, Y + s, Z - s },{ 0.f,0.f,-1.f },{ 0.5f, 1.0f } },//Top
						{ { X - s, Y - s, Z - s },{ 0.f,0.f,-1.f },{ 0.f, 0.5f } },
						{ { X - s, Y + s, Z - s },{ 0.f,0.f,-1.f },{ 0.f, 1.0f } },

						//Front
						{ { X - s, Y - s, Z + s },{ 0.f,0.f,1.f },{ 0.f, 0.5f } },//bot
						{ { X + s,Y - s, Z + s },{ 0.f,0.f,1.f },{ 0.5f, 0.5f } },
						{ { X + s, Y + s,Z + s },{ 0.f,0.f,1.f },{ s, 1.0f } },
						{ { X + s, Y + s, Z + s },{ 0.f,0.f,1.f },{ 0.5f, 1.0f } },//Top
						{ { X - s, Y + s, Z + s },{ 0.f,0.f,1.f },{ 0.f, 1.0f } },
						{ { X - s, Y - s,Z + s },{ 0.f,0.f,1.f },{ 0.f, 0.5f } },

						//Left
						{ { X - s, Y + s, Z + s },{ -1.f,0.f,0.f },{ 0.f, 1.f } },//Top
						{ { X - s, Y + s, Z - s },{ -1.f,0.f,0.f },{ 0.5f, 1.f } },
						{ { X - s, Y - s, Z - s },{ -1.f,0.f,0.f },{ 0.5f, 0.5f } },
						{ { X - s, Y - s, Z - s },{ -1.f,0.f,0.f },{ 0.5f, 0.5f } },//Bot
						{ { X - s, Y - s, Z + s },{ -1.f,0.f,0.f },{ 0.f, 0.5f } },
						{ { X - s, Y + s, Z + s },{ -1.f,0.f,0.f },{ 0.f, 1.f } },

						//Right
						{ { X + s, Y + s, Z + s },{ 1.f,0.f,0.f },{ 0.f, 1.f } },//Top
						{ { X + s, Y - s, Z - s },{ 1.f,0.f,0.f },{ 0.5f, 0.5f } },
						{ { X + s, Y + s, Z - s },{ 1.f,0.f,0.f },{ 0.5f, 1.f } },
						{ { X + s, Y - s, Z - s },{ 1.f,0.f,0.f },{ 0.5f, 0.5f } },//Bot
						{ { X + s, Y + s, Z + s },{ 1.f,0.f,0.f },{ 0.f, 1.f } },
						{ { X + s, Y - s, Z + s },{ 1.f,0.f,0.f },{ 0.f, 0.5f } },

						//Bot
						{ { X - s, Y - s, Z - s },{ 0.f,-1.f,0.f },{ 0.5f, 1.0f } },
						{ { X + s, Y - s, Z - s },{ 0.f,-1.f,0.f },{ 1.0f, 1.0f } },
						{ { X + s, Y - s, Z + s },{ 0.f,-1.f,0.f },{ 1.0f, 0.5f } },
						{ { X + s, Y - s, Z + s },{ 0.f,-1.f,0.f },{ 1.0f, 0.5f } },
						{ { X - s, Y - s, Z + s },{ 0.f,-1.f,0.f },{ 0.5f, 0.5f } },
						{ { X - s, Y - s, Z - s },{ 0.f,-1.f,0.f },{ 0.5f, 1.0f } },

						//Top
						{ { X + s,  Y + s, Z - s },{ 0.f,1.f,0.f },{ 0.5f, 0.5f } },
						{ { X - s,  Y + s, Z - s },{ 0.f,1.f,0.f },{ 0.0f, 0.5f } },
						{ { X + s,  Y + s, Z + s },{ 0.f,1.f,0.f },{ 0.5f, 0.0f } },
						{ { X - s,  Y + s, Z + s },{ 0.f,1.f,0.f },{ 0.0f, 0.0f } },
						{ { X + s,  Y + s, Z + s },{ 0.f,1.f,0.f },{ 0.5f, 0.0f } },
						{ { X - s,  Y + s, Z - s },{ 0.f,1.f,0.f },{ 0.0f, 0.5f } }
					};
					vertices.insert(vertices.end(), cube.begin(), cube.end());
				}
			}
		}
	}
	m_model = Model({ Mesh(vertices) });
}
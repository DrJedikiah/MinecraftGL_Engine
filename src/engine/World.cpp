#include "engine/World.h"

World::World()
{
	perlinGen.reseed(42);

	m_chuncks = new Chunck**[size];
	for (int i = 0; i < size; ++i)
	{
		m_chuncks[i] = new Chunck*[height];
		for (int j = 0; j < height; ++j)
			m_chuncks[i][j] = new Chunck[size];
	}

	for (int y = 0; y < height; ++y)
		for (int z = 0; z < size; ++z)
			for (int x = 0; x < size; ++x)
				m_chuncks[x][y][z].Setup(btVector3((float)x, (float)y, (float)z));

	generate();
}



Chunck & World::GetChunck(int x, int y, int z)
{
	return m_chuncks[x][y][z];
}

Block & World::GetBlock(int x, int y, int z)
{
	return GetChunck(x / Chunck::size, y / Chunck::size, z / Chunck::size).GetBlock(x%Chunck::size, y%Chunck::size, z%Chunck::size);
}

void World::GeneratePhysics(PhysicsEngine & physicsEngine)
{
	for (int y = 0; y < height; ++y)
		for (int z = 0; z < size; ++z)
			for (int x = 0; x < size; ++x)
				m_chuncks[x][y][z].GenerateCollider(physicsEngine);
}

void World::generate()
{
	//Perlin cubes generation
	for (int y = 0; y < height * Chunck::size; ++y)
		for (int x = 0; x < size * Chunck::size; ++x)
			for (int z = 0; z < size * Chunck::size; ++z)
			{
				const int heightMap = height * Chunck::size;
				float density;
				if (y < heightMap / 100000 )
				{
					float scale3D = 1.f / 5.f;
					density = (float)perlinGen.noise((double)(scale3D * x), (double)(scale3D * y), (double)(scale3D * z));
				}
				else
				{
					float scale2D = 1.f / 10.f;
					int h = (int)(heightMap *(perlinGen.noise(scale2D*x, scale2D*z) + 1.f) / 2.f); 
					if (y > h)
						density = -1.f;
					else
						density = 1.f;

				}
				if (density > 0)
					GetBlock(x, y, z).SetType(Block::Type::dirt);
				else
					GetBlock(x, y, z).SetType(Block::Type::air);
			}

	//Removing hidden cubes
	for (int y = 1; y < height * Chunck::size - 1; ++y)
		for (int x = 1; x < size * Chunck::size - 1; ++x)
			for (int z = 1; z < size * Chunck::size  - 1; ++z)
			{
				Block& cube = GetBlock(x, y, z);
				if (GetBlock( x+1,	y,		z	).IsSolid() &&
					GetBlock( x-1,	y,		z	).IsSolid() &&
					GetBlock( x,		y+1,	z	).IsSolid() &&
					GetBlock( x,		y-1,	z	).IsSolid() &&
					GetBlock( x,		y,		z+1	).IsSolid() &&
					GetBlock( x,		y,		z-1	).IsSolid())
				{
					cube.SetEnabled(false);
				}

				if (cube.GetType() == Block::Type::dirt && !GetBlock(x, y + 1, z).IsSolid())
					cube.SetType(Block::Type::grass);
			}

	//Mesh generation
	for (int x = 0; x < size; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			for (int z = 0; z < size; ++z)
			{
				Chunck & chunck = GetChunck(x, y, z);
				chunck.GenerateMesh();
				chunck.m_model.Translate(Chunck::size * Block::size * glm::vec3{ x,y,z });
			}
		}
	}

}

void World::Draw(const Shader& shader) const
{
	for (int y = 0; y < height; ++y)
		for (int z = 0; z < size; ++z)
			for (int x = 0; x < size; ++x)
				m_chuncks[x][y][z].Draw(shader);
}
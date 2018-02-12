#include "World.h"

World::World()
{
	perlinGen.reseed(42);
	m_chuncks.resize(size * size * height);
	generate();
}

Chunck & World::getChunck(int x, int y, int z)
{
	return m_chuncks[x + z * size + y * size * size];
}

Cube & World::getCube(int x, int y, int z)
{
	return getChunck(x / Chunck::size, y / Chunck::size, z / Chunck::size).GetCube(x%Chunck::size, y%Chunck::size, z%Chunck::size);
}

void World::generate()
{	
	for (int y = 0; y < height * Chunck::size; ++y)
		for (int x = 0; x < size * Chunck::size; ++x)
			for (int z = 0; z < size * Chunck::size; ++z)
			{
				Cube & cube = getCube(x, y, z);

				float scale = 1.f / 10.f;
				if (perlinGen.noise(scale * x, scale * y, scale * z) > 0)
					cube.type = Cube::Type::air;
			}

	for (int x = 0; x < size; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			for (int z = 0; z < size; ++z)
			{
				Chunck & chunck = getChunck(x, y, z);
				chunck.GenerateMesh();
				chunck.m_model.Translate(Chunck::size * Cube::size * glm::vec3{ x,y,z });
			}
		}
	}

	//Set cubes

	//Generate mesh

}

void World::Draw(const Shader& shader) const
{
	for (const Chunck & chunck : m_chuncks)
		chunck.Draw(shader);
}
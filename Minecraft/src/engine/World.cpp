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
				m_chuncks[x][y][z].Setup(glm::ivec3((float)x, (float)y, (float)z));
}

 
Chunck & World::GetChunck(glm::ivec3 position) { return m_chuncks[position.x][position.y][position.z];}
Chunck & World::GetChunck(int x, int y, int z) { return m_chuncks[x][y][z]; } 

Block & World::GetBlock(glm::ivec3 position) { return GetBlock(position.x, position.y, position.z); }
Block & World::GetBlock(int x, int y, int z){ 
	return GetChunck(x / Chunck::size, y / Chunck::size, z / Chunck::size).GetBlock(x%Chunck::size, y%Chunck::size, z%Chunck::size);
}



bool World::BlockExists(glm::ivec3 position) { return BlockExists(position.x, position.y, position.z); }
bool World::BlockExists(int x, int y, int z)
{
	if (x >= 0 && y >= 0 && z >= 0 && x < size*Chunck::size && y < height * Chunck::size && z < size*Chunck::size)
		return true;
	else
		return false;
}


void World::GeneratePhysics()
{
	for (int y = 0; y < height; ++y)
		for (int z = 0; z < size; ++z)
			for (int x = 0; x < size; ++x)
				m_chuncks[x][y][z].GenerateCollider();
}  

void World::GenerateChunks()
{
	//Perlin cubes generation
	for (int y = 0; y < height * Chunck::size; ++y)
		for (int x = 0; x < size * Chunck::size; ++x)
			for (int z = 0; z < size * Chunck::size; ++z)
			{
				const int heightMap = height * Chunck::size; 

				float scale2D = 1.f / 10.f;
				int h = (int)((float)heightMap *( perlinGen.noise(scale2D*x, scale2D*z) + 1.f) / 2.f);

				if (y > h) 
				{
					GetBlock(x, y, z).solid = false;
					GetBlock(x, y, z).enabled = false;
				}
				else
				{
					GetBlock(x, y, z).solid = true;
					GetBlock(x, y, z).enabled = true;
				}
			}
	
	//Removing hidden cubes
	for (int y = 0; y < height * Chunck::size; ++y)
		for (int x = 0; x < size * Chunck::size; ++x)
			for (int z = 0; z < size * Chunck::size; ++z)
			{
				Block& block = GetBlock(x, y, z);
				if ((x + 1 >= size * Chunck::size	|| GetBlock( x+1,	y, z ).solid ) &&
					(x - 1 < 0						|| GetBlock( x-1,	y,		z	).solid )&&
					(y + 1 >= size * Chunck::size	|| GetBlock( x,		y+1,	z	).solid )&&
					(y - 1 < 0 * Chunck::size		|| GetBlock( x,		y-1,	z	).solid )&&
					(z + 1 >= size * Chunck::size	|| GetBlock( x,		y,		z+1	).solid )&&
					(z - 1 < 0 * Chunck::size		|| GetBlock( x,		y,		z-1	).solid) )
				{
					block.enabled = false;
				}

				if (block.type == Block::Type::dirt && ( y + 1 >= height * Chunck::size || ! GetBlock(x, y + 1, z).solid))
					block.type = Block::Type::grass;
			}

	//Mesh generation
	for (int x = 0; x < size; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			for (int z = 0; z < size; ++z)
			{
				Chunck & chunck = GetChunck(x, y, z);
				chunck.GenerateMesh( *this );
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
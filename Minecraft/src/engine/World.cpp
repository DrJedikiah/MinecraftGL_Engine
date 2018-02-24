#include "engine/World.h"

World World::m_instance = World();
PerlinNoise World::perlinGen;
Chunck *** World::m_chuncks;

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
				m_chuncks[x][y][z].Setup(this, glm::ivec3((float)x, (float)y, (float)z));
}

 
Chunck & World::GetChunck(glm::ivec3 position) { return m_chuncks[position.x][position.y][position.z];}

Block & World::GetBlock(glm::ivec3 position) {
	return GetChunck(position / Chunck::size).GetBlock(position % Chunck::size);
}

bool World::BlockGenerated(glm::ivec3 position)
{
	if (position.x >= 0 && position.y >= 0 && position.z >= 0 && 
		position.x < size*Chunck::size && position.y < height * Chunck::size && position.z < size*Chunck::size)
		return true;
	else
		return false;
}

glm::ivec3 World::BlockAt(btVector3 worldPos)
{
	return glm::toVec3(worldPos + 0.5f * btVector3(Block::size, Block::size, Block::size));
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
					GetBlock({ x, y, z }).ToAir();
				else
					GetBlock({ x, y, z }).ToDirt();
			}
	
	//Removing hidden cubes
	for (int y = 0; y < height * Chunck::size; ++y)
		for (int x = 0; x < size * Chunck::size; ++x)
			for (int z = 0; z < size * Chunck::size; ++z)
			{
				Block& block = GetBlock({ x, y, z });
				if ( ! IsVisible({ x, y, z }))
					block.enabled = false;
			}

	//Setting grass	
	for (int y = 0; y < height * Chunck::size; ++y)
		for (int x = 0; x < size * Chunck::size; ++x)
			for (int z = 0; z < size * Chunck::size; ++z)
			{
				Block& block = GetBlock({ x, y, z });
				if (block.type == Block::Type::dirt && (y + 1 >= height * Chunck::size || !GetBlock({ x, y + 1, z }).solid))
					block.ToGrass();
			}

	//Mesh generation
	for (int x = 0; x < size; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			for (int z = 0; z < size; ++z)
			{
				Chunck & chunck = GetChunck({ x, y, z });
				chunck.GenerateMesh();
			}
		}
	}
}

bool World::IsVisible(glm::ivec3 position)
{
	if(position.x + 1 >= size * Chunck::size ||
		position.x - 1 < 0 ||
		position.y + 1 >= size * Chunck::size ||
		position.y - 1 < 0 * Chunck::size ||
		position.z + 1 >= size * Chunck::size || 
		position.z - 1 < 0 * Chunck::size )
	return true;

	if (GetBlock({ position.x + 1, position.y, position.z }).solid &&
		GetBlock({ position.x - 1,	position.y,	position.z }).solid &&
		GetBlock({ position.x, position.y + 1, position.z }).solid &&
		GetBlock({ position.x, position.y - 1,  position.z }).solid &&
		GetBlock({ position.x, position.y, position.z + 1 }).solid &&
		GetBlock({ position.x, position.y, position.z - 1 }).solid)
		return false;

	return true;
}

void World::RemoveBlock(glm::ivec3 position)
{
		Block& block = GetBlock(position);
		if (block.solid)
		{
			block.ToAir();
			UpdateAround(position);
		}
}
 
void World::AddBlock(glm::ivec3 position)
{
	if (BlockGenerated(position))
	{
		Block& block = GetBlock(position);
		if (!block.solid)
		{
			block.ToDirt();
			UpdateAround(position);
		}
	}
}


void World::UpdateBlock(glm::ivec3 position)
{
	if (BlockGenerated(position))
	{
		GetBlock(position).enabled = IsVisible(position);
		GetChunck(position / Chunck::size).GenerateLater();
	}
}

void World::Update(float delta)
{
	for (int x = 0; x < size; ++x)
		for (int y = 0; y < height; ++y)
			for (int z = 0; z < size; ++z)
				GetChunck({ x, y, z }).Update(delta);

}
void World::UpdateAround(glm::ivec3 position)
{
	UpdateBlock(position + glm::ivec3(1, 0, 0));
	UpdateBlock(position + glm::ivec3(-1, 0, 0));
	UpdateBlock(position + glm::ivec3(0, 1, 0));
	UpdateBlock(position + glm::ivec3(0, -1, 0));
	UpdateBlock(position + glm::ivec3(0, 0, 1));
	UpdateBlock(position + glm::ivec3(0, 0, -1));
}

void World::Draw(const Shader& shader)
{
	for (int y = 0; y < height; ++y)
		for (int z = 0; z < size; ++z)
			for (int x = 0; x < size; ++x)
				m_chuncks[x][y][z].Draw(shader);
}

World::~World()
{

}
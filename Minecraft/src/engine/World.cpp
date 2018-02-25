#include "engine/World.h"

World World::m_instance = World();
PerlinNoise World::perlinGen(33);
Chunck *** World::m_chuncks;

World::World()
{

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
	const glm::vec3 sizeMap(size * Chunck::size, height * Chunck::size,  size * Chunck::size);
	const float high = 0.95f * sizeMap.y;
	const float low = 0.75f*sizeMap.y;

	for (int x = 0; x < size * Chunck::size; ++x)
		for (int y = 0; y < height * Chunck::size; ++y)
			for (int z = 0; z < size * Chunck::size; ++z)
			{
				glm::vec3 pos((float)x, (float)y, (float)z);

				float density = (float)perlinGen.CustomPerlin(pos.x, pos.y, pos.z,100., 30., 10., 1., 0.5, 0.25);

				//Plateau
				float scaleHeight;

				if (pos.y > high)
					scaleHeight = 0.f; 
				else if (pos.y > low)
					scaleHeight = (high - pos.y) / (high - low);
				else
					scaleHeight = 1.f;

				//Rondeur
				float scaleDist = 0.15f * glm::length(sizeMap) / (1.f + glm::distance(pos, glm::vec3(sizeMap.x / 2.f, low, sizeMap.z / 2.f)));
				
				//Map density
				density *= scaleHeight * scaleDist;

				//Set blocks
				Block& block = GetBlock({ x, y, z });

				if (density > 0.4 )
					block.ToStone();
				else
					block.ToAir();

			}

	//Set dirt
	const float superLow = 0.5f*sizeMap.y;
	for (int x = 0; x < size * Chunck::size; ++x)
		for (int y = 0; y < height * Chunck::size; ++y)
			for (int z = 0; z < size * Chunck::size; ++z)
			{
				glm::vec3 pos((float)x, (float)y, (float)z);
				float nbDirt;
				if (pos.y > low)
					nbDirt = 1.f;
				else if (pos.y < superLow)
					nbDirt = 0.f;
				else
					nbDirt = 1.f - (low - pos.y) / (low - superLow);

				nbDirt = nbDirt * 5.f;

				if (GetBlock({ x, y, z }).type == Block::Type::stone && ( ! BlockGenerated({ x, y+1, z }) || !GetBlock({ x, y + 1, z }).solid))
				for( int i = 0; i < (int)nbDirt; ++i )
					if(BlockGenerated({ x, y - i, z }) && GetBlock({ x, y - i, z }).type == Block::Type::stone)
						GetBlock({ x, y-i, z }).ToDirt();	
			}

	//Set caves
	for (int x = 0; x < size * Chunck::size; ++x)
		for (int y = 0; y < height * Chunck::size; ++y)
			for (int z = 0; z < size * Chunck::size; ++z)
			{
				glm::vec3 pos((float)x, (float)y, (float)z);

				//Caves
				float cavesScale;
				if (pos.y > high)
					cavesScale = 1.f;
				else if (pos.y > low)
					cavesScale = 1.f - (high - pos.y) / (high - low);
				else
					cavesScale = 0.f;

				float cavesFreqLow = 15;
				float cavesLow = 0.5f *(1.f + (float)perlinGen.noise(pos.x / cavesFreqLow, pos.y / cavesFreqLow, pos.z / cavesFreqLow));
				float cavesFreqHigh = cavesFreqLow / 3;
				float cavesHigh = 0.5f *(1.f + (float)perlinGen.noise(pos.x / cavesFreqHigh, pos.y / cavesFreqHigh, pos.z / cavesFreqHigh));

				//cavesDensity
				float cavesDensity = 0.8f*cavesLow + 0.2f*cavesHigh + 0.6f * cavesScale;

				//Set blocks
				Block& block = GetBlock({ x, y, z });
				if (cavesDensity < 0.4)
					block.ToAir();
			}

	//Set grass	
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
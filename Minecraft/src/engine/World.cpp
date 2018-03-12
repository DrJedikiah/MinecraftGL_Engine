#include "engine/World.h"

const int seed = 23;


World World::m_instance = World();
PerlinNoise World::perlinGen(seed);
Chunck *** World::m_chuncks; 

TreeGen World::m_treeGen;
Node * World::m_lastTree = nullptr;
 
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
	/*for (int x = 0; x < size * Chunck::size; ++x)
		for (int y = 0; y < height * Chunck::size; ++y)
			for (int z = 0; z < size * Chunck::size; ++z)
			{
				Block& block = GetBlock({ x, y, z });
				if (y == 0)
					block.SetType(Block::Type::dirt);
				else
					block.SetType(Block::Type::air);
				
			}*/

	const glm::vec3 sizeMap(size * Chunck::size, height * Chunck::size,  size * Chunck::size);
	const float high = 0.8f * sizeMap.y;
	const float low = 0.65f*sizeMap.y;


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
					block.SetType(Block::Type::stone);
				else
					block.SetType( Block::Type::air);

			}

	//Set dirt
	const float superLow = 0.3f*sizeMap.y;

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
						GetBlock({ x, y-i, z }).SetType(Block::Type::dirt);
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
					block.SetType(Block::Type::air);
			}


	std::default_random_engine generator(seed);
	std::uniform_real_distribution<float> distribution(0.f, 1.f);


	//Set grass	
	for (int y = 0; y < height * Chunck::size; ++y)
		for (int x = 0; x < size * Chunck::size; ++x)
			for (int z = 0; z < size * Chunck::size; ++z)
			{
				Block& block = GetBlock({ x, y, z });
				if (block.type == Block::Type::dirt && (y + 1 >= height * Chunck::size || !GetBlock({ x, y + 1, z }).solid))
				{
					block.SetType(Block::Type::grass);

					float random = distribution(generator);
					 if(random > 0.99)
						GenerateTree({ x, y + 1, z }, 5.f + (float)height * distribution(generator));
				}
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

void World::RemoveBlock(glm::ivec3 position)
{
		Block& block = GetBlock(position);
		if (block.solid)
		{
			block.SetType(Block::Type::air);
			UpdateAround(position);
		}
}
 

void World::SetBlock(glm::ivec3 position, Block::Type blockType)
{
	if (BlockGenerated(position))
		World::GetBlock(position).SetType(blockType);
}


void World::GenerateTree(glm::ivec3 position, float size)
{
	delete(m_lastTree);
	m_lastTree = m_treeGen.GenerateTree(position, size);

	std::stack<Node * > stack;
	stack.push(m_lastTree);
	while (!stack.empty())
	{
		Node * node = stack.top();
		stack.pop();

		if (BlockGenerated(node->position))
		{
			Block& block = GetBlock(node->position);
			if (node->depth <= 2 && (block.type == Block::air || block.type == Block::leaf))
			{
				SetBlock(node->position, Block::Type::wood);
				UpdateBlock(node->position);
			}
			else if (block.type == Block::air)
			{
				SetBlock(node->position, Block::Type::leaf);
				UpdateBlock(node->position);
			}
		}
		  

		for (Node * n : node->next)
			stack.push(n);
	}
}


void World::UpdateBlock(glm::ivec3 position)
{
	if (BlockGenerated(position))
		GetChunck(position / Chunck::size).GenerateLater(); 
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

void World::DrawTransparent(const Shader & shader)
{
	for (int y = 0; y < height; ++y)
		for (int z = 0; z < size; ++z)
			for (int x = 0; x < size; ++x)
				m_chuncks[x][y][z].DrawTransparent(shader);
}


void World::DrawOpaque(const Shader & shader)
{
	if (m_lastTree)
	{
		glm::vec3 red(1, 0, 0);
		glm::vec3 green(0, 1, 0);

		std::stack<Node * > stack;
		stack.push(m_lastTree);
		while (!stack.empty())
		{
			Node * node = stack.top();
			stack.pop();


			if (node->parent)
			{
				if (node->depth < 3)
				{
					Debug::DrawLine(node->position, node->parent->position, red);
					Debug::DrawCross(node->position, 0.1f, red);
				}
				else
					Debug::DrawCross(node->position, 0.1f, green);
			}

			for (Node * n : node->next)
				stack.push(n);
		}
	}
	for (int y = 0; y < height; ++y)
		for (int z = 0; z < size; ++z)
			for (int x = 0; x < size; ++x)
				m_chuncks[x][y][z].DrawOpaque(shader);
}

World::~World()
{

}
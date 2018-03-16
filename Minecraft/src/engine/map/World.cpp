#include "engine/map/World.h"

const int seed = 23;

CircularArray World::m_array(World::size, 30,30);
PerlinNoise World::perlinGen(seed);
TreeGen World::m_treeGen;
Node * World::m_lastTree = nullptr;

World World::m_instance = World();

World::World() 
{  

} 

Chunck* World::GetChunck(int x, int z)
{ 
	return m_array.Get(x, z);
}


Block* World::GetBlock(glm::ivec3 position) 
{
	if (position.y < 0 || position.y >= SubChunck::size * Chunck::height)
		return nullptr;

	glm::ivec3 pos = position / SubChunck::size;
	Chunck * chunck = GetChunck(pos.x, pos.z);
	if (chunck)
		return chunck->GetBlock( glm::vec3( position.x % SubChunck::size, position.y, position.z % SubChunck::size) );
	else
		return nullptr;
} 


glm::ivec3 World::BlockAt(glm::vec3 worldPos)
{
	return worldPos + 0.5f * glm::vec3(Block::size, Block::size, Block::size);
}

void World::GenerateChunks() 
{
	for (int z = 0; z < size; ++z)
		for (int x = 0; x < size; ++x)
		{
			Chunck * newChunck = new Chunck(m_array.OriginX() + x, m_array.OriginZ() + z);
			m_array.Set(m_array.OriginX() + x, m_array.OriginZ() + z, newChunck);
			newChunck->GenerateBlocks();
		}

	for (int x = 0; x < size; ++x)
			for (int z = 0; z < size; ++z)
				for (int i = 0; i < Chunck::height; ++i)
					GetChunck(m_array.OriginX() + x, m_array.OriginZ() + z)->GenerateMesh(i);
}

void World::RemoveBlock(glm::ivec3 position)
{
		Block* block = GetBlock(position);
		if (block && block->solid)
		{
			block->SetType(Block::Type::air);
			UpdateAround(position);
		}
}
 

void World::SetBlock(glm::ivec3 position, Block::Type blockType)
{
	Block* block = GetBlock(position);
	if (block)
		block->SetType(blockType);
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

		Block* block = GetBlock(node->position);
		if (block)
		{
			if (node->depth <= 2 && (block->type == Block::air || block->type == Block::leaf))
			{
				SetBlock(node->position, Block::Type::wood);
				UpdateBlock(node->position);
			}
			else if (block->type == Block::air)
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
	Chunck* chunck = GetChunck(position.x / SubChunck::size, position.z / SubChunck::size);
	if (chunck)
		chunck->GenerateLater(position.y / SubChunck::size);
}

void World::EnableAllChuncks()
{
	/*for (int x = 0; x < size; ++x)
			for (int z = 0; z < size; ++z)
				GetChunck({ x, 0, z }).SetEnabled(true);*/
}

void World::ClipChuncks(const Camera & camera)
{
	/*std::vector<glm::vec3> chunckPoints =
	{
		 glm::vec3(0, 0, 0),
		 glm::vec3(1, 0, 0),
		 glm::vec3(1, 0, 1),
		 glm::vec3(0, 0, 1),
		 glm::vec3(0, 1, 0),
		 glm::vec3(1, 1, 0),
		 glm::vec3(1, 1, 1),
		 glm::vec3(0, 1, 1),
	};

	std::vector<Plane> frustrumPlanes = camera.GetFrustrumPlanes();

	for (int x = 0; x < size; ++x)
		for (int y = 0; y < height; ++y)
			for (int z = 0; z < size; ++z)
			{
				bool enabled = false;
				for (glm::vec3 point : chunckPoints)
				{
					//If the chunck point is very close, set enabled true
					glm::vec3 center = (float)Chunck::size * glm::vec3(x + 0.5, y + 0.5, z + 0.5);
					float distance = glm::distance(center, camera.position());
					if (distance < Chunck::size)
					{
						enabled = true;
						break;
					}

					//If the chunck point is on the wrong side of a view frustrum plane
					glm::vec3 worldPoint = (float)Chunck::size * (glm::vec3(x, y, z) + point);
					bool outside = false;
					for (Plane plane : frustrumPlanes)
					{
						if (plane.Right(worldPoint))
						{
							outside = true;
							break;
						}
					}

					if (!outside)
					{
						enabled = true;
						break;
					}
				}
				GetChunck({ x, y, z }).SetEnabled(enabled);
			}*/
}

void World::Update(float delta)
{
	for (int x = 0; x < size; ++x)
			for (int z = 0; z < size; ++z)
				GetChunck(m_array.OriginX() + x, m_array.OriginZ() + z )->Update(delta);
}

void World::CenterChuncksAround(glm::ivec3 chunckPos)
{
	for (int x = -1; x < 2; ++x)
		for (int y = -1; y < 2; ++y)
			for (int z = -1; z < 2; ++z)
			{
				Chunck * chunck = World::GetChunck(chunckPos.x+x, chunckPos.z+z);
				if (!chunck) continue;
					chunck->GenerateCollider(chunckPos.y + y);
			}


	//Generates missing chuncks
	if (chunckPos.x < m_array.OriginX() + World::size / 2 - 1 )
	{
		//Move delete and generate
		m_array.MoveLeft();
		for (int z = 0; z < size; ++z)
		{
			delete m_array.Get(m_array.OriginX(), m_array.OriginZ() + z);
			m_array.Set(m_array.OriginX(), m_array.OriginZ() + z, new Chunck(m_array.OriginX(), m_array.OriginZ() + z));
			GetChunck(m_array.OriginX(), m_array.OriginZ() + z)->GenerateBlocks();
			
			for (int y = 0; y < Chunck::height; ++y)
			{
				GetChunck(m_array.OriginX(), m_array.OriginZ() + z)->GenerateMesh(y);
				GetChunck(m_array.OriginX() + 1, m_array.OriginZ() + z)->GenerateMesh(y);
			}
		}
	}
	else if (chunckPos.x > m_array.OriginX() + World::size / 2 + 1 )
	{
		//Move delete and generate
		m_array.MoveRight();
		for (int z = 0; z < size; ++z)
		{
			delete m_array.Get(m_array.OriginX() + size - 1, m_array.OriginZ() + z);
			m_array.Set(m_array.OriginX() + size - 1, m_array.OriginZ() + z, new Chunck(m_array.OriginX() + size - 1, m_array.OriginZ() + z));
			GetChunck(m_array.OriginX() + size - 1, m_array.OriginZ() + z)->GenerateBlocks();
			
			for (int y = 0; y < Chunck::height; ++y)
			{
				GetChunck(m_array.OriginX() + size - 1, m_array.OriginZ() + z)->GenerateMesh(y);
				GetChunck(m_array.OriginX() + size - 2, m_array.OriginZ() + z)->GenerateMesh(y);
			}
		}
	}

	else if (chunckPos.z > m_array.OriginZ() + World::size / 2 + 1)
	{
		m_array.MoveFront();
		for (int x = 0; x < size; ++x)
		{
			delete m_array.Get(m_array.OriginX() + x, m_array.OriginZ() + size - 1);
			m_array.Set(m_array.OriginX() + x, m_array.OriginZ() + size - 1, new Chunck(m_array.OriginX() + x, m_array.OriginZ() + size - 1));
			GetChunck(m_array.OriginX() + x, m_array.OriginZ() + size - 1)->GenerateBlocks();
			
			for (int y = 0; y < Chunck::height; ++y)
			{
				GetChunck(m_array.OriginX() + x, m_array.OriginZ() + size - 1)->GenerateMesh(y);
				GetChunck(m_array.OriginX() + x, m_array.OriginZ() + size - 2)->GenerateMesh(y);
			}
		}
	}
	else if (chunckPos.z < m_array.OriginZ() + World::size / 2 - 1)
	{
		m_array.MoveBack();
		for (int x = 0; x < size; ++x)
		{
			delete m_array.Get(m_array.OriginX() + x, m_array.OriginZ());
			m_array.Set(m_array.OriginX() + x, m_array.OriginZ(), new Chunck(m_array.OriginX() + x, m_array.OriginZ()));
			GetChunck(m_array.OriginX() + x, m_array.OriginZ())->GenerateBlocks();
			for (int y = 0; y < Chunck::height; ++y)
			{
				GetChunck(m_array.OriginX() + x, m_array.OriginZ())->GenerateMesh(y);
				GetChunck(m_array.OriginX() + x, m_array.OriginZ() + 1)->GenerateMesh(y);
			}

		}
	}
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
	for (int z = 0; z < size; ++z)
		for (int x = 0; x < size; ++x)
		{
			Chunck * chunck = GetChunck(m_array.OriginX() + x, m_array.OriginZ() + z);
			if(chunck)
				chunck->DrawTransparent(shader);
		}
			
}

void World::DrawOpaque(const Shader & shader)
{
	for (int z = 0; z < size; ++z)
		for (int x = 0; x < size; ++x)
		{
			Chunck * chunck = GetChunck(m_array.OriginX() + x, m_array.OriginZ() + z);
			if (chunck)
				chunck->DrawOpaque(shader);
		}
			
}

void World::OnDrawDebug() const
{
	if (m_lastTree)
	{
		glm::vec4 red(1, 0, 0, 1.f);
		glm::vec4 green(0, 1, 0, 1.f);

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
}

glm::ivec3 World::GetOrigin() { return { m_array.OriginX(), 0, m_array.OriginZ() }; }

World::~World()
{

}
#include "engine/map/World.h"

CircularArray World::m_array(World::size, 100,100);

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
	{
		SubChunck * subChunck = chunck->GetSubChunck(position.y / SubChunck::size);
		m_array.UpdateSubChunckMesh(subChunck);
		subChunck->GenerateCollider();
	}
}

void World::EnableAllChuncks()
{
	for (int x = 0; x < size; ++x)
		for (int z = 0; z < size; ++z)
			{
				Chunck * chunck = World::GetChunck(m_array.OriginX() + x, m_array.OriginZ() + z);
				if (chunck)
					chunck->SetEnabled(true);
			}
}

void World::ClipChuncks(const Camera & camera)
{
	std::vector<glm::vec3> chunckPoints =
	{
		glm::vec3(0, 0, 0),
		glm::vec3(1, 0, 0),
		glm::vec3(1, 0, 1),
		glm::vec3(0, 0, 1),
		glm::vec3(0, Chunck::height, 0),
		glm::vec3(1, Chunck::height, 0),
		glm::vec3(1, Chunck::height, 1),
		glm::vec3(0, Chunck::height, 1),
	};

	std::vector<Plane> planes = camera.GetFrustrumPlanes();
	std::vector<Plane> leftRightPlanes = { planes[0], planes[1] };

	std::vector<Chunck*> enabledChuncks;
	//First pass to eliminate full chuncks
	for (int z = 0; z < size; ++z)
		for (int x = 0; x < size; ++x)
		{
			glm::ivec3 chunckPos = glm::ivec3(m_array.OriginX() + x, 0, m_array.OriginZ() + z);
			Chunck * chunck = GetChunck(chunckPos.x, chunckPos.z);
			if (chunck)
			{
				bool enabled = false;
				for (glm::vec3 point : chunckPoints)
				{
					//If the chunck point is on the wrong side of a view frustrum plane
					glm::vec3 worldPoint = (float)SubChunck::size * ( glm::vec3(chunckPos) + point);
					bool outside = false;
					for (Plane plane : leftRightPlanes)
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
						enabledChuncks.push_back(chunck);
						break;
					}
				}
				chunck->SetEnabled(enabled);
			}
		}

	std::vector<glm::vec3> subChunckPoints =
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

	std::vector<Plane> topBotPlanes = { planes[2], planes[3] };

	for (Chunck * chunck : enabledChuncks)
	{
		for (int y = 0; y < Chunck::height; ++y)
		{
			bool enabled = false;
			for (glm::vec3 point : subChunckPoints)
			{
				glm::ivec3 subChunckPos = glm::ivec3(chunck->Position().x, y, chunck->Position().z);

				//If the chunck point is very close, set enabled true
				glm::vec3 center = (float)SubChunck::size * (glm::vec3(subChunckPos) + glm::vec3(0.5, 0.5, 0.5));
				float distance = glm::distance(center, camera.position());
				if (distance < SubChunck::size)
				{
					enabled = true;
					break;
				}

				//If the chunck point is on the wrong side of a view frustrum plane
				glm::vec3 worldPoint = (float)SubChunck::size * (glm::vec3(subChunckPos) + point);
				bool outside = false;
				for (Plane plane : topBotPlanes)
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
			chunck->SetSubChunckEnabled(y, enabled);
		}
	}
}

void World::Update(float delta)
{
	m_array.Update(delta);

	//Update chuncks
	for (int x = 0; x < size; ++x)
		for (int z = 0; z < size; ++z)
		{
			Chunck * chunck = GetChunck(m_array.OriginX() + x, m_array.OriginZ() + z);
			if( chunck )
				chunck->Update(delta);
		}
				
}

void World::CenterChuncksAround(glm::ivec3 chunckPos)
{
	//Generates colliders around the center
	for (int x = -1; x < 2; ++x)
		for (int y = -1; y < 2; ++y)
			for (int z = -1; z < 2; ++z)
			{
				Chunck * chunck = World::GetChunck(chunckPos.x+x, chunckPos.z+z);
				if (chunck )
					chunck->GenerateCollider(chunckPos.y + y);
			}


	//Generates missing chuncks
	if (chunckPos.x < m_array.OriginX() + World::size / 2 - 1 )
		m_array.MoveLeft();
	else if (chunckPos.x > m_array.OriginX() + World::size / 2 + 1 )
		m_array.MoveRight();
	else if (chunckPos.z > m_array.OriginZ() + World::size / 2 + 1)
		m_array.MoveFront();
	else if (chunckPos.z < m_array.OriginZ() + World::size / 2 - 1)
		m_array.MoveBack();
	
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
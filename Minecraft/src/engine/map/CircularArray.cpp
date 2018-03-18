#include <engine/map/CircularArray.h>



CircularArray::CircularArray(int size, int originX, int originZ) :
	m_size(size),
	m_xOrigin(originX),
	m_zOrigin(originZ),
	m_xOffset(0),
	m_zOffset(0),
	m_chunckGenerator( new ChunckGenerator())
{
	m_array.resize(size);
	for (int i = 0; i < size; ++i)
		m_array[i].resize(size, nullptr);

	for (int z = 0; z < m_size; ++z)
		for (int x = 0; x < m_size; ++x)
		{
			glm::vec2 pos = glm::vec2(x, z);
			glm::vec2 center = glm::vec2((float)m_size/2);
			float dist = glm::distance(center, pos);
			m_chunckGenerator->GenerateBlocks(OriginX() + x, OriginZ() + z, dist);
		}	
}

bool  CircularArray::InsideArray(int x, int z) const
{
	if (x < m_xOrigin || x >= m_xOrigin + m_size || z < m_zOrigin || z >= m_zOrigin + m_size)
		return false;
	return true;
}

void CircularArray::UpdateSubChunckMesh(SubChunck* subChunck)
{
	if (subChunck)
		m_genMeshLater.emplace(subChunck);
	else
		std::cout << "zob6" << std::endl;
}

void CircularArray::DeleteChunck(Chunck * chunck)
{
	if (chunck)
		m_toDelete.push_back(chunck);
}

void CircularArray::Update(float delta)
{
	//Delete chuncks
	for (int i = 0; i < (int)m_toDelete.size(); ++i)
	{
		Chunck * chunck = m_toDelete[i];
		if (!chunck)
		{
			m_toDelete[i] = m_toDelete[m_toDelete.size() - 1];
			m_toDelete.pop_back();
			--i;
		}
		else
		{
			bool generating = false;
			for( int y = 0; y < Chunck::height; ++y)
				if (chunck->GetSubChunck(y)->generating)
				{
					generating = true;
					break;
				}
			if (!generating)
			{
				delete chunck;
				m_toDelete[i] = m_toDelete[m_toDelete.size() - 1];
				m_toDelete.pop_back();
				--i;
			}
		}
	}
	
	//Add newly allocated chuncks to world
	std::vector<Chunck*> chuncks = m_chunckGenerator->PopChuncksGenerateds();
	for (Chunck * chunck : chuncks)
		if (InsideArray(chunck->Position().x, chunck->Position().z))
		{
			m_waitingFirstGen.push_back(chunck);
			Set(chunck->Position().x, chunck->Position().z, chunck);
		}
		else
			DeleteChunck(chunck);
	
	//Generates mesh of newly allocateds chuncks
	for (int i = 0; i < (int)m_waitingFirstGen.size(); ++i)
	{
		Chunck * chunck = m_waitingFirstGen[i];

		glm::ivec2 pos = glm::ivec2(chunck->Position().x, chunck->Position().z);

		if (Get(pos.x + 1, pos.y) && Get(pos.x + 1, pos.y)->BlocksGenerated() &&
			Get(pos.x - 1, pos.y) && Get(pos.x - 1, pos.y)->BlocksGenerated() &&
			Get(pos.x, pos.y + 1) && Get(pos.x, pos.y + 1)->BlocksGenerated() &&
			Get(pos.x, pos.y - 1) && Get(pos.x, pos.y - 1)->BlocksGenerated()
			)
		{
			//Generates additionnal content (trees)
			chunck->LateGenerateBlocks();

			//Send subChunck to generator for mesh creation
			glm::vec2 center = glm::vec2(m_xOrigin + m_size / 2, m_zOrigin + m_size / 2);
			float dist = glm::distance(center, glm::vec2(pos.x, pos.y));
			for (int i = 0; i < Chunck::height; ++i)
				m_chunckGenerator->GenerateMesh(chunck->GetSubChunck(i), dist);

			m_waitingFirstGen[i] = m_waitingFirstGen[m_waitingFirstGen.size() - 1];
			m_waitingFirstGen.pop_back();
			--i;
		}
	}

	//Send subChuncks to generator for mesh creation
	for (SubChunck * subChunck : m_genMeshLater)
	{
		if (!subChunck->generating)
		{
			glm::vec2 pos = glm::vec2(subChunck->Position().x, subChunck->Position().z);
			glm::vec2 center = glm::vec2(m_xOrigin + m_size / 2, m_zOrigin + m_size / 2);
			float dist = glm::distance(center, glm::vec2(pos.x, pos.y));
			m_chunckGenerator->GenerateMesh(subChunck, dist);
		}
	}
	m_genMeshLater.clear();



	//Generates models
	for (SubChunck * chunck : m_chunckGenerator->PopMeshGenerateds())
		chunck->GenerateModels();
}

void CircularArray::MoveRight()
{
	m_xOffset = (m_xOffset + 1) % m_size;
	++m_xOrigin;

	for (int z = 0; z < m_size; ++z)
	{
		DeleteChunck(Get(OriginX() + m_size - 1, OriginZ() + z));
		m_chunckGenerator->GenerateBlocks(OriginX() + m_size - 1, OriginZ() + z);
		Set(OriginX() + m_size - 1, OriginZ() + z, nullptr);

		Chunck * chunck = Get(OriginX() + m_size - 2, OriginZ() + z);
		if (chunck)
			for (int y = 0; y < Chunck::height; ++y)
				UpdateSubChunckMesh(chunck->GetSubChunck(y));
	}
}

void CircularArray::MoveLeft()
{
	m_xOffset = (m_xOffset + m_size - 1) % m_size;
	--m_xOrigin;

	for (int z = 0; z < m_size; ++z)
	{
		DeleteChunck(Get(OriginX(), OriginZ() + z));
		Set(OriginX(), OriginZ() + z, nullptr);
		m_chunckGenerator->GenerateBlocks(OriginX(), OriginZ() + z);

		Chunck * chunck = Get(OriginX() + 1, OriginZ() + z);
		if (chunck)
			for (int y = 0; y < Chunck::height; ++y)
				UpdateSubChunckMesh(chunck->GetSubChunck(y));
	}
}

void CircularArray::MoveBack()
{
	m_zOffset = (m_zOffset + m_size - 1) % m_size;
	--m_zOrigin;

	for (int x = 0; x < m_size; ++x)
	{
		DeleteChunck(Get(OriginX() + x, OriginZ()));

		Set(OriginX() + x, OriginZ(), nullptr);
		m_chunckGenerator->GenerateBlocks(OriginX() + x, OriginZ());

		Chunck * chunck = Get(OriginX() + x, OriginZ() + 1);
		if (chunck)
			for (int y = 0; y < Chunck::height; ++y)
				UpdateSubChunckMesh(chunck->GetSubChunck(y));
	}
}

void CircularArray::MoveFront()
{
	m_zOffset = (m_zOffset + 1) % m_size;
	++m_zOrigin;

	for (int x = 0; x < m_size; ++x)
	{
		DeleteChunck(Get(OriginX() + x, OriginZ() + m_size - 1));

		Set(OriginX() + x, OriginZ() + m_size - 1, nullptr);
		m_chunckGenerator->GenerateBlocks(OriginX() + x, OriginZ() + m_size - 1);

		Chunck * chunck = Get(OriginX() + x, OriginZ() + m_size - 2);
		if (chunck)
			for (int y = 0; y < Chunck::height; ++y)
				UpdateSubChunckMesh(chunck->GetSubChunck(y));
	}
}

void CircularArray::Set(int x, int z, Chunck* chunck)
{
	m_array[(x - m_xOrigin + m_xOffset) % m_size][(z - m_zOrigin + m_zOffset) % m_size] = chunck;
}

Chunck * CircularArray::Get(int x, int z)
{
	if (x < m_xOrigin || x >= m_xOrigin + m_size || z < m_zOrigin || z >= m_zOrigin + m_size)
		return nullptr;
	else
		return m_array[(x - m_xOrigin + m_xOffset) % m_size][(z - m_zOrigin + m_zOffset) % m_size];
}

int CircularArray::Size() const { return m_size; }
int CircularArray::OriginX() const { return m_xOrigin; }
int CircularArray::OriginZ() const { return m_zOrigin; }

CircularArray::~CircularArray()
{
	delete m_chunckGenerator;
}

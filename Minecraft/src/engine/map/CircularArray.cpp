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
			m_chunckGenerator->GenerateBlocks(OriginX() + x, OriginZ() + z);
}

bool  CircularArray::InsideArray(int x, int z) const
{
	if (x < m_xOrigin || x >= m_xOrigin + m_size || z < m_zOrigin || z >= m_zOrigin + m_size)
		return false;
	return true;
}

void CircularArray::Update(float delta)
{
	if (m_toDelete.size() > 0)
	{
		Chunck * chunck = m_toDelete[m_toDelete.size()-1];
		if ( ! chunck)
			m_toDelete.pop_back();
		else if(!chunck->generating)
		{
			delete chunck;
			m_toDelete.pop_back();
		}

	}

	//Get the chuncks generated
	for( Chunck * chunck : m_chunckGenerator->PopChuncksGenerateds())
	{
		if (InsideArray(chunck->Position().x, chunck->Position().z))
		{
			//Send to generator for mesh creation
			Set(chunck->Position().x, chunck->Position().z, chunck);
			m_chunckGenerator->GenerateMesh(chunck);
		}
		else
			m_toDelete.push_back(chunck);
	}

	//Generates models
	for (Chunck * chunck : m_chunckGenerator->PopMeshGenerateds())
	{
		for (int i = 0; i < Chunck::height; ++i)
			chunck->GenerateModels(i);
	}
}

void CircularArray::MoveRight()
{
	m_xOffset = (m_xOffset + 1) % m_size;
	++m_xOrigin;

	for (int z = 0; z < m_size; ++z)
	{
		m_toDelete.push_back(Get(OriginX() + m_size - 1, OriginZ() + z));
		m_chunckGenerator->GenerateBlocks(OriginX() + m_size - 1, OriginZ() + z);
		Set(OriginX() + m_size - 1, OriginZ() + z, nullptr);
		/*Chunck * chunck = World::GetChunck(OriginX() + m_size - 2, OriginZ() + z);
		if(chunck)
			m_chunckGenerator->GenerateMesh(chunck);*/
	}
}

void CircularArray::MoveLeft()
{
	m_xOffset = (m_xOffset + m_size - 1) % m_size;
	--m_xOrigin;

	for (int z = 0; z < m_size; ++z)
	{
		m_toDelete.push_back(Get(OriginX(), OriginZ() + z)); 
		Set(OriginX(), OriginZ() + z, nullptr);
		m_chunckGenerator->GenerateBlocks(OriginX(), OriginZ() + z);
		/*Chunck * chunck = World::GetChunck(OriginX() + 1, OriginZ() + z);
		if (chunck)
			m_chunckGenerator->GenerateMesh(chunck);*/
	}
}

void CircularArray::MoveBack()
{
	m_zOffset = (m_zOffset + m_size - 1) % m_size;
	--m_zOrigin;

	for (int x = 0; x < m_size; ++x)
	{
		m_toDelete.push_back(Get(OriginX() + x, OriginZ()));

		Set(OriginX() + x, OriginZ(), nullptr);
		m_chunckGenerator->GenerateBlocks(OriginX() + x, OriginZ());
		/*Chunck * chunck = World::GetChunck(OriginX() + x, OriginZ() + 1);
		if (chunck)
			m_chunckGenerator->GenerateMesh(chunck);*/
	}
}

void CircularArray::MoveFront()
{
	m_zOffset = (m_zOffset + 1) % m_size;
	++m_zOrigin;

	for (int x = 0; x < m_size; ++x)
	{
		m_toDelete.push_back(Get(OriginX() + x, OriginZ() + m_size - 1));

		Set(OriginX() + x, OriginZ() + m_size - 1, nullptr);
		m_chunckGenerator->GenerateBlocks(OriginX() + x, OriginZ() + m_size - 1);
		/*Chunck * chunck = World::GetChunck(OriginX() + x, OriginZ() + m_size - 2);
		if (chunck)
			m_chunckGenerator->GenerateMesh(chunck);*/
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

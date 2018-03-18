#include  "engine/generators/ChunckGenerator.h"

ChunckGenerator::ChunckGenerator() : 
	m_chuncksGenBlocks(cmpChuncksGen),
	m_chuncksGenMesh(cmpMeshGen)
{
	m_updateBlocksThread = new std::thread(&ChunckGenerator::UpdateBlocks, this);
	m_updateMeshThread = new std::thread(&ChunckGenerator::UpdateMesh, this);
}


std::vector<Chunck *> ChunckGenerator::PopChuncksGenerateds()
{
	std::vector <Chunck *> chuncks;
	m_chuncksBlocksGeneratedsMtx.lock();
	for (Chunck * chunck : m_chuncksBlocksGenerateds)
		chuncks.push_back(chunck);
	m_chuncksBlocksGenerateds.clear();
	m_chuncksBlocksGeneratedsMtx.unlock();
	return chuncks;
}

std::vector<SubChunck *>  ChunckGenerator::PopMeshGenerateds()
{
	std::vector <SubChunck *> chuncks;
	m_chuncksMeshGeneratedsMtx.lock();
	for (SubChunck * chunck : m_chuncksMeshGenerateds)
	{
		chuncks.push_back(chunck);
		chunck->generating = false;
	}
	m_chuncksMeshGenerateds.clear();
	m_chuncksMeshGeneratedsMtx.unlock();
	return chuncks;
}

void ChunckGenerator::UpdateBlocks()
{
	while ( !m_quitting )
	{
		//Get the Blocks positions
		std::vector <glm::ivec2> positions;

		m_chuncksGenBlocksMtx.lock();
		if (!m_chuncksGenBlocks.empty())
		{
			//m_meshGenerationPaused = true;
			int count = 0;
			while (!m_chuncksGenBlocks.empty() && count++ < 16)
			{
				positions.push_back(m_chuncksGenBlocks.top().first);
				m_chuncksGenBlocks.pop();
			}
			m_chuncksGenBlocksMtx.unlock();
		}
		else
		{
			//m_meshGenerationPaused = false;
			m_chuncksGenBlocksMtx.unlock();
			std::this_thread::sleep_for(std::chrono::duration<float>(0.1));
		}
		//Generates the blocks
		std::vector <Chunck *> chuncks;
		for (glm::ivec2 vec2 : positions)
		{
			Chunck * newChunck = new Chunck(vec2.x, vec2.y);
			newChunck->GenerateBlocks();
			chuncks.push_back(newChunck);
		}
		//Returns the chuncks
		m_chuncksBlocksGeneratedsMtx.lock();
		for (Chunck * newChunck : chuncks)
			m_chuncksBlocksGenerateds.push_back(newChunck);
		m_chuncksBlocksGeneratedsMtx.unlock();
	}
}
void ChunckGenerator::UpdateMesh()
{
	while (!m_quitting)
	{
		//Get the chuncks
		std::vector <SubChunck * > chuncks;
		
		m_chuncksGenMeshMtx.lock();
		//Sleeps when no mesh to generate
		if (m_chuncksGenMesh.empty())
		{
			m_chuncksGenMeshMtx.unlock();
			std::this_thread::sleep_for(std::chrono::duration<float>(0.1));
		}
		else
		{
			int count = 0;
			while (!m_chuncksGenMesh.empty() && count++ < 16)
			{
				chuncks.push_back(m_chuncksGenMesh.top().first);
				m_chuncksGenMesh.pop();
			}
			m_chuncksGenMeshMtx.unlock();
		}
		//Generates the meshs
		for (SubChunck * chunck : chuncks)
			chunck->GenerateMesh();

		//Returns the chuncks
		m_chuncksMeshGeneratedsMtx.lock();
		for (SubChunck * chunck : chuncks)
			m_chuncksMeshGenerateds.push_back(chunck);
		m_chuncksMeshGeneratedsMtx.unlock();
	}
}

void ChunckGenerator::GenerateBlocks(int  x, int z, float priority )
{
	m_chuncksGenBlocksMtx.lock();
	m_chuncksGenBlocks.push( std::make_pair(glm::ivec2(x,z), priority));
	m_chuncksGenBlocksMtx.unlock();
}


void ChunckGenerator::GenerateMesh( SubChunck * chunck, float priority)
{
	if ( ! chunck->generating)
	{
		chunck->generating = true;
		m_chuncksGenMeshMtx.lock();
		m_chuncksGenMesh.push(std::make_pair(chunck, priority));
		m_chuncksGenMeshMtx.unlock();
	}
}

ChunckGenerator::~ChunckGenerator()
{
	m_quitting = true;
	m_updateBlocksThread->join();
	m_updateMeshThread->join();

	delete m_updateBlocksThread;
	delete m_updateMeshThread;
}
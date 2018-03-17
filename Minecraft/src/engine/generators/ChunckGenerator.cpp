#include  "engine/generators/ChunckGenerator.h"

ChunckGenerator::ChunckGenerator()
{
	new std::thread(&ChunckGenerator::UpdateBlocks, this);
	new std::thread(&ChunckGenerator::UpdateMesh, this);
	new std::thread(&ChunckGenerator::UpdateBlocks, this);
	new std::thread(&ChunckGenerator::UpdateMesh, this);

}

void ChunckGenerator::UpdateBlocks( )
{
	while (true)
	{
		//Get the Blocks positions
		std::vector <glm::ivec2> positions;
		
		m_chuncksGenBlocksMtx.lock();
		if (!m_chuncksGenBlocks.empty())
		{
			positions.push_back(m_chuncksGenBlocks.top());
			m_chuncksGenBlocks.pop();
			m_chuncksGenBlocksMtx.unlock();
		}
		else
		{
			m_chuncksGenBlocksMtx.unlock();
			std::this_thread::sleep_for(std::chrono::duration<float>(1));
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

std::vector<Chunck *>  ChunckGenerator::PopMeshGenerateds()
{
	std::vector <Chunck *> chuncks;
	m_chuncksMeshGeneratedsMtx.lock();
	for (Chunck * chunck : m_chuncksMeshGenerateds)
	{
		chuncks.push_back(chunck);
		chunck->generating = false;
	}
		
	m_chuncksMeshGenerateds.clear();
	m_chuncksMeshGeneratedsMtx.unlock();
	return chuncks;
}

void ChunckGenerator::UpdateMesh()
{
	while (true)
	{
		//Get the chuncks
		std::vector <Chunck * > chuncks;
		
		m_chuncksGenMeshMtx.lock();
		if (m_chuncksGenMesh.empty())
		{
			m_chuncksGenMeshMtx.unlock();
			std::this_thread::sleep_for(std::chrono::duration<float>(0.1));
		}
		else
		{
			chuncks.push_back(m_chuncksGenMesh.top());
			m_chuncksGenMesh.pop();
			m_chuncksGenMeshMtx.unlock();
		}
			
		

		//Generates the meshs
		for (Chunck * chunck : chuncks)
			for (int i = 0; i < Chunck::height; ++i)
				chunck->GenerateMesh(i);


		//Returns the chuncks
		m_chuncksMeshGeneratedsMtx.lock();
		for (Chunck * chunck : chuncks)
			m_chuncksMeshGenerateds.push_back(chunck);
		m_chuncksMeshGeneratedsMtx.unlock();
	}
}



void ChunckGenerator::GenerateBlocks(int  x, int z )
{
	m_chuncksGenBlocksMtx.lock();
	m_chuncksGenBlocks.push(glm::ivec2(x,z));
	m_chuncksGenBlocksMtx.unlock();
}

void ChunckGenerator::GenerateMesh( Chunck * chunck )
{
	if (chunck->generating)
		return;
	chunck->generating = true;
	m_chuncksGenMeshMtx.lock();
	m_chuncksGenMesh.push(chunck);
	m_chuncksGenMeshMtx.unlock();
}


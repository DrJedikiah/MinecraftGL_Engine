#pragma once

#include <thread>
#include <stack>
#include <list>
#include <mutex>
#include <chrono>

#include <glm/glm.hpp>

#include "engine/map/Chunck.h"

class Chunck;

class ChunckGenerator
{
public:
	ChunckGenerator();

	
	void UpdateMesh();

	void GenerateBlocks( int x, int z );
	void GenerateMesh(Chunck * chunck);

	std::vector<Chunck *> PopChuncksGenerateds();
	std::vector<Chunck *> PopMeshGenerateds();
	
private:
	void UpdateBlocks();

	std::mutex m_chuncksGenBlocksMtx;
	std::stack<glm::ivec2> m_chuncksGenBlocks;
	std::mutex m_chuncksBlocksGeneratedsMtx;
	std::vector<Chunck * > m_chuncksBlocksGenerateds;


	std::mutex m_chuncksGenMeshMtx;
	std::stack< Chunck *> m_chuncksGenMesh;
	std::mutex m_chuncksMeshGeneratedsMtx;
	std::vector<Chunck * > m_chuncksMeshGenerateds;


	std::thread *  m_updateBlocksThread;
	std::thread *  m_updateMeshThread;
};
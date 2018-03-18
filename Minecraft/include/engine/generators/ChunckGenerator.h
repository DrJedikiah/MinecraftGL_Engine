#pragma once

#include <thread>
#include <stack>
#include <list>
#include <mutex>
#include <chrono>

#include <glm/glm.hpp>

#include "engine/map/Chunck.h"

class Chunck;
class SubChunck;

class ChunckGenerator
{
public:
	ChunckGenerator();

	
	void UpdateMesh();

	void GenerateBlocks( int x, int z );
	void GenerateMesh(SubChunck * chunck);

	std::vector<Chunck *> PopChuncksGenerateds();
	std::vector<SubChunck *> PopMeshGenerateds();
	
private:
	void UpdateBlocks();

	std::mutex m_chuncksGenBlocksMtx;
	std::stack<glm::ivec2> m_chuncksGenBlocks;
	std::mutex m_chuncksBlocksGeneratedsMtx;
	std::vector<Chunck * > m_chuncksBlocksGenerateds;


	std::mutex m_chuncksGenMeshMtx;
	std::stack< SubChunck *> m_chuncksGenMesh;
	std::mutex m_chuncksMeshGeneratedsMtx;
	std::vector<SubChunck * > m_chuncksMeshGenerateds;


	bool m_meshGenerationPaused = true;

	std::thread *  m_updateBlocksThread;
	std::thread *  m_updateMeshThread;
};
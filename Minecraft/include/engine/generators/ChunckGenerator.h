#pragma once

#include <thread>
#include <stack>
#include <list>
#include <mutex>
#include <chrono>
#include <queue>

#include <glm/glm.hpp>

#include "engine/map/Chunck.h"

class Chunck;
class SubChunck;

class ChunckGenerator
{
public:
	ChunckGenerator();
	~ChunckGenerator();
	
	void UpdateMesh();

	void GenerateBlocks( int x, int z, float priority = 0);
	void GenerateMesh(SubChunck * chunck, float priority = 0);

	std::vector<Chunck *> PopChuncksGenerateds();
	std::vector<SubChunck *> PopMeshGenerateds();
	
private:
	bool m_quitting = false;

	void UpdateBlocks();

	std::mutex m_chuncksGenBlocksMtx;
	std::function< bool(std::pair<glm::ivec2, float>, std::pair<glm::ivec2, float>)> cmpChuncksGen = [](std::pair<glm::ivec2, float> left, std::pair<glm::ivec2, float> right) { return left.second > right.second; };
	std::priority_queue< 
		std::pair<glm::ivec2, float>,
		std::vector<std::pair<glm::ivec2, float>>,
		std::function< bool(std::pair<glm::ivec2, float>, std::pair<glm::ivec2, float>)>
	> m_chuncksGenBlocks;//position + priority
	std::mutex m_chuncksBlocksGeneratedsMtx;
	std::vector<Chunck * > m_chuncksBlocksGenerateds;


	std::mutex m_chuncksGenMeshMtx;
	std::function< bool(std::pair<SubChunck *, float>, std::pair<SubChunck *, float>)> cmpMeshGen = [](std::pair<SubChunck *, float> left, std::pair<SubChunck *, float> right) { return left.second > right.second; };
	std::priority_queue<
		std::pair<SubChunck *, float>,
		std::vector<std::pair<SubChunck *, float>>,
		std::function< bool(std::pair<SubChunck *, float>, std::pair<SubChunck *, float>)>
	>m_chuncksGenMesh;//SubChunck + priority
	std::mutex m_chuncksMeshGeneratedsMtx;
	std::vector<SubChunck * > m_chuncksMeshGenerateds;

	std::thread *  m_updateBlocksThread;
	std::thread *  m_updateMeshThread;
};
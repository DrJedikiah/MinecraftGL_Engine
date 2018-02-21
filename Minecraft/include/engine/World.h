#pragma once


#include "util/Perlin.h"
#include "engine/Physics.h"
#include "engine/Chunck.h"

class Chunck;

class World
{
public:
	World();

	const static int size = 4;
	const static int height = 8;

	void Draw(const Shader& shader) const;
	void GenerateChunks();
	void GeneratePhysics(PhysicsEngine & physicsEngine);

	Chunck & GetChunck(glm::ivec3 position);
	bool BlockExists(glm::ivec3 position);
	Block & GetBlock(glm::ivec3 position);
	bool BlockExists( int x, int y, int z);
	Block & GetBlock(int x, int y, int z);
	Chunck & GetChunck(int x, int y, int z);
private:
	PerlinNoise perlinGen;

	Chunck ***m_chuncks;

	

};




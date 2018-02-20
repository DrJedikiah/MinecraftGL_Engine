#pragma once

#include "Chunck.h"
#include "Perlin.h"
#include "Physics.h"

class World
{
public:
	World();

	const static int size = 4;
	const static int height = 2;

	void Draw(const Shader& shader) const;
	void GeneratePhysics(PhysicsEngine & physicsEngine);

private:
	PerlinNoise perlinGen;

	Chunck ***m_chuncks;

	void generate();

	Block & GetBlock(int x, int y, int z);
	Chunck & GetChunck(int x, int y, int z);

};




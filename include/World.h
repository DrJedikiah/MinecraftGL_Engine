#pragma once

#include "Chunck.h"
#include "Perlin.h"
#include "Physics.h"


class World
{
public:
	World();

	static const int size;
	static const int height;

	void Draw(const Shader& shader) const;
	void GeneratePhysics(Physics & physicsEngine);

private:
	PerlinNoise perlinGen;
	std::vector<Chunck> m_chuncks;

	void generate();

	Cube& getCube(int x, int y, int z);
	Chunck & getChunck(int x, int y, int z);

};


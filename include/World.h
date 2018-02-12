#pragma once

#include "Chunck.h"
#include "Perlin.h"


class World
{
public:
	World();

	const int size = 2;
	const int height = 2;

	void Draw(const Shader& shader) const;

private:
	PerlinNoise perlinGen;
	std::vector<Chunck> m_chuncks;

	void generate();

	Cube& getCube(int x, int y, int z);
	Chunck & getChunck(int x, int y, int z);

};


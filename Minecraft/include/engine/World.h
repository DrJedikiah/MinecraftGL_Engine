#pragma once


#include "util/Perlin.h"
#include "engine/Physics.h"
#include "engine/Chunck.h"
#include "graphics/Drawable.h"

class Chunck;

class World : public Drawable
{
public:
	World();

	const static int size = 8;
	const static int height = 1;

	void Draw( const Shader& shader) const override;
	void GenerateChunks();
	void GeneratePhysics();

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




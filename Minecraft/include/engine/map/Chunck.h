#pragma once

#include "engine/map/SubChunck.h"
#include "util/Perlin.h"

class SubChunck;

class Chunck
{
public:

	Chunck(  int x, int z );
	~Chunck();

	static const int height = 12;

	void Update(float delta);

	void DrawTransparent(const Shader & shader) const;
	void DrawOpaque(const Shader & shader) const;

	SubChunck* GetSubChunck( int  height);
	Block* GetBlock(glm::ivec3 position);

	void GenerateLater( int subChunck );
	void GenerateBlocks();

	void GenerateMesh(int subChunck);
	void GenerateModels(int subChunck);
	void GenerateCollider(int subChunck, bool regenerate = false );

	void SetEnabled(bool state);
	void SetSubChunckEnabled(int subChunck, bool state);

	bool Enabled() const;
	bool BlocksGenerated() const;

	glm::ivec3 Position() const;
private:
	
	bool m_enabled;
	bool m_generateLater = false;
	bool m_blocksGenerated = false;

	int m_positionX;
	int m_positionZ;

	SubChunck * m_subChuncks[Chunck::height];
	static PerlinNoise perlinGen;
};
#pragma once

#include "engine/map/SubChunck.h"

class SubChunck;

class Chunck
{
public:

	Chunck(  int x, int z );
	~Chunck();

	static const int height = 16;

	void Update(float delta);

	void DrawTransparent(const Shader & shader) const;
	void DrawOpaque(const Shader & shader) const;

	Block* GetBlock(glm::ivec3 position);

	void GenerateLater( int subChunck );
	void GenerateBlocks();
	void GenerateMesh(int subChunck);
	void GenerateCollider(int subChunck, bool regenerate = false );

	void SetEnabled(bool state);
	void SetSubChunckEnabled(int subChunck, bool state);

	bool Enabled() const;

	glm::ivec3 Position() const;

private:
	bool m_enabled;
	bool m_generateLater = false;
	bool m_blocksGenerated = false;

	int m_positionX;
	int m_positionZ;



	SubChunck * m_subChuncks[Chunck::height];

};
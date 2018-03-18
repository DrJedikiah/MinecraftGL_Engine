#pragma once

#include "engine/map/SubChunck.h"
#include "engine/generators/TreeGen.h"
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

	void GenerateBlocks(); 
	void LateGenerateBlocks();
	void GenerateMesh(int subChunck);
	void GenerateModels(int subChunck);
	void GenerateCollider(int subChunck, bool regenerate = false );

	void GenerateTree(Node * tree);

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

	std::vector< Node *> m_pendingTrees;

	static TreeGen m_treeGen;
	static std::default_random_engine generator;
	static std::uniform_real_distribution<float> distribution;
};
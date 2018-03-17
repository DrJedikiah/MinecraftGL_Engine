#pragma once

#include <stack>
#include <limits>

#include "graphics/Drawable.h"
#include "engine/Physics.h"
#include "engine/map/Chunck.h"
#include "engine/Camera.h"
#include "engine/generators/TreeGen.h"
#include "engine/map/CircularArray.h"
#include "util/Perlin.h"

class Chunck;
class CircularArray;


class World : IWithDebug
{ 
public:
	const static int size = 25;
 
	static void Update(float delta);

	static void DrawTransparent(const Shader & shader);
	static void DrawOpaque(const Shader & shader);

	static void GenerateChunks();

	static Chunck* GetChunck( int x, int z );
	static Block* GetBlock(glm::ivec3 position);

	static void RemoveBlock(glm::ivec3 position);

	static void GenerateTree(glm::ivec3 position, float size);

	static void SetBlock(glm::ivec3 position, Block::Type blockType);
	static glm::ivec3 BlockAt(glm::vec3 worldPos);
	static void UpdateAround(glm::ivec3 position);
	static void UpdateBlock(glm::ivec3 position);
	static void CenterChuncksAround(glm::ivec3 chunckPos);
	static void EnableAllChuncks();
	static void ClipChuncks( const Camera & camera );



	static glm::ivec3 GetOrigin();
private:
	void OnDrawDebug() const override;

	//Singleton pattern
	static World m_instance;
	World& operator= (const World&) {}
	World(const Physics&) {}
	World(); 
	~World();

	static TreeGen m_treeGen;
	static Node * m_lastTree;

	static CircularArray m_array;
};




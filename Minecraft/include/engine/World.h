#pragma once


#include "util/Perlin.h"
#include "engine/Physics.h"
#include "engine/Chunck.h"
#include "graphics/Drawable.h"

class Chunck;

class World
{
public:
	const static int size = 2;
	const static int height = 1;

	static void Update(float delta);

	static void Draw(const Shader& shader);
	static void GenerateChunks();
	static void GeneratePhysics();

	static Chunck & GetChunck(glm::ivec3 position);
	static bool BlockGenerated(glm::ivec3 position);
	static Block & GetBlock(glm::ivec3 position);

	static void RemoveBlock(glm::ivec3 position);
	static void UpdateAround(glm::ivec3 position);
	static void UpdateBlock(glm::ivec3 position);

private:
	//Singleton pattern
	static World m_instance;
	World& operator= (const World&) {}
	World(const PhysicsEngine&) {}
	World();
	~World();

	static bool IsVisible(glm::ivec3 position);

	static PerlinNoise perlinGen;
	static Chunck ***m_chuncks;
};




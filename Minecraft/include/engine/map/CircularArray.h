#pragma once

#include <vector>
#include <list>
#include <iostream>

#include "engine/map/World.h"
#include <engine/generators/ChunckGenerator.h>


class ChunckGenerator;
class World;
class Chunck;
class SubChunck;

class CircularArray
{
public:
	CircularArray( int size,int originX, int originZ);
	~CircularArray();

	void Update(float delta);
	void UpdateSubChunckMesh( SubChunck* subChunck);
	bool InsideArray(int x, int z) const;

	void MoveRight();
	void MoveLeft();
	void MoveBack();
	void MoveFront();

	Chunck* Get(int x, int z);
	void Set(int x, int z, Chunck* chunck);

	int Size() const;
	int OriginX() const;
	int OriginZ() const;

private:
	void DeleteChunck( Chunck * chunck);

	ChunckGenerator * m_chunckGenerator;

	std::vector< std::vector<Chunck*>> m_array;
	std::vector<Chunck*> m_toDelete;

	int m_size;
	int m_xOrigin;
	int m_zOrigin;
	int m_xOffset;
	int m_zOffset;
}; 
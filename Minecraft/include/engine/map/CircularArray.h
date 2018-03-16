#pragma once

#include <vector>
#include <iostream>

class Chunck;

class CircularArray
{
public:
	CircularArray( int size,int originX, int originZ);


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
	std::vector< std::vector<Chunck*>> m_array;
	int m_size;
	int m_xOrigin;
	int m_zOrigin;
	int m_xOffset;
	int m_zOffset;
};
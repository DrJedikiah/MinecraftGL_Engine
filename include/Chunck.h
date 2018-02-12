#pragma once

#include <vector>

#include "Cube.h"
#include "Shader.h"
#include "Model.h"


class Chunck
{
public:
	Chunck();

	static const int size;

	void GenerateMesh();
	void Draw(const Shader& shader) const;

	Cube& GetCube( int x, int y, int z);

	friend class World;

private:
	Model m_model;
	std::vector<Cube> m_cubes;

};


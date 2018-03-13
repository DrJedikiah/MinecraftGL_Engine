#pragma once

#include <vector>

class Statistics
{
public:
	Statistics();

	static int GetTriangles();

protected:
	bool STATS_enabled = true;
	int STATS_triangles = 0;

private:
	int index;
	static std::vector< Statistics* > m_instances;
};
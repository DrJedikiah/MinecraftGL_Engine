#pragma once

#include <map>

class Statistics
{
public:
	Statistics();
	~Statistics();

	static int GetTriangles();

protected:
	bool STATS_enabled = true;
	int STATS_triangles = 0;

private:
	int m_index;

	static int m_count;
	static std::map< int, Statistics* > * m_instances;
};
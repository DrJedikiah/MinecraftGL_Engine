#include "util/Statistics.h"

std::vector< Statistics* > Statistics::m_instances;

Statistics::Statistics()
{
	m_instances.push_back(this);
}

int Statistics::GetTriangles()
{
	int count = 0;
	for (Statistics* ws : m_instances)
	{
		if(ws->STATS_enabled)
			count += ws->STATS_triangles;
	}
	return count;
}
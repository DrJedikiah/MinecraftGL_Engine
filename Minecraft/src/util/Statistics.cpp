#include "util/Statistics.h"


std::map< int, Statistics* >* Statistics::m_instances = nullptr;

int Statistics::m_count = 0;

Statistics::Statistics() : 
	m_index(m_count++)
{
	if (!m_instances)
		m_instances = new std::map< int, Statistics* >();

	(*m_instances)[m_index] = this;
}

int Statistics::GetTriangles()
{
	int nbTriangles = 0;
	for ( std::pair<int, Statistics*>  pair : (*m_instances))
	{
		if(pair.second->STATS_enabled)
			nbTriangles += pair.second->STATS_triangles;
	}
	return nbTriangles;
}

Statistics::~Statistics()
{
	(*m_instances).erase(m_index);
}

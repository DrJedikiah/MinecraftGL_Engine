#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <stack>
#include <random>
#include <math.h>

#include "util/Debug.h"
#include "util/Input.h"
 


class Node
{
public:

	
	Node(glm::vec3 position, Node* parent2, int depth2);
	~Node();


	glm::vec3 position;
	glm::vec3 direction;
	Node * parent;
	std::vector< Node* > next;

	float length;  
	int depth; 

};

class TreeGen
{
public:
	TreeGen();
	 
	void GenerateTree(glm::vec3 position, float size);
	void Clear();

	Node * NewNode( Node* parent, int depth );

	Node * m_root;
	std::default_random_engine generator;
	std::uniform_real_distribution<float> distribution;

	float maxLenght = 10.f;
}; 
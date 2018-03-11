#include "engine/TreeGen.h"

Node::Node(glm::vec3 pos, Node* parent2, int depth2) :
	position(pos),
	parent(parent2),
	depth(depth2)
{
	if (parent)
	{
		length = parent->length + glm::distance(position, parent->position);
		direction = normalize (position - parent->position);
	}
	else
	{
		direction = glm::vec3(0, 1, 0);
		length = 0.f;
	}
		
}

Node::~Node()
{
	for (Node * node : next)
		delete(node);
}

TreeGen::TreeGen() :
	distribution(0.f, 1.f)
{
	 
}

Node * TreeGen::GenerateTree(glm::vec3 position, float maxLenght)
{
	//Random gen
	generator.seed( Input::FrameCount() );

	//Init
	Node * root = new Node(position, nullptr, 0);
	std::stack<Node * > stack;
	stack.push(root);

	//Build
	while (!stack.empty())
	{
		Node * node = stack.top();
		stack.pop();

		if (node->length <= maxLenght)
		{
			//if trunk
			if (node->depth == 0)
			{
				//Continue trunk
				glm::vec3 dir = glm::vec3(0, 1, 0);
				Node * newNode = new Node(node->position + dir, node, 0);
				node->next.push_back(newNode);
				stack.push(newNode);

				//New branch ?
				if (node->length > maxLenght / 8 && distribution(generator) > 0.5f)
				{
					//Direction ?
					float angle = glm::radians(360.f * distribution(generator));
					float height = distribution(generator);
					glm::vec3 dir = glm::vec3(cos(angle), height, sin(angle));

					Node * newNode = new Node(node->position + dir, node, 1);

					//shorten the branch
					newNode->length += maxLenght * (0.3f * distribution(generator));

					node->next.push_back(newNode);
					stack.push(newNode);
				}
			}
			//Branch
			else if (node->depth == 1)
			{
				glm::vec3 dir = node->direction;

				//Goes up faster ?
				if (distribution(generator) > 1.f - node->length / maxLenght)
					dir = glm::normalize(dir + glm::vec3(0, 0.1, 0));

				//Continue branch
				Node * newNode = new Node(node->position + dir, node, 1);
				node->next.push_back(newNode);
				stack.push(newNode);

				//New small branch ?
				if (distribution(generator) > 0.5f)
				{
					//Direction ?
					float angle = glm::radians(180 * distribution(generator));

					glm::vec3 dir = glm::cross(node->direction, glm::vec3(0, 1.f, 0));
					glm::quat rot = glm::angleAxis(angle, node->direction);
					dir = dir * rot;
					Node * newNode = new Node(node->position + dir, node, 2);

					newNode->length -= maxLenght * (0.5f * distribution(generator));

					node->next.push_back(newNode);
					stack.push(newNode);
				}
			}
			//Small branch
			else if (node->depth == 2)
			{
				//Continue Small branch
				Node * newNode = new Node(node->position + node->direction, node, 2);
				newNode->length += maxLenght * (0.1f * distribution(generator));
				node->next.push_back(newNode);
				stack.push(newNode);
			}

		}

		if (node->depth > 0 || (node->depth == 0 && node->length > maxLenght * 3.f / 4))
		{
			glm::vec3 dir1(1, 0, 0);
			glm::vec3 dir2(-1, 0, 0);
			glm::vec3 dir3(0, 0, 1);
			glm::vec3 dir4(0, 0, -1);
			glm::vec3 dir5(0, 1, 0);

			glm::vec3 dir6(1, 0, 1);
			glm::vec3 dir7(1, 0, -1);
			glm::vec3 dir8(-1, 0, 1);
			glm::vec3 dir9(-1, 0, -1);

			glm::vec3 dir10(2, 0, 0);
			glm::vec3 dir11(-2, 0, 0);
			glm::vec3 dir12(0, 0, 2);
			glm::vec3 dir13(0, 0, -2);

			node->next.push_back(new Node(node->position + dir1, node, 3));
			node->next.push_back(new Node(node->position + dir2, node, 3));
			node->next.push_back(new Node(node->position + dir3, node, 3));
			node->next.push_back(new Node(node->position + dir4, node, 3));
			node->next.push_back(new Node(node->position + dir5, node, 3));

			node->next.push_back(new Node(node->position + dir6, node, 3));
			node->next.push_back(new Node(node->position + dir7, node, 3));
			node->next.push_back(new Node(node->position + dir8, node, 3));
			node->next.push_back(new Node(node->position + dir9, node, 3));

			node->next.push_back(new Node(node->position + dir10, node, 3));
			node->next.push_back(new Node(node->position + dir11, node, 3));
			node->next.push_back(new Node(node->position + dir12, node, 3));
			node->next.push_back(new Node(node->position + dir13, node, 3));

		}
	}

	return root;
}

Node * TreeGen::NewNode(Node* parent, int depth)
{
	//Trunk
	if (depth == 0)
	{
		glm::vec3 pos = parent->position;
		glm::vec3 dir = glm::vec3(0, 1, 0);

		return new Node(pos + dir, parent, 0);
	}

	return nullptr;

}

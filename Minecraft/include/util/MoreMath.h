#pragma once

#include <glm/glm.hpp>

class Plane
{
public :
	Plane(glm::vec3 planeOrigin, glm::vec3 planeNormal) : 
		origin(planeOrigin), 
		normal( glm::normalize(planeNormal))
	{}


	glm::vec3 origin;
	glm::vec3 normal;


	glm::vec3 Projection(glm::vec3 point)
	{
		glm::vec3 direction = point - origin;
		float dist = glm::dot(direction, normal);
		return point - dist * normal;
	}

	bool Right(glm::vec3 point)
	{
		glm::vec3 direction = point - origin;
		float dist = glm::dot(direction, normal);
		return dist > 0;
	}
};



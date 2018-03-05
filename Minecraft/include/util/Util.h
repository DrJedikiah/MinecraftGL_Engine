#pragma once

#include <vector>

#include "btBulletDynamicsCommon.h"
#include <glm/glm.hpp>



enum Tag { def, chunck, entity };

namespace glm
{
	inline glm::vec3 toVec3(btVector3 vector) { return glm::vec3(vector.getX(), vector.getY(), vector.getZ());  };
	inline float Lerp(float start, float end, float ratio) { return start + ratio * (end - start); }
}

namespace bt
{
	inline btVector3 toVec3(glm::vec3 vector) { return btVector3(vector.x, vector.y, vector.z); };
}

namespace std
{
	inline float clamp(float value, float min, float max)
	{
		if ( value < min ) return min;
		if (value > max) return max;
		return value;
	}
}

struct fRect
{
	float x = 0;
	float y = 0;
	float width = 0;
	float height = 0;
};
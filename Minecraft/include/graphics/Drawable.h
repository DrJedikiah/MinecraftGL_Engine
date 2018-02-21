#pragma once

#include "graphics/Shader.h"

class Drawable
{
public:
	virtual void Draw( const Shader& shader) const = 0;
};
#include "engine/map/Block.h"

const float Block::size = 1.f;

#define MAKE_STRINGS(VAR) #VAR,
const char* const Block::typeName[] = {
	SOME_ENUM(MAKE_STRINGS)
};
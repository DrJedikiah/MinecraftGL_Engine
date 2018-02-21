#pragma once

#include "graphics/Texture.h"
#include "engine/Block.h"

#include <array>

class TexturesBlocks
{
public:
	inline static fRect Top(Block::Type block) { return items[block].top; }
	inline static fRect Bot(Block::Type block) { return items[block].bot; }
	inline static fRect Left(Block::Type block) { return items[block].left; }
	inline static fRect Right(Block::Type block) { return items[block].right; }
	inline static fRect Front(Block::Type block) { return items[block].front; }
	inline static fRect Back(Block::Type block) { return items[block].back; }

	static void SetBlock(Block::Type block, fRect top, fRect bot, fRect left, fRect right, fRect front, fRect back)
	{
		items[block] = {  top, bot, left, right, front, back };
	}

	static void SetBlock(Block::Type block, fRect rect)
	{
		items[block] = { rect, rect, rect, rect, rect, rect };
	}

private:
	struct Item
	{
		fRect top;
		fRect bot;
		fRect left;
		fRect right;
		fRect front;
		fRect back;
	};

	static std::array< Item, 256 > items;
};
#include "graphics/TexturesBlocks.h"

std::array< TexturesBlocks::Item, 256 > TexturesBlocks::items;

void TexturesBlocks::Initialize()
{
	fRect dirtRect = Tiles::GetRectangle(Tiles::dirt);
	fRect grassSideRect = Tiles::GetRectangle(Tiles::grassSide);
	fRect grassTopRect = Tiles::GetRectangle(Tiles::grassTop);
	fRect playerRect = Tiles::GetRectangle(Tiles::player);

	SetBlock(Block::dirt, dirtRect);
	SetBlock(Block::grass, grassTopRect, dirtRect, grassSideRect, grassSideRect, grassSideRect, grassSideRect);
	SetBlock(Block::player, playerRect);
}
#include "graphics/TexturesBlocks.h"

std::array< TexturesBlocks::Item, 256 > TexturesBlocks::items;

void TexturesBlocks::Initialize()
{
	fRect invalidRect = Tiles::GetRectangle(Tiles::invalid);
	fRect dirtRect = Tiles::GetRectangle(Tiles::dirt);
	fRect grassSideRect = Tiles::GetRectangle(Tiles::grassSide);
	fRect grassTopRect = Tiles::GetRectangle(Tiles::grassTop);
	fRect stoneRect = Tiles::GetRectangle(Tiles::stone);
	fRect bedrockRect = Tiles::GetRectangle(Tiles::bedrock);
	fRect woodRect = Tiles::GetRectangle(Tiles::wood);
	fRect woodTopRect = Tiles::GetRectangle(Tiles::woodTop);
	fRect leafRect = Tiles::GetRectangle(Tiles::leaf);
	fRect glassRedRect = Tiles::GetRectangle(Tiles::glassRed);
	fRect glassBlueRect = Tiles::GetRectangle(Tiles::glassBlue);

	SetBlock(Block::dirt, dirtRect);
	SetBlock(Block::grass, grassTopRect, dirtRect, grassSideRect, grassSideRect, grassSideRect, grassSideRect);
	SetBlock(Block::invalid, invalidRect);
	SetBlock(Block::stone, stoneRect);
	SetBlock(Block::bedrock, bedrockRect);
	SetBlock(Block::wood, woodTopRect, woodTopRect, woodRect, woodRect, woodRect, woodRect);
	SetBlock(Block::leaf, leafRect);
	SetBlock(Block::glassRed, glassRedRect);
	SetBlock(Block::glassBlue, glassBlueRect);
}
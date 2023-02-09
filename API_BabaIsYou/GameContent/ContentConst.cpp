#include "ContentConst.h"
#include "ContentEnum.h"

ContentConst::ContentConst()
{
}

ContentConst::~ContentConst()
{
}

const int ContentConst::WIGGLE_MAXTICK = 3;
const float ContentConst::WIGGLE_TIME = 0.33f;
const float ContentConst::MOVE_SPEED = 9.0f;
const float ContentConst::FADE_TIME = 0.6f;
const float ContentConst::FADE_DELAY = 0.5f;
const float ContentConst::FADE_WRITE_DELAY = 4.0f;
const float ContentConst::CONGRATULATION_TIME = 1.2f;
const float ContentConst::CONGRATULATION_DELAY = 3.0f;
const float ContentConst::TITLE_CAMERA_SPEED = 40.0f;

const int ContentConst::GRID_SIZE_X = 33;
const int ContentConst::GRID_SIZE_Y = 18;
const int2 ContentConst::GRID_SIZE = {GRID_SIZE_X, GRID_SIZE_Y };
const float4 ContentConst::GRID_BITMAP_SIZE = { 38.0f, 38.0f };
const float4 ContentConst::ACTOR_SIZE = { 35.0f, 35.0f };
const float4 ContentConst::MAPTOOL_PALLET_SIZE = {50.0f, 50.0f};

std::map<int, int> ContentConst::mapTileRenderImageIndex;

int ContentConst::GetTile(const int _Key)
{
	if (0 >= mapTileRenderImageIndex.size())
	{
		TileImageIndexInit();
	}

	return mapTileRenderImageIndex[_Key];
}

void ContentConst::TileImageIndexInit()
{
	mapTileRenderImageIndex[DIR_FLAG::NONE] = 0;
	mapTileRenderImageIndex[DIR_FLAG::RIGHT] = 1;
	mapTileRenderImageIndex[DIR_FLAG::UP] = 2;
	mapTileRenderImageIndex[DIR_FLAG::RIGHT | DIR_FLAG::UP] = 3;
	mapTileRenderImageIndex[DIR_FLAG::LEFT] = 4;
	mapTileRenderImageIndex[DIR_FLAG::LEFT | DIR_FLAG::RIGHT] = 5;
	mapTileRenderImageIndex[DIR_FLAG::LEFT | DIR_FLAG::UP] = 6;
	mapTileRenderImageIndex[DIR_FLAG::LEFT | DIR_FLAG::UP | DIR_FLAG::RIGHT] = 7;
	mapTileRenderImageIndex[DIR_FLAG::DOWN] = 8;
	mapTileRenderImageIndex[DIR_FLAG::DOWN | DIR_FLAG::RIGHT] = 9;
	mapTileRenderImageIndex[DIR_FLAG::UP | DIR_FLAG::DOWN] = 10;
	mapTileRenderImageIndex[DIR_FLAG::UP | DIR_FLAG::DOWN | DIR_FLAG::RIGHT] = 11;
	mapTileRenderImageIndex[DIR_FLAG::LEFT | DIR_FLAG::DOWN] = 12;
	mapTileRenderImageIndex[DIR_FLAG::LEFT | DIR_FLAG::DOWN | DIR_FLAG::RIGHT] = 13;
	mapTileRenderImageIndex[DIR_FLAG::UP | DIR_FLAG::DOWN | DIR_FLAG::LEFT] = 14;
	mapTileRenderImageIndex[DIR_FLAG::UP | DIR_FLAG::DOWN | DIR_FLAG::LEFT | DIR_FLAG::RIGHT] = 15;
}
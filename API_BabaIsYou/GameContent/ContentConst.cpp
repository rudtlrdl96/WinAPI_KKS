#include "ContentConst.h"

ContentConst::ContentConst()
{
}

ContentConst::~ContentConst()
{
}

const int ContentConst::WIGGLE_MAXTICK = 3;
const float ContentConst::WIGGLE_TIME = 0.33f;
const float ContentConst::MOVE_SPEED = 12.0f;
const float ContentConst::FADE_TIME = 0.5f;
const float ContentConst::FADE_DELAY = 0.5f;
const float ContentConst::CONGRATULATION_TIME = 1.2f;
const float ContentConst::CONGRATULATION_DELAY = 3.0f;

const int ContentConst::GRID_SIZE_X = 33;
const int ContentConst::GRID_SIZE_Y = 18;
const int2 ContentConst::GRID_SIZE = {GRID_SIZE_X, GRID_SIZE_Y };
const float4 ContentConst::GRID_BITMAP_SIZE = { 38.0f, 38.0f };
const float4 ContentConst::ACTOR_SIZE = { 36.0f, 36.0f };
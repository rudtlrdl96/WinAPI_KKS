#include "ContentConst.h"

ContentConst::ContentConst()
{
}

ContentConst::~ContentConst()
{
}

const int ContentConst::WIGGLE_MAXTICK = 3;
const float ContentConst::WIGGLE_TIME = 0.33f;
const float ContentConst::MOVE_SPEED = 3.0f;


const int ContentConst::GRID_SIZE_X = 32;
const int ContentConst::GRID_SIZE_Y = 18;
const int2 ContentConst::GRID_SIZE = {GRID_SIZE_X, GRID_SIZE_Y };
const float4 ContentConst::ACTOR_SIZE = { 40.0f, 40.0f };
#pragma once

#include <map>
#include <GameEngineBase/GameEngineMath.h>
#include "ContentMath.h"

class ContentConst
{
public:
	static const int WIGGLE_MAXTICK;
	static const float WIGGLE_TIME;
	static const float MOVE_SPEED;
	static const float FADE_TIME;
	static const float FADE_DELAY;
	static const float FADE_WRITE_DELAY;
	static const float CONGRATULATION_TIME;
	static const float CONGRATULATION_DELAY;
	static const float TITLE_CAMERA_SPEED;

	static const int GRID_SIZE_X;
	static const int GRID_SIZE_Y;
	static const int2 GRID_SIZE;
	static const float4 GRID_BITMAP_SIZE;
	static const float4 ACTOR_SIZE;
	static const float4 MAPTOOL_PALLET_SIZE;

	static int GetTile(const int _Key);

public:
	ContentConst(const ContentConst& _Other) = delete;
	ContentConst(ContentConst&& _Other) noexcept = delete;
	ContentConst& operator=(const ContentConst& _Other) = delete;
	ContentConst& operator=(ContentConst&& _Other) noexcept = delete;

protected:

private:
	ContentConst();
	~ContentConst();

	static std::map<int, int> mapTileRenderImageIndex;

	static void TileImageIndexInit();
};
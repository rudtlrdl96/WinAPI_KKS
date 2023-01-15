#pragma once

#include <GameEngineBase/GameEngineMath.h>
#include "ContentMath.h"

enum class RENDER_ORDER
{
	BACKGROUND  = 0,
	WORLDMAP,
	TEXTUI = 10000,
	UIIMAGE,
	BUTTON,
	FADE,
};

enum class TEMP_ACTOR_TYPE
{
	BABA, // Chacter Start
	KEKE,
	ROBOT, // Chacter End
	BELT, // Belt Start, End
	WALL, // Tile Start
	WATER,
	LAVA,
	GRASS,
	BRICK,
	PLANK, // Tile End
	SKULL, // Dynamic Actor Start, End
	FLAG, // Static Actor Start
	FLOWER,
	ROCK, // Static Actor End
	BABA_TEXT, // Text Start
	KEKE_TEXT,
	ROBOT_TEXT,
	BELT_TEXT,
	WALL_TEXT,
	WATER_TEXT,
	LAVA_TEXT,
	GRASS_TEXT,
	BRICK_TEXT,
	PLANK_TEXT,
	SKULL_TEXT,
	FLAG_TEXT,
	FLOWER_TEXT,
	ROCK_TEXT,
	IS_TEXT,
	AND_TEXT,
	HAS_TEXT,
	YOU_TEXT,
	WIN_TEXT,
	STOP_TEXT,
	PUSH_TEXT,
	SINK_TEXT,
	DEFEAT_TEXT,
	HOT_TEXT,
	MELT_TEXT, // Text End 
	COUNT,
};

class ContentConst
{
public:
	static const int WIGGLE_MAXTICK;
	static const float WIGGLE_TIME;
	static const float MOVE_SPEED;

	static const int GRID_SIZE_X;
	static const int GRID_SIZE_Y;
	static const int2 GRID_SIZE;
	static const float4 ACTOR_SIZE;

public:
	ContentConst(const ContentConst& _Other) = delete;
	ContentConst(ContentConst&& _Other) noexcept = delete;
	ContentConst& operator=(const ContentConst& _Other) = delete;
	ContentConst& operator=(ContentConst&& _Other) noexcept = delete;

protected:

private:
	ContentConst();
	~ContentConst();
};
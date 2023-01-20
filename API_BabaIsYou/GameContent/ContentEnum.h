#pragma once

enum DIR_FLAG
{
	NONE	= 0,
	UP		= 1 << 0,
	DOWN	= 1 << 1,
	LEFT 	= 1 << 2,
	RIGHT	= 1 << 3,
};

enum class RENDER_ORDER
{
	BACKGROUND = 0,
	GRIDBACK,
	WORLDMAP,
	ACTORSTART,
	TEXTUI = 10000,
	UIIMAGE,
	BUTTON,
	FADE,
};

enum class TEMP_ACTOR_INDEX
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

enum class BEHAVIOR
{
	WAIT,
	MOVE,
	PUSH,
	TURN_LEFT,
	TURN_RIGHT,
	DEATH,
	WIN,
	DEFINE_ADD,
	DEFINE_REMOVE,
};


enum class ACTOR_RENDER_TYPE
{
	STATIC,
	DYNAMIC,
	CHARACTER,
	TILE,
	BELT,
};

enum class ACTOR_TYPE
{
	ACTOR,
	SUBJECT_TEXT,
	VERB_TEXT,
	DEFINE_TEXT,
};

enum class ACTOR_DEFINE
{
	NONE = 0,
	YOU = 1 << 0,
	PUSH = 1 << 1,
	STOP = 1 << 2,
	MOVE = 1 << 3,
	HOT = 1 << 4,
	MELT = 1 << 5,
	SINK = 1 << 6,
	DEFEAT = 1 << 7,
	WIN = 1 << 8,
};

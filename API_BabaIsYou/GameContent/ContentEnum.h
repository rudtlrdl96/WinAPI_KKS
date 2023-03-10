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
	WORLDMAPGRID,
	WORLDMAPSELECT,
	ACTORSTART,
	PARTICLE = 50000,
	TEXTUI = 100000,
	UIIMAGE,
	BUTTON,
	FADE,
	FADE_TEXT
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
	CHANGE_INFO,
};

enum class ACTOR_RENDER_TYPE
{
	STATIC,
	DYNAMIC,
	CHARACTER,
	TILE,
	BELT,
};

enum class TEXT_COLOR
{
	WHITE = 0,
	PINK = 1
};

enum class PARTICLE_COLOR
{
	WHITE	  = 0,
	BURGUNDY  = 1,
	GRAY	  = 2,
	JAVA	  = 3,
	ANZAC	  = 4,
	SUSHI	  = 5,
	CLAY	  = 6,
	MOCHA	  = 7,
	FLAX	  = 8,
	ROYALBLUE = 9,
	BLACK	  = 10,
	COUNT
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

enum class RULE_TYPE
{
	IS_SUBJECT,
	IS_DEFINE
};

enum class SOUND_GROUP
{
	BGM,
	EFFECT,
	UI
};
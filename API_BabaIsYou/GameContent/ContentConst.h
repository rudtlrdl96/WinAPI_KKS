#pragma once

#include <GameEngineBase/GameEngineMath.h>
#include "ContentMath.h"

enum RENDER_ORDER
{
	BACKGROUND,
	WORLDMAP,
	BABA,
	KEKE,
	TEXT,
	UIIMAGE,
	BUTTON,
	FADE,
};

class ContentConst
{
public:
	static const int WIGGLE_MAXTICK;
	static const float WIGGLE_TIME;
	static const float MOVE_SPEED;

	static const int2 GridSize;
	static const float4 PuzzleActorSize;

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
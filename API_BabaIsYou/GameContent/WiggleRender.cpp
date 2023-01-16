#include "WiggleRender.h"
#include <GameEngineCore/GameEngineRender.h>
#include <GameEngineBase/GameEngineDebug.h>

WiggleRender::WiggleRender(GameEngineRender* _WiggleRender, int _StartIndex, int _AnimLength, int _BitmapInterval) :
	Render(_WiggleRender),
	AnimLength(_AnimLength),
	BitmapInterval(_BitmapInterval),
	StartIndex(_StartIndex),
	CurIndex(_StartIndex),
	IsWiggle(false)
{
	if (nullptr == _WiggleRender)
	{
		MsgAssert("Nullptr Render를 추가하려 했습니다");
		return;
	}
}

WiggleRender::~WiggleRender()
{
}


void WiggleRender::SetRenderIndex()
{
	CurIndex = StartIndex + CurAnim + (CurWiggle * BitmapInterval) + static_cast<int>(DirIndex);
	Render->SetFrame(CurIndex);
}

void WiggleRender::Wiggle()
{
	if (false == IsWiggle)
	{
		CurWiggle = 0;
		return;
	}

	++CurWiggle;

	if (CurWiggle >= 3)
	{
		CurWiggle = 0;
	}

	SetRenderIndex();
}

void WiggleRender::PrevAnim()
{
	--CurAnim;

	if (0 > CurAnim)
	{
		CurAnim = AnimLength - 1;
	}

	SetRenderIndex();
}

void WiggleRender::NextAnim()
{
	++CurAnim;

	if (CurAnim >= AnimLength)
	{
		CurAnim = 0;
	}

	SetRenderIndex();
}


void WiggleRender::SetDirInterval(size_t _DirInterval)
{
	DirInterval = _DirInterval;
}

void WiggleRender::SetAnimDir(const int2& _Dir)
{
	if (int2::Right == _Dir)
	{
		DirIndex = 0;
	}
	else if (int2::Up == _Dir)
	{
		DirIndex = DirInterval;
	}
	else if (int2::Left == _Dir)
	{
		DirIndex = DirInterval * 2;
	}
	else if (int2::Down == _Dir)
	{
		DirIndex = DirInterval * 3;
	}

	SetRenderIndex();
}
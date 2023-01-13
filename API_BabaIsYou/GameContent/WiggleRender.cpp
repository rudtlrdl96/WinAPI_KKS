#include "WiggleRender.h"
#include <GameEngineCore/GameEngineRender.h>
#include <GameEngineBase/GameEngineDebug.h>

WiggleRender::WiggleRender(GameEngineRender* _WiggleRender, int _StartIndex, int _AnimLength, int _Interval) :
	Render(_WiggleRender),
	AnimLength(_AnimLength),
	Interval(_Interval),
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
	CurIndex = StartIndex + CurAnim + (CurWiggle * Interval);
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

void WiggleRender::NextAnim()
{
	++CurAnim;

	if (CurAnim >= AnimLength)
	{
		CurAnim = 0;
	}

	SetRenderIndex();
}

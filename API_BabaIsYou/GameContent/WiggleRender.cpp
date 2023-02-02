#include "WiggleRender.h"
#include <GameEngineCore/GameEngineRender.h>
#include <GameEngineBase/GameEngineDebug.h>

WiggleRender::WiggleRender(GameEngineRender* _WiggleRender, int _StartIndex, int _AnimLength, int _BitmapInterval) :
	Render(_WiggleRender),
	CurAnimationIndex(0),
	CurWiggleIndex(0),
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

void WiggleRender::Wiggle()
{
	if (true == IsWiggle)
	{
		++CurWiggleIndex;

		if (CurWiggleIndex >= 3)
		{
			CurWiggleIndex = 0;
		}
	}

	SetRenderIndex();
}

void WiggleRender::Reset()
{
	CurIndex = 0;
	CurAnimationIndex = 0;
	CurWiggleIndex = 0;
	CurTileIndex = 0;

	StartIndex = 0;
	AnimLength = 1;

	DirIndex = 0;
	DirInterval = 0;

	IsTile = false;
	IsWiggle = false;
	IsDir = false;
	IsAnimation = false;
}


void WiggleRender::SetRenderIndex()
{
	CurIndex = StartIndex;

	if (true == IsWiggle)
	{
		CurIndex += CurWiggleIndex * BitmapInterval;
	}

	if (true == IsAnimation)
	{
		CurIndex += CurAnimationIndex;
	}

	if (true == IsDir)
	{
		CurIndex += static_cast<int>(DirIndex);
	}

	if (true == IsText)
	{
		CurIndex += CurTextIndex;
	}

	if (true == IsTile)
	{
		CurIndex += CurTileIndex;
	}

	Render->SetFrame(CurIndex);
}


void WiggleRender::SetStartIndex(int _Index)
{
	StartIndex = _Index;
	SetRenderIndex();
}

void WiggleRender::SetAnimLength(int _Length)
{
	AnimLength = _Length;
	SetRenderIndex();
}

void WiggleRender::SetTextIndex(int _TextIndex)
{
	CurTextIndex = _TextIndex;
	SetRenderIndex();
}

void WiggleRender::SetTileIndex(int _Index)
{
	CurTileIndex = _Index;
	SetRenderIndex();
}


void WiggleRender::SetDirInterval(size_t _DirInterval)
{
	DirInterval = _DirInterval;
	SetRenderIndex();
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

void WiggleRender::SetScale(const float4& _Scale)
{
	Render->SetScale(_Scale);
}

void WiggleRender::PrevAnim()
{
	--CurAnimationIndex;

	if (0 > CurAnimationIndex)
	{
		CurAnimationIndex = AnimLength - 1;
	}

	SetRenderIndex();
}

void WiggleRender::NextAnim()
{
	++CurAnimationIndex;

	if (CurAnimationIndex >= AnimLength)
	{
		CurAnimationIndex = 0;
	}

	SetRenderIndex();
}

void WiggleRender::ResetAnim()
{
	CurAnimationIndex = 0;
	SetRenderIndex();
}

void WiggleRender::RenderOn()
{
	Render->On();
}

void WiggleRender::RenderOff()
{
	Render->Off();
}

void WiggleRender::CameraEffectOff()
{
	Render->EffectCameraOff();
}

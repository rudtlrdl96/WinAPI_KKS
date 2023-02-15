#include "WiggleRender.h"
#include <GameEngineCore/GameEngineRender.h>
#include <GameEngineBase/GameEngineDebug.h>

WiggleRender::WiggleRender(int _StartIndex, int _AnimLength, int _BitmapInterval) :
	CurAnimationIndex(0),
	CurWiggleIndex(0),
	AnimLength(_AnimLength),
	BitmapInterval(_BitmapInterval),
	StartIndex(_StartIndex),
	CurIndex(_StartIndex),
	IsWiggle(false)
{
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

	CalculationRenderIndex();
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


void WiggleRender::SetRender(GameEngineRender* _WiggleRender)
{
	if (nullptr == _WiggleRender)
	{
		MsgAssert("nullptr render를 인자값으로 입력했습니다");
		return;
	}

	if (nullptr != Render)
	{
		_WiggleRender->SetScale(Render->GetScale());

		Render->Death();
		Render = nullptr;
	}

	Render = _WiggleRender;
}

void WiggleRender::SetOrder(int _Order)
{
	Render->SetOrder(_Order);
}

void WiggleRender::CalculationRenderIndex()
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

	if (nullptr == Render)
	{
		MsgAssert("WiggleRender를 설정하지 않고 사용하려 했습니다. ");
		return;
	}

	Render->SetFrame(CurIndex);
}


void WiggleRender::SetStartIndex(int _Index)
{
	StartIndex = _Index;
	CalculationRenderIndex();
}

void WiggleRender::SetAnimLength(int _Length)
{
	AnimLength = _Length;
	CalculationRenderIndex();
}

void WiggleRender::SetCompletionText(bool _IsCompletion)
{
	if (true == _IsCompletion)
	{
		CurTextIndex = 0;
	}
	else
	{
		CurTextIndex = 1;
	}

	CalculationRenderIndex();
}

void WiggleRender::SetTileIndex(int _Index)
{
	CurTileIndex = _Index;
	CalculationRenderIndex();
}


void WiggleRender::SetDirInterval(size_t _DirInterval)
{
	DirInterval = _DirInterval;
	CalculationRenderIndex();
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

	CalculationRenderIndex();
}

void WiggleRender::SetScale(const float4& _Scale)
{
	if (nullptr == Render)
	{
		MsgAssert("WiggleRender를 설정하지 않고 사용하려 했습니다. ");
		return;
	}

	Render->SetScale(_Scale);
}

void WiggleRender::PrevAnim()
{
	--CurAnimationIndex;

	if (0 > CurAnimationIndex)
	{
		CurAnimationIndex = AnimLength - 1;
	}

	CalculationRenderIndex();
}

void WiggleRender::NextAnim()
{
	++CurAnimationIndex;

	if (CurAnimationIndex >= AnimLength)
	{
		CurAnimationIndex = 0;
	}

	CalculationRenderIndex();
}

void WiggleRender::ResetAnim()
{
	CurAnimationIndex = 0;
	CalculationRenderIndex();
}

void WiggleRender::RenderOn()
{
	if (nullptr == Render)
	{
		MsgAssert("WiggleRender를 설정하지 않고 사용하려 했습니다. ");
		return;
	}

	Render->On();
}

void WiggleRender::RenderOff()
{
	if (nullptr == Render)
	{
		MsgAssert("WiggleRender를 설정하지 않고 사용하려 했습니다. ");
		return;
	}

	Render->Off();
}

void WiggleRender::CameraEffectOff()
{
	if (nullptr == Render)
	{
		MsgAssert("WiggleRender를 설정하지 않고 사용하려 했습니다. ");
		return;
	}

	Render->EffectCameraOff();
}

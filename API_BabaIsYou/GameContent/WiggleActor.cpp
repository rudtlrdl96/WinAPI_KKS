#include "WiggleActor.h"

#include <GameEngineBase/GameEngineDebug.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineImage.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineRender.h>

#include "ContentConst.h"
#include "WiggleRender.h"

WiggleActor::WiggleActor() :
	Render(nullptr)
{
}

WiggleActor::~WiggleActor()
{
	if (nullptr != Render)
	{
		delete Render;
		Render = nullptr;
	}
}

void WiggleActor::Update(float _DT)
{
	if (nullptr == Render)
	{
		return;
	}

	ProgressTime += _DT;

	if (ProgressTime >= ContentConst::WIGGLE_TIME)
	{
		ProgressTime = 0.0f;
		Render->Wiggle();
	}
}

void WiggleActor::InitRender(const WiggleAnimationParameter& _Parameter)
{
	if (nullptr != Render)
	{
		MsgAssert("Render를 중복 초기화하려 했습니다");
		return;
	}

	GameEngineRender* TempRender = CreateRender(_Parameter.FileName, _Parameter.Order);

	if (true == _Parameter.IsUI)
	{
		TempRender->EffectCameraOff();
	}

	if (nullptr == TempRender)
	{
		MsgAssert("GameEngineRender의 생성에 실패했습니다");
		return;
	}

	TempRender->SetPosition(_Parameter.Pos);
	TempRender->SetScale(_Parameter.Scale);

	Render = new WiggleRender(TempRender, _Parameter.StartIndex, _Parameter.AnimLength, _Parameter.BitmapInterval);

	if (0 >= _Parameter.BitmapInterval)
	{
		Render->DisableWiggle();
	}
	else
	{
		Render->ActiveWiggle();
	}
}

void WiggleActor::SetFrame(int _Frame)
{
	Render->SetStartIndex(_Frame);
	Render->SetRenderIndex();
}

void WiggleActor::SetLength(int _Length)
{
	Render->SetAnimLength(_Length);
	Render->SetRenderIndex();
}


void WiggleActor::SetDirInterval(size_t _DirInterval)
{
	Render->SetDirInterval(_DirInterval);
}

void WiggleActor::ResetAnim()
{
	Render->ResetAnim();
}

void WiggleActor::PrevAnim()
{
	Render->PrevAnim();
}

void WiggleActor::SetTileIndex(int _Index)
{
	Render->SetTileIndex(_Index);
}

void WiggleActor::NextAnim()
{
	Render->NextAnim();
}

void WiggleActor::SetAnimDir(const int2& _Dir)
{
	Render->SetAnimDir(_Dir);
}

void WiggleActor::RenderOn()
{
	Render->RenderOn();
}

void WiggleActor::RenderOff()
{
	Render->RenderOff();
}
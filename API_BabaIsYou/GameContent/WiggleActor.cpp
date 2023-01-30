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

void WiggleActor::InitWiggleRender(const WiggleAnimationParameter& _Parameter)
{
	if (nullptr != Render)
	{
		MsgAssert("Render�� �ߺ� �ʱ�ȭ�Ϸ� �߽��ϴ�");
		return;
	}

	GameEngineRender* TempRender = CreateRender(_Parameter.FileName, _Parameter.Order);

	if (true == _Parameter.IsUI)
	{
		TempRender->EffectCameraOff();
	}

	if (nullptr == TempRender)
	{
		MsgAssert("GameEngineRender�� ������ �����߽��ϴ�");
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

WiggleRender* WiggleActor::GetWiggleRender() const
{
	if (nullptr == Render)
	{
		MsgAssert("WiggleRender�� �ʱ�ȭ���� �ʰ� ����Ϸ� �߽��ϴ�.");
	}

	return Render;
}
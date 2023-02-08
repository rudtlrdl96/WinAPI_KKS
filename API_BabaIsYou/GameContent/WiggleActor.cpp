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

WiggleRender* WiggleActor::InitWiggleRender(const WiggleAnimationParameter& _Parameter)
{
	if (nullptr != Render)
	{
		MsgAssert("Render�� �ߺ� �ʱ�ȭ�Ϸ� �߽��ϴ�");
		return nullptr;
	}

	GameEngineRender* TempRender = CreateRender(_Parameter.FileName, _Parameter.Order);

	if (true == _Parameter.IsUI)
	{
		TempRender->EffectCameraOff();
	}

	if (nullptr == TempRender)
	{
		MsgAssert("GameEngineRender�� ������ �����߽��ϴ�");
		return nullptr;
	}

	TempRender->SetPosition(_Parameter.Pos);
	TempRender->SetScale(_Parameter.Scale);

	Render = new WiggleRender(_Parameter.StartIndex, _Parameter.AnimLength, _Parameter.BitmapInterval);
	Render->SetRender(TempRender);

	if (0 >= _Parameter.BitmapInterval)
	{
		Render->DisableWiggle();
	}
	else
	{
		Render->ActiveWiggle();
	}

	return Render;
}

WiggleRender* WiggleActor::GetWiggleRender() const
{
	if (nullptr == Render)
	{
		MsgAssert("WiggleRender�� �ʱ�ȭ���� �ʰ� ����Ϸ� �߽��ϴ�.");
	}

	return Render;
}
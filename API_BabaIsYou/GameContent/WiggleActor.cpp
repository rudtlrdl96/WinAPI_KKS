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

void WiggleActor::InitRender(const std::string_view& _FileName, const float4& _Pos, const float4& _Scale, int _StartIndex, int _AnimLength, int _Order,  int _Interval)
{
	if (nullptr != Render)
	{
		MsgAssert("Render를 중복 초기화하려 했습니다");
		return;
	}

	GameEngineRender* TempRender = CreateRender(_FileName, _Order);

	if (nullptr == TempRender)
	{
		MsgAssert("GameEngineRender의 생성에 실패했습니다");
		return;
	}

	TempRender->SetPosition(_Pos);
	TempRender->SetScale(_Scale);

	Render = new WiggleRender(TempRender, _StartIndex, _AnimLength, _Interval);


	if (0 >= _Interval)
	{
		Render->DisableWiggle();
	}
	else
	{
		Render->ActiveWiggle();
	}
}
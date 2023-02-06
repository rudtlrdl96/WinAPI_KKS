#include "WiggleTextUI.h"
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEngineCore/GameEngineRender.h>
#include "ContentEnum.h"
#include "WiggleRender.h"
#include "ContentConst.h"

WiggleTextUI::WiggleTextUI() :
	Render(nullptr)
{
}

WiggleTextUI::~WiggleTextUI()
{
	if (nullptr != Render)
	{
		delete Render;
		Render = nullptr;
	}
}

void WiggleTextUI::Init(int _Order)
{
	if (nullptr != Render)
	{
		MsgAssert("Render를 중복 초기화하려 했습니다");
		return;
	}

	GameEngineRender* TempRender = CreateRender("WiggleText.BMP", _Order);

	if (nullptr == TempRender)
	{
		MsgAssert("GameEngineRender의 생성에 실패했습니다");
		return;
	}

	TempRender->EffectCameraOff();
	TempRender->SetScale({10, 10});

	Render = new WiggleRender(0, 1, 37);
	Render->SetRender(TempRender);
	Render->ActiveWiggle();
}

void WiggleTextUI::Update(float _DT)
{
	TextUI::Update(_DT);

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

void WiggleTextUI::SetText(char _Text)
{
	_Text = toupper(_Text);

	int ColorIndex = static_cast<int>(TextColor) * 111;

	if ('0' <= _Text && '9' >= _Text)
	{
		Render->SetStartIndex(ColorIndex + _Text - '0' + 1);
	}
	else if ('A' <= _Text && 'Z' >= _Text)
	{
		Render->SetStartIndex(ColorIndex + 11 + _Text - 'A');
	}
	else
	{
		Render->SetStartIndex(ColorIndex + 0);
	}
}

void WiggleTextUI::SetScale(const float4& _Scale)
{
	Render->SetScale(_Scale);
}

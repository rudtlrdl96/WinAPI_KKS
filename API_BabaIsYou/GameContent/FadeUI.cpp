#include "FadeUI.h"
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineRender.h>
#include <GameEngineCore/GameEngineLevel.h>

#include "ContentMath.h"
#include "ContentEnum.h"
#include "ContentConst.h"
#include "StringUI.h"

FadeUI::FadeUI()
{
}

FadeUI::~FadeUI()
{
}

void FadeUI::Start()
{
	FadeRender = CreateRender(RENDER_ORDER::FADE);

	FadeRender->CreateAnimation({ 
		.AnimationName = "FadeIn",
		.ImageName = "FadeAnim.BMP", 
		.Start = 0, .End = 34,
		.InterTime = ContentConst::FADE_TIME / 34,
		.Loop = false});
	
	
	std::vector<int> FadeOutAnimIndex;
	FadeOutAnimIndex.reserve(35);

	for (int i = 34; i >= 0; --i)
	{
		FadeOutAnimIndex.push_back(i);
	}

	FadeRender->CreateAnimation({ 
		.AnimationName = "FadeOut", 
		.ImageName = "FadeAnim.BMP", 
		.InterTime = ContentConst::FADE_TIME / 34, 
		.Loop = false, 
		.FrameIndex = FadeOutAnimIndex});

	FadeRender->ChangeAnimation("FadeIn");

	FadeRender->SetPosition(GameEngineWindow::GetScreenSize().half());
	FadeRender->SetScale(GameEngineWindow::GetScreenSize());
	FadeRender->EffectCameraOff();
	FadeRender->Off();

	BoxRender = CreateRender("Background_Gray.BMP", RENDER_ORDER::FADE);
	
	BoxRender->SetPosition(GameEngineWindow::GetScreenSize().half());
	BoxRender->SetScale(GameEngineWindow::GetScreenSize());
	BoxRender->EffectCameraOff();
	
	BoxRender->Off();

	TopStringUIRender = GetLevel()->CreateActor<StringUI>();
	TopStringUIRender->SetTextRenderOrder(static_cast<int>(RENDER_ORDER::FADE_TEXT));
	TopStringUIRender->SetTextOrder(StringUI::TEXT_ORDER::MIDDLE);
	TopStringUIRender->SetPos(GameEngineWindow::GetScreenSize().half() - float4{0.0f, 70.0f});

	MiddleStringUIRender = GetLevel()->CreateActor<StringUI>();
	MiddleStringUIRender->SetTextRenderOrder(static_cast<int>(RENDER_ORDER::FADE_TEXT));
	MiddleStringUIRender->SetTextOrder(StringUI::TEXT_ORDER::MIDDLE);
	MiddleStringUIRender->SetPos(GameEngineWindow::GetScreenSize().half());
}

void FadeUI::Update(float _Time)
{
	if (true == IsDelay)
	{
		ProgressDelayTime += _Time;

		if (ProgressDelayTime >= DelayTime)
		{
			IsProgressValue = false;
			IsDelay = false;
			FadeRender->Off();
			BoxRender->Off();
			Off();
		}

		return;
	}

	if (true == IsWait)
	{
		ProgressWaitTime += _Time;

		if (ProgressWaitTime >= WaitTime)
		{
			IsWait = false;
			FadeRender->On();
			BoxRender->Off();

			TopStringUIRender->StringOff();
			MiddleStringUIRender->StringOff();
		}

		return;
	}

	FadeRatio += _Time;

	if (ContentConst::FADE_TIME <= FadeRatio)
	{
		switch (State)
		{
		case FADE_STATE::FADEIN:
			BoxRender->On();
			break;
		}

		if (nullptr != EndFunction)
		{
			EndFunction();
			EndFunction = nullptr;
		}

		IsDelay = true;
	}

}

void FadeUI::Fade(const FadeParameter& _FadeParameter)
{
	State = _FadeParameter.State;

	if (FADE_STATE::FADEIN == State)
	{
		FadeRender->ChangeAnimation("FadeIn");
		BoxRender->Off();
	}
	else if (FADE_STATE::FADEOUT == State)
	{
		FadeRender->ChangeAnimation("FadeOut");
		BoxRender->On();
	}

	IsProgressValue = true;
	EndFunction = _FadeParameter.Func;

	WaitTime = _FadeParameter.WaitTime;
	DelayTime = _FadeParameter.DelayTime;


	if ("" == _FadeParameter.WriteTopText)
	{
		TopStringUIRender->StringOff();
	}
	else
	{
		TopStringUIRender->SetFontInterval(_FadeParameter.TopTextInterval);
		TopStringUIRender->SetFontSize(_FadeParameter.TopTextSize);
		TopStringUIRender->WriteText(_FadeParameter.WriteTopText);
		TopStringUIRender->StringOn();
	}

	if ("" == _FadeParameter.WriteTopText)
	{
		MiddleStringUIRender->StringOff();
	}
	else 
	{	
		MiddleStringUIRender->SetFontInterval(_FadeParameter.MiddleTextInterval);
		MiddleStringUIRender->SetFontSize(_FadeParameter.MiddleTextSize);
		MiddleStringUIRender->WriteText(_FadeParameter.WriteMiddleText);
		MiddleStringUIRender->StringOn();
	}

	ProgressDelayTime = 0.0f;
	ProgressWaitTime = 0.0f;

	FadeRatio = 0.0f;
	DelayTime = 0.0f;
	IsWait = true;
	IsDelay = false;
	FadeRender->Off();

	On();
}
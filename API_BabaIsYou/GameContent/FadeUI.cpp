#include "FadeUI.h"
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineRender.h>

#include "ContentMath.h"
#include "ContentEnum.h"
#include "ContentConst.h"

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
	
	BoxRender->Off();
}

void FadeUI::Update(float _Time)
{
	ProgressTime += _Time;

	if (ContentConst::FADE_TIME <= ProgressTime)
	{
		switch (State)
		{
		case FADE_STATE::FADEIN:
		{
			BoxRender->On();
			DelayTime += _Time;

			if (ContentConst::FADE_DELAY <= DelayTime)
			{
				if (nullptr != EndFunction)
				{
					EndFunction();
					EndFunction = nullptr;
				}
				IsProgressValue = false;
			}
		}
			break;
		case FADE_STATE::FADEOUT:
		{
			if (nullptr != EndFunction)
			{
				EndFunction();
				EndFunction = nullptr;
			}
			IsProgressValue = false;
			Off();
		}
		return;
		}
	}

}


void FadeUI::Fade(FADE_STATE _State, void (*_Func)(void))
{
	State = _State;

	if (FADE_STATE::FADEIN == State)
	{
		FadeRender->ChangeAnimation("FadeIn");
	}
	else if (FADE_STATE::FADEOUT == State)
	{
		FadeRender->ChangeAnimation("FadeOut");
	}

	IsProgressValue = true;
	EndFunction = _Func;

	ProgressTime = 0.0f;
	DelayTime = 0.0f;
	BoxRender->Off();
	FadeRender->On();
	On();
}
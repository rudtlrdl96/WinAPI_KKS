#include "FadeUI.h"
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineRender.h>

#include "ContentMath.h"
#include "ContentConst.h"

std::map<GameEngineLevel*, FadeUI*> FadeUI::mapFades;
void (*FadeUI::FuncPtr)(void) = nullptr;

FadeUI::FadeUI()
{
}

FadeUI::~FadeUI()
{
}

void FadeUI::Start()
{
	mapFades.insert(std::make_pair(GetLevel(), this));
	FadeRender = CreateRender(RENDER_ORDER::FADE);

	FadeRender->CreateAnimation(
		{ .AnimationName = "FadeIn", .ImageName = "FadeAnim.BMP", .Start = 0, .End = 34, .InterTime = FadeTime / 34, .Loop = false});
	
	
	std::vector<int> FadeOutAnimIndex;
	FadeOutAnimIndex.reserve(35);

	for (int i = 34; i >= 0; --i)
	{
		FadeOutAnimIndex.push_back(i);
	}

	FadeRender->CreateAnimation(
		{ .AnimationName = "FadeOut", .ImageName = "FadeAnim.BMP", .InterTime = FadeTime / 34, .Loop = false, .FrameIndex = FadeOutAnimIndex});

	FadeRender->ChangeAnimation("FadeIn");

	FadeRender->SetPosition(GameEngineWindow::GetScreenSize().half());
	FadeRender->SetScale(GameEngineWindow::GetScreenSize());
	FadeRender->Off();

	BoxRender = CreateRender("Background_Gray.BMP", RENDER_ORDER::FADE);
	
	BoxRender->SetPosition(GameEngineWindow::GetScreenSize().half());
	BoxRender->SetScale(GameEngineWindow::GetScreenSize());
	
	BoxRender->Off();
}

void FadeUI::Update(float _Time)
{
	ProgressTime += _Time;

	if (FadeTime <= ProgressTime)
	{
		switch (State)
		{
		case FADE_STATE::FADEIN:
		{
			BoxRender->On();
			DelayTime += _Time;

			if (0.3f <= DelayTime)
			{
				if (nullptr != FuncPtr)
				{
					FuncPtr();
				}
			}
		}
			break;
		case FADE_STATE::FADEOUT:
		{
			if (nullptr != FuncPtr)
			{
				FuncPtr();
			}
			Off();
		}
		return;
		}
	}

}

void FadeUI::SetState(FADE_STATE _State)
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

	ProgressTime = 0.0f;
	DelayTime = 0.0f;
	BoxRender->Off();
	FadeRender->On();
	On();
}

void FadeUI::ActiveFade(FADE_STATE _State, GameEngineLevel* _ParentLevel, void (*_Func)(void))
{
	if (nullptr == _ParentLevel)
	{
		MsgAssert("Nullptr Level을 참조하려 했습니다.");
		return;
	}

	FadeUI* FadeActor = mapFades[_ParentLevel];

	if (FadeActor == nullptr)
	{
		MsgAssert("삭제된 페이트 액터를 참조하려 합니다.");
	}

	if (FadeActor->GetLevel() == _ParentLevel)
	{
		FadeActor->SetState(_State);
	}

	FuncPtr = _Func;
}
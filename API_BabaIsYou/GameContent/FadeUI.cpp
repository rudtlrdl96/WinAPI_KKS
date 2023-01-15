#include "FadeUI.h"
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineRender.h>

#include "ContentMath.h"
#include "ContentConst.h"
#include "ContentAnim.h"

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
	FadeRender = CreateRender("FadeAnim.BMP", RENDER_ORDER::FADE);
	FadeRender->SetPosition(GameEngineWindow::GetScreenSize().half());
	FadeRender->SetScale(GameEngineWindow::GetScreenSize());
}

void FadeUI::Update(float _Time)
{
	ProgressTime += _Time / FadeTime;

	if (1.0f <= ProgressTime)
	{
		ProgressTime = 1.0f;
		DelayTime += _Time;

		if (FADE_STATE::FADEOUT == State)
		{
			Off();
		}
	}

	switch (State)
	{
	case FADE_STATE::NONE:
	{
		MsgAssert("잘못된 FadeUI Enum State 입니다");
	}
		return;
	case FADE_STATE::FADEIN:
		FadeRender->SetFrame(static_cast<int>(34 * ProgressTime));
		break;
	case FADE_STATE::FADEOUT:
		FadeRender->SetFrame(static_cast<int>(34 - (34 * ProgressTime)));
		break;
	default:
		MsgAssert("잘못된 FadeUI Enum State 입니다");
		return;
	}

	if (0.4f <= DelayTime)
	{
		if (nullptr != FuncPtr)
		{
			FuncPtr();
		}

		FuncPtr = nullptr;
	}
}

void FadeUI::FadeIn(GameEngineLevel* _ParentLevel, void (*_Func)(void))
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
		FadeActor->State = FADE_STATE::FADEIN;
		FadeActor->DelayTime = 0.0f;
		FadeActor->ProgressTime = 0.0f;
		FadeActor->On();
	}

	FuncPtr = _Func;
}

void FadeUI::FadeOut(GameEngineLevel* _ParentLevel, void (*_Func)(void))
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
		FadeActor->State = FADE_STATE::FADEOUT;
		FadeActor->DelayTime = 0.0f;
		FadeActor->ProgressTime = 0.0f;
		FadeActor->On();
	}


	FuncPtr = _Func;
}

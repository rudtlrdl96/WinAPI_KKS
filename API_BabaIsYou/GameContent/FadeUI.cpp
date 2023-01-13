#include "FadeUI.h"
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineRender.h>

#include "ContentMath.h"
#include "ContentConst.h"
#include "ContentAnim.h"

std::vector<FadeUI*> FadeUI::vecFades;
void (*FadeUI::FuncPtr)(void) = nullptr;

FadeUI::FadeUI()
{
}

FadeUI::~FadeUI()
{
	if (nullptr != FadeRender)
	{
		delete FadeRender;
		FadeRender = nullptr;
	}
}

void FadeUI::Start()
{
	vecFades.push_back(this);

	GameEngineRender* TempRender = CreateRender("FadeAnim.BMP", RENDER_ORDER::FADE);
	TempRender->SetPosition(GameEngineWindow::GetScreenSize().half());
	TempRender->SetScale(GameEngineWindow::GetScreenSize());

	FadeRender = new ContentAnim(TempRender, 0, 10);

	Off();
}

void FadeUI::Update(float _Time)
{
	FadeRender->UpdateAnim(_Time);
	ProgressTime += _Time;

	if (ProgressTime >= 1.0f)
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

	for (FadeUI* FadeActor : vecFades)
	{
		if (FadeActor == nullptr)
		{
			MsgAssert("삭제된 페이트 액터를 참조하려 합니다.");
			continue;
		}

		if (FadeActor->GetLevel() == _ParentLevel)
		{
			FadeActor->ProgressTime = 0.0f;
			FadeActor->FadeRender->SetProgress(0.0f);
			FadeActor->FadeRender->DisableFilp();
			FadeActor->On();
		}
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

	for (FadeUI* FadeActor : vecFades)
	{
		if (FadeActor == nullptr)
		{
 			MsgAssert("삭제된 페이트 액터를 참조하려 합니다.");
			continue;
		}
		if (FadeActor->GetLevel() == _ParentLevel)
		{
			FadeActor->ProgressTime = 0.0f;
			FadeActor->FadeRender->SetProgress(1.0f);
			FadeActor->FadeRender->ActiveFilp();
			FadeActor->On();
		}
	}

	FuncPtr = _Func;
}

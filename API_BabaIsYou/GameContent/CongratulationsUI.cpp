#include "CongratulationsUI.h"
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineRender.h>
#include "ContentEnum.h"
#include "ContentConst.h"

CongratulationsUI::CongratulationsUI()
{
}

CongratulationsUI::~CongratulationsUI()
{
}

void CongratulationsUI::Start()
{
	CongratulationRender = CreateRender(RENDER_ORDER::UIIMAGE);

	CongratulationRender->SetScale({540, 180});
	CongratulationRender->SetPosition(GameEngineWindow::GetScreenSize().half());

	CongratulationRender->CreateAnimation({
		.AnimationName = "CongratulationsAnim",
		.ImageName = "CongratulationsAnim.BMP",
		.Start = 0, .End = 36,
		.InterTime = ContentConst::CONGRATULATION_TIME / 36,
		.Loop = false });

	CongratulationRender->CreateAnimation({
		.AnimationName = "CongratulationsWiggle",
		.ImageName = "CongratulationsWiggle.BMP",
		.Start = 0, .End = 8,
		.InterTime = ContentConst::WIGGLE_TIME / 3,
		.Loop = true });

	CongratulationRender->EffectCameraOff();
	Off();
}

void CongratulationsUI::Update(float _DT)
{
	ProgressTime += _DT;

	if (ContentConst::CONGRATULATION_TIME <= ProgressTime)
	{
		CongratulationRender->ChangeAnimation("CongratulationsWiggle");
	}

	if (ContentConst::CONGRATULATION_TIME + ContentConst::CONGRATULATION_DELAY <= ProgressTime)
	{
		if (nullptr == EndFunction)
		{
			MsgAssert("Congratulations 함수 호출 중 nullptr 함수 포인터가 인자로 들어왔습니다.");
			return;
		}
		IsProgressValue = false;
		EndFunction();
	}
}


void CongratulationsUI::Congratulations(void(*_EndFunction)(void))
{
	if (nullptr == _EndFunction)
	{
		MsgAssert("Congratulations 함수 호출 중 nullptr 함수 포인터가 인자로 들어왔습니다.");
		return;
	}

	IsProgressValue = true;
	EndFunction = _EndFunction;
	ProgressTime = 0.0f;
	CongratulationRender->ChangeAnimation("CongratulationsAnim");
	On();

}
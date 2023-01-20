#include "GrayBackUI.h"
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineRender.h>
#include "ContentEnum.h"

GrayBackUI::GrayBackUI()
{
}

GrayBackUI::~GrayBackUI()
{
}

void GrayBackUI::Start()
{
	BackRender = CreateRender("Background_Gray.BMP", RENDER_ORDER::BACKGROUND);
	BackRender->SetPosition(GameEngineWindow::GetScreenSize().half());
	BackRender->SetScale(GameEngineWindow::GetScreenSize());
	BackRender->EffectCameraOff();
}
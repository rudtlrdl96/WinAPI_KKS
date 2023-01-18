#include "BlackBackUI.h"
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineRender.h>
#include "ContentEnum.h"

BlackBackUI::BlackBackUI()
{
}

BlackBackUI::~BlackBackUI()
{
}
void BlackBackUI::Start()
{
	BackRender = CreateRender("Background_Black.BMP", RENDER_ORDER::GRIDBACK);
	BackRender->SetPosition(GameEngineWindow::GetScreenSize().half());
	BackRender->SetScale(GameEngineWindow::GetScreenSize());
	BackRender->EffectCameraOff();
}


void BlackBackUI::SetScale(const float4& _Scale)
{
	BackRender->SetScale(_Scale);
}
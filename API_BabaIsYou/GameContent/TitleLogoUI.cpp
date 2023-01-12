#include "TitleLogoUI.h"
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineRender.h>

TitleLogoUI::TitleLogoUI()
{
}

TitleLogoUI::~TitleLogoUI()
{
}

void TitleLogoUI::Start()
{
	{
		GameEngineRender* TitleLogoRender = CreateRender("TitleLogo.BMP", 0);
		TitleLogoRender->SetPosition(GameEngineWindow::GetScreenSize().half() - float4{ 0.0f, 135.0f });
		TitleLogoRender->SetScale({ 600, 250 });
	}
}

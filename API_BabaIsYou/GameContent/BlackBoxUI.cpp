#include "BlackBoxUI.h"
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineRender.h>

BlackBoxUI::BlackBoxUI()
{
}

BlackBoxUI::~BlackBoxUI()
{
}
void BlackBoxUI::Start()
{
	{
		GameEngineRender* BoxRender = CreateRender("Fade_Black.BMP", -10);
		BoxRender->SetScale(GameEngineWindow::GetScreenSize());
	}

	SetPos(GameEngineWindow::GetScreenSize().half());
}

void BlackBoxUI::Render(float _DT)
{
}
#include "LogoUI.h"
#include <GameEngineCore/GameEngineRender.h>

LogoUI::LogoUI()
{
}

LogoUI::~LogoUI()
{
}

void LogoUI::Start()
{
	{
		GameEngineRender* LogoRender = CreateRender("Logo.BMP", 0);
		LogoRender->SetPosition(GameEngineWindow::GetScreenSize().half());
		LogoRender->SetScale(GameEngineWindow::GetScreenSize());
	}
}
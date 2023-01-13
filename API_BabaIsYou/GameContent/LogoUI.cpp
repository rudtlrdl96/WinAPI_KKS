#include "LogoUI.h"
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineRender.h>
#include "ContentConst.h"

LogoUI::LogoUI()
{
}

LogoUI::~LogoUI()
{
}

void LogoUI::Start()
{
	InitRender("Logo.BMP",
		GameEngineWindow::GetScreenSize().half(),
		GameEngineWindow::GetScreenSize(),
		0, 1, RENDER_ORDER::UIIMAGE, 1);
}
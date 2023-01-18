#include "LogoUI.h"
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineRender.h>
#include "ContentEnum.h"

LogoUI::LogoUI()
{
}

LogoUI::~LogoUI()
{
}

void LogoUI::Start()
{
	InitRender({
	.FileName = "Logo.BMP",
	.Pos = GameEngineWindow::GetScreenSize().half(),
	.Scale = GameEngineWindow::GetScreenSize(),
	.StartIndex = 0,
	.AnimLength = 3,
	.Order = static_cast<int>(RENDER_ORDER::UIIMAGE),
	.BitmapInterval = 1,
	.IsUI = true });
}
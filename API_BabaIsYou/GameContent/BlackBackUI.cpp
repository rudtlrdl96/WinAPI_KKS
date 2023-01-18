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
	InitRender({
		.FileName = "Background_Black.BMP",
		.Pos = GameEngineWindow::GetScreenSize().half(),
		.Scale = GameEngineWindow::GetScreenSize(),
		.StartIndex = 0,
		.AnimLength = 1,
		.Order = static_cast<int>(RENDER_ORDER::BACKGROUND),
		.IsUI = true});
}
#include "GrayBackUI.h"
#include <GameEnginePlatform/GameEngineWindow.h>
#include "ContentEnum.h"

GrayBackUI::GrayBackUI()
{
}

GrayBackUI::~GrayBackUI()
{
}

void GrayBackUI::Start()
{

	InitRender({
	.FileName = "Background_Gray.BMP",
	.Pos = GameEngineWindow::GetScreenSize().half(),
	.Scale = GameEngineWindow::GetScreenSize(),
	.StartIndex = 0,
	.AnimLength = 1,
	.Order = static_cast<int>(RENDER_ORDER::BACKGROUND),
	.IsUI = true });
}
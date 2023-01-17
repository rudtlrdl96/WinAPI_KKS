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
	InitRender("Background_Gray.BMP",
		GameEngineWindow::GetScreenSize().half(),
		GameEngineWindow::GetScreenSize(),
		0, 1, static_cast<int>(RENDER_ORDER::BACKGROUND));
}
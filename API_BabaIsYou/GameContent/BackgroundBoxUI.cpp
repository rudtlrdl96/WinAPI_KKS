#include "BackgroundBoxUI.h"
#include <GameEnginePlatform/GameEngineWindow.h>
#include "ContentConst.h"

BackgroundBoxUI::BackgroundBoxUI()
{
}

BackgroundBoxUI::~BackgroundBoxUI()
{
}

void BackgroundBoxUI::Start()
{
	InitRender("Fade.BMP",
		GameEngineWindow::GetScreenSize().half(),
		GameEngineWindow::GetScreenSize(),
		0, 1, RENDER_ORDER::BACKGROUND);
}
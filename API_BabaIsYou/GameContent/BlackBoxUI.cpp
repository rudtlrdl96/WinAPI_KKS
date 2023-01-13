#include "BlackBoxUI.h"
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineRender.h>
#include "ContentConst.h"
BlackBoxUI::BlackBoxUI()
{
}

BlackBoxUI::~BlackBoxUI()
{
}
void BlackBoxUI::Start()
{
	InitRender("Fade_Black.BMP",
		GameEngineWindow::GetScreenSize().half(),
		GameEngineWindow::GetScreenSize(),
		0, 1, RENDER_ORDER::BACKGROUND);
}
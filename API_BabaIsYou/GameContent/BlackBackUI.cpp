#include "BlackBackUI.h"
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineRender.h>
#include "ContentConst.h"
BlackBackUI::BlackBackUI()
{
}

BlackBackUI::~BlackBackUI()
{
}
void BlackBackUI::Start()
{
	InitRender("Background_Black.BMP",
		GameEngineWindow::GetScreenSize().half(),
		GameEngineWindow::GetScreenSize(),
		0, 1, static_cast<int>(RENDER_ORDER::BACKGROUND));
}
#include "TitleLogoUI.h"
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineRender.h>
#include "ContentConst.h"

TitleLogoUI::TitleLogoUI()
{
}

TitleLogoUI::~TitleLogoUI()
{
}

void TitleLogoUI::Start()
{
	InitRender("TitleLogo.BMP",
		GameEngineWindow::GetScreenSize().half() - float4{ 0.0f, 135.0f },
		{ 600, 250 },
		0, 1, RENDER_ORDER::UIIMAGE, 1);
}

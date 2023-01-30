#include "TitleLogoUI.h"
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineRender.h>
#include "ContentEnum.h"

TitleLogoUI::TitleLogoUI()
{
}

TitleLogoUI::~TitleLogoUI()
{
}

void TitleLogoUI::Start()
{
	InitWiggleRender({
		.FileName = "TitleLogo.BMP",
		.Pos = GameEngineWindow::GetScreenSize().half() - float4{ 0.0f, 135.0f },
		.Scale = { 600, 250 },
		.StartIndex = 0,
		.AnimLength = 1,
		.Order = static_cast<int>(RENDER_ORDER::UIIMAGE),
		.BitmapInterval = 1,
		.IsUI = true });
}

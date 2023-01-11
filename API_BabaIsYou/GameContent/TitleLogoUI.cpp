#include "TitleLogoUI.h"
#include <GameEnginePlatform/GameEngineWindow.h>

TitleLogoUI::TitleLogoUI()
{
}

TitleLogoUI::~TitleLogoUI()
{
}

void TitleLogoUI::Start()
{
	TitleLogoImage = GameEngineResources::GetInst().ImageFind("TitleLogo.BMP");
	SetPos(GameEngineWindow::GetScreenSize().half() - float4{0.0f, 135.0f});
}

void TitleLogoUI::Render()
{
	GameEngineWindow::GetDoubleBufferImage()->TransCopy(
		TitleLogoImage,
		GetPos(),
		{ 600, 250 },
		{ 0, static_cast<float>(370 * (GetWiggleIndex())) },
		{ 900, 370 });
}

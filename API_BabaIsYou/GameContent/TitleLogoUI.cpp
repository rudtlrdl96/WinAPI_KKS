#include "TitleLogoUI.h"

TitleLogoUI::TitleLogoUI()
{
}

TitleLogoUI::~TitleLogoUI()
{
}

void TitleLogoUI::Start()
{
	SetPos({ 340, 95 });
	TitleLogoImage = GameEngineResources::GetInst().ImageFind("TitleLogo.BMP");
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

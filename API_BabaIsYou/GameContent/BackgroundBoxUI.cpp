#include "BackgroundBoxUI.h"

BackgroundBoxUI::BackgroundBoxUI()
{
}

BackgroundBoxUI::~BackgroundBoxUI()
{
}

void BackgroundBoxUI::Start()
{
	BoxImage = GameEngineResources::GetInst().ImageFind("Fade.BMP");
	SetPos(GameEngineWindow::GetScreenSize().half());
}

void BackgroundBoxUI::Render()
{
	GameEngineWindow::GetDoubleBufferImage()->TransCopy(
		BoxImage,
		GetPos(),
		GameEngineWindow::GetScreenSize(),
		float4::Zero,
		BoxImage->GetImageScale());
}
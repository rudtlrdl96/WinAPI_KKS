#include "BlackBoxUI.h"

BlackBoxUI::BlackBoxUI()
{
}

BlackBoxUI::~BlackBoxUI()
{
}
void BlackBoxUI::Start()
{
	BoxImage = GameEngineResources::GetInst().ImageFind("Fade_Black.BMP");
	SetPos(GameEngineWindow::GetScreenSize().half());
}

void BlackBoxUI::Render()
{
	GameEngineWindow::GetDoubleBufferImage()->TransCopy(
		BoxImage,
		GetPos(),
		GameEngineWindow::GetScreenSize(),
		float4::Zero,
		BoxImage->GetImageScale());
}
#include "FadeUI.h"

FadeUI::FadeUI()
{
}

FadeUI::~FadeUI()
{
}

void FadeUI::Start()
{
	FadeImage = GameEngineResources::GetInst().ImageFind("Fade.BMP");
}

void FadeUI::Render()
{
	GameEngineWindow::GetDoubleBufferImage()->TransCopy(
		FadeImage,
		float4::Zero, 
		GameEngineWindow::GetScreenSize(),
		float4::Zero,
		FadeImage->GetImageScale());
}
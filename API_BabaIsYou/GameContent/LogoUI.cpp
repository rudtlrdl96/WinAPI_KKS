#include "LogoUI.h"

LogoUI::LogoUI()
{
}

LogoUI::~LogoUI()
{
}

void LogoUI::Start()
{
	LogoImage = GameEngineResources::GetInst().ImageFind("Logo.BMP");
}

void LogoUI::Update()
{


}

void LogoUI::Render()
{ 
	GameEngineWindow::GetDoubleBufferImage()->TransCopy(
		LogoImage, 
		float4::Zero, GameEngineWindow::GetScreenSize(),
		{0, static_cast<float>(1080 * (GetWiggleIndex())) },
		{ 1920, 1080 });
}
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
	SetPos(GameEngineWindow::GetScreenSize().half());
}

void LogoUI::Update()
{

}

void LogoUI::Render()
{ 
	GameEngineWindow::GetDoubleBufferImage()->TransCopy(
		LogoImage, 
		GetPos(),
		GameEngineWindow::GetScreenSize(),
		{0, static_cast<float>(1080 * (GetWiggleIndex())) },
		{ 1920, 1080 });
}
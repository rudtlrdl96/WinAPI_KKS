#include "LogoUI.h"
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineImage.h>
#include <GameEngineCore/GameEngineResources.h>

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
	// todo : 키가 눌려졌을 경우 Level 변경
}

void LogoUI::Render()
{ 
	GameEngineWindow::GetDoubleBufferImage()->TransCopy(
		LogoImage, float4::Zero, GameEngineWindow::GetScreenSize(), float4::Zero, {1920, 1080}); 
}
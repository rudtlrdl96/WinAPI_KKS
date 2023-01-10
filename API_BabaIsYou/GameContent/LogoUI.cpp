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
	// todo : Ű�� �������� ��� Level ����
}

void LogoUI::Render()
{ 
	GameEngineWindow::GetDoubleBufferImage()->TransCopy(
		LogoImage, float4::Zero, GameEngineWindow::GetScreenSize(), float4::Zero, {1920, 1080}); 
}
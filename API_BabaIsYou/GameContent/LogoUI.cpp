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
	GameEngineWindow::GetBackBufferImage()->BitCopy(LogoImage, {0.0f, 0.0f}, { 1920, 1080 });
}